// ----------------------------------------------------------------
//
// ファイル：ThreadedRenderer.cpp
// 機能：マルチスレッドレンダリングシステムのメインクラス
// 作成：2025/05/08
//
// ----------------------------------------------------------------

#include "pch.h"
#include "ThreadedRenderer.h"

// シングルトンインスタンス
ThreadedRenderer* ThreadedRenderer::s_instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
ThreadedRenderer::ThreadedRenderer()
    : 
    m_device(nullptr),
    m_immediateContext(nullptr) 
{
}

/// <summary>
/// デストラクタ
/// </summary>
ThreadedRenderer::~ThreadedRenderer() 
{
    // スレッドプールの解放は自動的に行われる
}

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
ThreadedRenderer* ThreadedRenderer::GetInstance() 
{
    // 生成されていない場合
    if (s_instance == nullptr) 
    {
        // 初期生成
        s_instance = new ThreadedRenderer();
    }
	// 既存のインスタンスを返す
    return s_instance;
}

/// <summary>
/// システムの初期化
/// </summary>
/// <param name="device">DirectX 11 デバイス</param>
void ThreadedRenderer::Initialize(ID3D11Device* device) 
{
    // デバイスの保存
    m_device = device;

    // イミディエートコンテキストの取得
    device->GetImmediateContext(&m_immediateContext);

    // 最適なスレッド数を取得
    size_t threadCount = GetOptimalThreadCount();

    // スレッドプールの作成
    m_threadPool = std::make_unique<ThreadPool>(threadCount);

    // 遅延コンテキストをスレッド数分作成
    m_deferredContexts.resize(threadCount);
    for (size_t i = 0; i < threadCount; ++i) 
    {
        // 遅延コンテキストの作成
        HRESULT hr = device->CreateDeferredContext(0, &m_deferredContexts[i]);
        if (FAILED(hr)) 
        {
            MessageBoxA(nullptr, "遅延コンテキストの作成に失敗しました", "エラー", MB_OK | MB_ICONERROR);
            // エラー時の処理
            m_deferredContexts[i] = nullptr;
        }
    }
}

/// <summary>
/// レンダラブルオブジェクトの登録
/// </summary>
/// <param name="renderable">登録するIRenderableオブジェクト</param>
void ThreadedRenderer::RegisterRenderable(IRenderable* renderable) 
{
    if (!renderable) 
    {
        return;
    }

    // スレッドセーフな操作
    std::lock_guard<std::mutex> lock(m_renderablesMutex);

    // 既に登録されていないか確認
    auto it = std::find(m_renderables.begin(), m_renderables.end(), renderable);
    if (it == m_renderables.end()) 
    {
        // リストに追加
        m_renderables.push_back(renderable);
    }
}

/// <summary>
/// レンダラブルオブジェクトの登録解除
/// </summary>
/// <param name="renderable">登録解除するIRenderableオブジェクト</param>
void ThreadedRenderer::UnregisterRenderable(IRenderable* renderable) 
{
    if (!renderable) 
    {
        return;
    }

    // スレッドセーフな操作
    std::lock_guard<std::mutex> lock(m_renderablesMutex);

    // オブジェクトを検索して削除
    auto it = std::find(m_renderables.begin(), m_renderables.end(), renderable);
    if (it != m_renderables.end()) 
    {
        m_renderables.erase(it);
    }
}

/// <summary>
/// シーン全体の描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void ThreadedRenderer::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) 
{
    // パラメータチェック
    if (!m_device || !m_immediateContext || !m_threadPool) 
    {
        OutputDebugStringA("ThreadedRenderer: システムが初期化されていません\n");
        return;
    }

    // 有効な遅延コンテキストの数を確認
    size_t validContextCount = 0;
    for (const auto& ctx : m_deferredContexts) 
    {
        if (ctx != nullptr) 
        {
            validContextCount++;
        }
    }

    if (validContextCount == 0) 
    {
        OutputDebugStringA("ThreadedRenderer: 有効な遅延コンテキストがありません\n");
        return;
    }

    // レンダラブルオブジェクトのコピーを作成（スレッドセーフに）
    std::vector<IRenderable*> renderablesCopy;
    {
        std::lock_guard<std::mutex> lock(m_renderablesMutex);
        renderablesCopy = m_renderables;
    }

    // レンダリングジョブとコマンドリストを格納する配列
    std::vector<std::unique_ptr<RenderJob>> renderJobs;
    std::vector<ID3D11CommandList*> commandLists;

    // オブジェクト数が少ない場合は処理を最適化
    size_t objectCount = renderablesCopy.size();
    if (objectCount == 0) 
    {
        return; // 描画するものがない
    }

    // 遅延コンテキストを使用可能なコンテキストの数だけ使用
    size_t contextIndex = 0;
    size_t usedContextCount = std::min(objectCount, validContextCount);

    // レンダリングジョブの作成とタスクの登録
    for (size_t i = 0; i < objectCount; ++i) 
    {
        // 遅延コンテキストをラウンドロビン方式で割り当て
        contextIndex = i % usedContextCount;

        // 使用可能なコンテキストを取得
        auto deferredContext = m_deferredContexts[contextIndex].Get();
        if (!deferredContext) 
        {
            continue;
        }

        // レンダリングジョブの作成
        auto job = std::make_unique<RenderJob>(renderablesCopy[i], view, proj, deferredContext);

        // スレッドプールにタスクを追加
        m_threadPool->Enqueue([job = job.get()]() 
            {
            job->Execute();
            });

        // 後で使用するためにジョブを保持
        renderJobs.push_back(std::move(job));
    }

    // すべてのレンダリングタスクの完了を待機
    m_threadPool->WaitAll();

    // コマンドリストの収集
    for (const auto& job : renderJobs) 
    {
        ID3D11CommandList* cmdList = job->GetCommandList();
        if (cmdList) {
            commandLists.push_back(cmdList);
        }
    }

    // イミディエートコンテキストでコマンドリストを実行
    for (auto cmdList : commandLists) 
    {
        if (cmdList) 
        {
            m_immediateContext->ExecuteCommandList(cmdList, FALSE);
        }
    }
}

/// <summary>
/// システムに最適なスレッド数を取得
/// </summary>
/// <returns>最適なスレッド数</returns>
size_t ThreadedRenderer::GetOptimalThreadCount() const {
    // ハードウェアの並列処理能力を取得
    unsigned int threadCount = std::thread::hardware_concurrency();

    // ハードウェア情報が取得できない場合のフォールバック
    if (threadCount == 0) 
    {
        threadCount = 4; // デフォルト値
    }

    // メインスレッドを除いたスレッド数を使用（最低2スレッド）
    return std::max<size_t>(2, threadCount - 1);
}