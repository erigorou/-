// ----------------------------------------------------------------
//
// ファイル：ThreadedRenderer.cpp
// 機能：マルチスレッドレンダリングシステムのメインクラス
// 作成：2025/05/08
//
// ----------------------------------------------------------------

#include "pch.h"
#include "ThreadedRenderer.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

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
    if (!renderable) return;

    // スレッドセーフな操作
    std::lock_guard<std::mutex> lock(m_renderablesMutex);

    // 既に登録されていないか確認
    auto it = std::find(m_renderables.begin(), m_renderables.end(), renderable);
    if (it == m_renderables.end()) 
    {
        // リストに追加
        m_renderables.emplace_back(renderable);
    }
}

/// <summary>
/// レンダラブルオブジェクトの登録解除
/// </summary>
/// <param name="renderable">登録解除するIRenderableオブジェクト</param>
void ThreadedRenderer::UnregisterRenderable(IRenderable* renderable) 
{
    if (!renderable) return;

    // 遅延コンテキストリセット
    for (auto& ctx : m_deferredContexts)
    {
        if (ctx)
        {
            // すべてのステートをクリア
            ctx->ClearState();
        }
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
        if (ctx)
        {
            // 共通のリソースを取得
            auto resources = CommonResources::GetInstance();
            auto rtv = resources->GetDeviceResources()->GetRenderTargetView();
            auto dsv = resources->GetDeviceResources()->GetDepthStencilView();
            auto viewport = resources->GetDeviceResources()->GetScreenViewport();

            // 各遅延コンテキストでレンダリングターゲットとビューポートを設定
            ctx->OMSetRenderTargets(1, &rtv, dsv);
            ctx->RSSetViewports(1, &viewport);

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

    size_t objectCount = renderablesCopy.size();
    if (objectCount == 0)
    {
        return; // 描画するものがない
    }

    // 使用する遅延コンテキスト数は描画対象数と有効コンテキスト数の小さい方
    size_t usedContextCount = std::min(objectCount, validContextCount);

    // ----------------------------------------------------------------
    // ここからレイヤーごとに描画オブジェクトをグルーピングし、
    // 各レイヤー単位でコンテキストにラウンドロビン割り振り
    // ----------------------------------------------------------------

    // レイヤーごとに描画オブジェクトをグルーピング
    std::map<UINT, std::vector<IRenderable*>> layerGroups;
    for (auto* r : renderablesCopy) {
        layerGroups[static_cast<UINT>(r->GetLayer())].push_back(r);
    }

    // 各レイヤー単位でコンテキストに割り振る（ラウンドロビン）
    std::vector<std::vector<IRenderable*>> renderableGroups(usedContextCount);
    size_t contextIdx = 0;
    for (auto& [layer, group] : layerGroups) {
        for (auto* r : group) {
            renderableGroups[contextIdx].push_back(r);
            contextIdx = (contextIdx + 1) % usedContextCount;
        }
    }

    // ----------------------------------------------------------------
    // 各コンテキストのレンダリング処理をスレッドプールで並列実行
    // ----------------------------------------------------------------

    // コマンドリスト格納用配列（各コンテキスト分）
    std::vector<ID3D11CommandList*> commandLists(usedContextCount, nullptr);

    for (size_t i = 0; i < usedContextCount; ++i)
    {
        ID3D11DeviceContext* deferredContext = m_deferredContexts[i].Get();
        auto& renderables = renderableGroups[i];

        if (!deferredContext || renderables.empty()) continue;

        m_threadPool->Enqueue([&, i, deferredContext, renderables, view, proj]() {
            // 各IRenderableのRenderJobを実行
            for (auto* renderable : renderables)
            {
                RenderJob job(renderable, view, proj, deferredContext);
                job.Execute();
            }

            // コマンドリスト生成
            ID3D11CommandList* cmdList = nullptr;
            HRESULT hr = deferredContext->FinishCommandList(FALSE, &cmdList);
            if (SUCCEEDED(hr) && cmdList)
            {
                commandLists[i] = cmdList;
            }
            else
            {
                OutputDebugStringA("ThreadedRenderer: コマンドリスト作成に失敗\n");
            }
            });
    }

    // 全スレッドの完了待ち
    m_threadPool->WaitAll();

    // イミディエートコンテキストでコマンドリストを実行
    for (auto* cmdList : commandLists)
    {
        if (cmdList)
        {
            m_immediateContext->ExecuteCommandList(cmdList, FALSE);
            cmdList->Release();
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
        threadCount = 2; // デフォルト値
    }

    // メインスレッドを除いたスレッド数を使用（最低2スレッド）
    return std::max<size_t>(2, threadCount - 1);
}