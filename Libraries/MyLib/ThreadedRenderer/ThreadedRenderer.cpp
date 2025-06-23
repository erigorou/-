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
#include "../DeferredContextPool/DeferredContextPool.h"

#include "Game/Messenger/EventMessenger.h"
#include "Libraries/MyLib/ScreenShot.h"


// シングルトンインスタンス
ThreadedRenderer* ThreadedRenderer::s_instance = nullptr;


/// <summary>
/// コンストラクタ
/// </summary>
ThreadedRenderer::ThreadedRenderer()
    : 
    m_device(nullptr),
    m_immediateContext(nullptr),
    m_threadPool(nullptr)
{
	// 遅延コンテキストプールの初期化
    DeferredContextPool::GetInstance();
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

        // レンダラブルオブジェクトを遅延コンテキストプールに登録
		DeferredContextPool::GetInstance()->RegisterRenderable(renderable);
    }

    std::string msg = "マルチスレッドに登録されているアイテムの数: " + std::to_string(DeferredContextPool::GetInstance()->GetAllDeferredContexts().size()) + "\n";
    OutputDebugStringA(msg.c_str());
}


/// <summary>
/// レンダラブルオブジェクトの登録解除
/// </summary>
/// <param name="renderable">登録解除するIRenderableオブジェクト</param>
void ThreadedRenderer::UnregisterRenderable(IRenderable* renderable) 
{
    if (!renderable) return;

    // 遅延コンテキストリセット
	DeferredContextPool::GetInstance()->ResetAllContexts();

    // スレッドセーフな操作
    std::lock_guard<std::mutex> lock(m_renderablesMutex);

    // オブジェクトを検索して削除
    auto it = std::find(m_renderables.begin(), m_renderables.end(), renderable);
    if (it != m_renderables.end()) 
    {
        m_renderables.erase(it);

		// レンダラブルオブジェクトを遅延コンテキストプールから登録解除
		DeferredContextPool::GetInstance()->ReleaseDeferredContext(renderable);
	}

    std::string msg = "マルチスレッドに登録されているアイテムの数: " + std::to_string(DeferredContextPool::GetInstance()->GetAllDeferredContexts().size()) + "\n";
    OutputDebugStringA(msg.c_str());
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

	// 使用中の遅延コンテキストの取得
	auto deferredContexts = DeferredContextPool::GetInstance()->GetAllDeferredContexts();

    // 一括設定
    for (const auto& ctx : deferredContexts)
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
        }
    }

    // レンダラブルオブジェクトのコピーを作成（スレッドセーフに）
    std::vector<IRenderable*> renderablesCopy;
    {
        std::lock_guard<std::mutex> lock(m_renderablesMutex);
        renderablesCopy = m_renderables;
    }

    // レンダラブルオブジェクトをレイヤーでソート
    std::sort(renderablesCopy.begin(), renderablesCopy.end(),
        [](IRenderable* a, IRenderable* b)
        {
            UINT la = static_cast<UINT>(a->GetLayer());
            UINT lb = static_cast<UINT>(b->GetLayer());
            return (la == lb) ? (a < b) : (la < lb);
        }
    );

    // レンダリングジョブとコマンドリストを格納する配列
    std::vector<std::unique_ptr<RenderJob>> renderJobs;
    std::vector<ID3D11CommandList*> commandLists;

    // オブジェクト数が少ない場合は処理を最適化
    size_t objectCount = renderablesCopy.size();
    if (objectCount == 0)
    {
        return; // 描画するものがない
    }

    // 遅延コンテキストのインデックス
	size_t contextIndex = 0;

	// 遅延コンテキストを入れておく変数
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferredContext;

    // レンダリングジョブの作成とタスクの登録
	for (size_t i = 0; i < deferredContexts.size(); ++i)
    {
        // 遅延コンテキストをラウドロビン方式で割り当て
        contextIndex = i % deferredContexts.size() - 1;

        // 連結してある遅延コンテキストを取得する
        deferredContext = DeferredContextPool::GetInstance()->GetDeferredContext(renderablesCopy[i]);

        // 共通のリソースを取得
        auto resources = CommonResources::GetInstance();
        auto rtv = resources->GetDeviceResources()->GetRenderTargetView();
        auto dsv = resources->GetDeviceResources()->GetDepthStencilView();
        auto viewport = resources->GetDeviceResources()->GetScreenViewport();

        // 重要: 各遅延コンテキストでレンダリングターゲットとビューポートを設定
        deferredContext->OMSetRenderTargets(1, &rtv, dsv);
        deferredContext->RSSetViewports(1, &viewport);

        // レンダリングジョブの作成
        auto job = std::make_unique<RenderJob>(renderablesCopy[i], view, proj, deferredContext.Get());

        // スレッドプールにタスクを追加
        m_threadPool->Enqueue([job = job.get()]()
            {
                job->Execute();
            });

        // 後で使用するためにジョブを保持
        renderJobs.emplace_back(std::move(job));
	}

    // 全スレッドの完了待ち
    m_threadPool->WaitAll();

	// コマンドリストを作成
	for (const auto& job : renderJobs)
    {
        if (job && job->GetCommandList())
        {
			// コマンドリストを取得して保存
			ID3D11CommandList* cmdList = nullptr;
			job->GetCommandList()->QueryInterface(IID_PPV_ARGS(&cmdList));
			commandLists.push_back(cmdList);
		}
	}


    // イミディエートコンテキストでコマンドリストを実行
    for (auto* cmdList : commandLists)
    {
        if (cmdList)
        {
            m_immediateContext->ExecuteCommandList(cmdList, FALSE);
            cmdList->Release();
        }
    }

	// 遅延コンテキストのリセット
	DeferredContextPool::GetInstance()->ResetAllContexts();

    // スクショを撮影
	ScreenShot::TakeScreenshot();
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