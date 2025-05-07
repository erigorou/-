// --------------------------------------
//
// 名前: RenderTaskManager.cpp
// 概要: レンダリングタスクを管理するクラスの実装（完成版）
// 作成: 2025/05/07
//
// --------------------------------------

// インクルード
#include "pch.h"
#include "RenderTaskManager.h"
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Interface/IRenderable.h"

// 静的メンバ変数の定義
std::unique_ptr<RenderTaskManager> RenderTaskManager::m_RTM = nullptr;

/// <summary>
/// インスタンスを取得する
/// </summary>
RenderTaskManager* const RenderTaskManager::GetInstance()
{
    if (m_RTM == nullptr)
    {
        m_RTM.reset(new RenderTaskManager());
    }
    return m_RTM.get();
}

/// <summary>
/// コンストラクタ
/// </summary>
RenderTaskManager::RenderTaskManager()
    : m_isRunning(true)
{
    // スレッドプールの作成
    unsigned int threadCount = std::thread::hardware_concurrency();
    for (unsigned int i = 0; i < threadCount; ++i)
    {
        m_threads.emplace_back(&RenderTaskManager::WorkerThread, this);
    }
}

/// <summary>
/// デストラクタ
/// </summary>
RenderTaskManager::~RenderTaskManager()
{
    // スレッド終了フラグを設定
    m_isRunning = false;
    m_taskCV.notify_all();

    // 全てのスレッドが終了するまで待機
    for (auto& thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

/// <summary>
/// タスクを追加する
/// </summary>
void RenderTaskManager::AddTask(IRenderable* renderable, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
    std::lock_guard<std::mutex> lock(m_taskMutex);

    // タスクの追加
    m_taskQueue.push({ [=](ID3D11DeviceContext* context) {
        if (context)
        {
            renderable->RecordRenderCommands(view, proj, context);
        }
    } });

    // スレッドに通知
    m_taskCV.notify_one();
}

/// <summary>
/// タスクを実行するワーカースレッド
/// </summary>
void RenderTaskManager::WorkerThread()
{
    while (m_isRunning)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(m_taskMutex);

            // タスクが空の場合、待機
            m_taskCV.wait(lock, [this]() { return !m_isRunning || !m_taskQueue.empty(); });

            // 終了フラグが立っている場合はスレッドを終了
            if (!m_isRunning) return;

            // タスクを取り出す
            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
        }

        // ディファードコンテキストを取得して実行
        ID3D11DeviceContext* deferredContext = CommonResources::GetInstance()->GetDeferredContext();
        task.function(deferredContext);
    }
}

/// <summary>
/// 全てのタスクを実行し、即時コンテキストに統合
/// </summary>
void RenderTaskManager::ExecuteTasks(ID3D11DeviceContext* immediateContext)
{
    if (!immediateContext) return;

    std::lock_guard<std::mutex> lock(m_taskMutex);

    // タスクが全て実行されるまで待機
    while (!m_taskQueue.empty())
    {
        Task task = std::move(m_taskQueue.front());
        m_taskQueue.pop();
        task.function(immediateContext);
    }
}
