// ----------------------------------------------------------------
//
// 名前：RenderTaskManager.cpp
// 機能：レンダリングタスクを管理するクラスの実装
// 作成：2025/05/07
//
// ----------------------------------------------------------------

// インクルード
#include "pch.h"
#include "RenderTaskManager.h"
#include "Game/CommonResources.h"

// 静的メンバ変数の定義
std::unique_ptr<RenderTaskManager> RenderTaskManager::m_RTM = nullptr;

// ---------------------------------------------------------
// インスタンスを取得する
// ---------------------------------------------------------
/// <summary>
/// シングルトンのインスタンスを取得する
/// </summary>
/// <returns>RenderTaskManagerクラスのインスタンス</returns>
RenderTaskManager* const RenderTaskManager::GetInstance()
{
    if (m_RTM == nullptr)
    {
        m_RTM.reset(new RenderTaskManager());
    }
    return m_RTM.get();
}

// ---------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------
/// <summary>
/// レンダリングタスクマネージャーを初期化する
/// </summary>
RenderTaskManager::RenderTaskManager()
    :
    m_threadPool(std::thread::hardware_concurrency())
{
}

// ---------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------
/// <summary>
/// レンダリングタスクマネージャーを終了する
/// </summary>
RenderTaskManager::~RenderTaskManager()
{
    m_threadPool.WaitAll();
}

// ---------------------------------------------------------
// タスクを追加する
// ---------------------------------------------------------
/// <summary>
/// レンダリングタスクを追加する
/// </summary>
/// <param name="renderable">描画対象オブジェクト</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void RenderTaskManager::AddTask(IRenderable* renderable, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
    if (!renderable)
    {
        OutputDebugStringA("エラー: 追加されたレンダリングオブジェクトがnullです\n");
        return;
    }

    // スレッドプールに描画コマンド記録タスクを登録
    m_threadPool.Enqueue([renderable, view, proj]() {
        // スレッドごとに異なるコンテキストを取得
        ID3D11DeviceContext* context = CommonResources::GetInstance()->GetDeferredContext();
        if (context)
        {
            // 描画コマンドを記録
            try
            {
                renderable->RecordRenderCommands(view, proj, context);
            }
            catch (const std::exception& e)
            {
                std::string errorMsg = "レンダリングコマンド記録中にエラー: ";
                errorMsg += e.what();
                OutputDebugStringA(errorMsg.c_str());
            }
        }
        else
        {
            OutputDebugStringA("エラー: ディファードコンテキストがnullです\n");
        }
        });
}

// ---------------------------------------------------------
// 全てのタスクを実行し、即時コンテキストに統合する
// ---------------------------------------------------------
/// <summary>
/// 全てのディファードコンテキストのコマンドリストを即時コンテキストで実行する
/// </summary>
/// <param name="immediateContext">即時コンテキスト</param>
/// <param name="deferredContexts">ディファードコンテキスト一覧</param>
void RenderTaskManager::ExecuteTasks(ID3D11DeviceContext* immediateContext, const std::vector<ID3D11DeviceContext*>& deferredContexts)
{
    // ThreadPoolのすべてのタスクが完了するのを待つ
    m_threadPool.WaitAll();

    // nullチェック
    if (!immediateContext)
    {
        OutputDebugStringA("エラー: 即時コンテキストがnullです\n");
        return;
    }

    // deferredContextsの各コンテキストからコマンドリストを生成して実行
    for (auto context : deferredContexts)
    {
        if (!context)
        {
            OutputDebugStringA("警告: ディファードコンテキストがnullです\n");
            continue;
        }

        // コマンドリストを生成
        Microsoft::WRL::ComPtr<ID3D11CommandList> commandList;
        HRESULT hr = context->FinishCommandList(FALSE, &commandList);

        if (SUCCEEDED(hr) && commandList)
        {
            // 即時コンテキストでコマンドリストを実行
            immediateContext->ExecuteCommandList(commandList.Get(), FALSE);
        }
        else
        {
            OutputDebugStringA("エラー: コマンドリストの生成に失敗しました\n");
        }
    }
}

// ---------------------------------------------------------
// スレッドプールを取得する
// ---------------------------------------------------------
/// <summary>
/// スレッドプールを取得する
/// </summary>
/// <returns>スレッドプール</returns>
ThreadPool* RenderTaskManager::GetThreadPool()
{
    return &m_threadPool;
}