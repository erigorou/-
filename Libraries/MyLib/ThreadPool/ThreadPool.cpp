// ----------------------------------------------------------------
//
// ファイル：ThreadPool.cpp
// 機能：マルチスレッド処理を効率的に管理するスレッドプール
// 作成：2025/05/08
//
// ----------------------------------------------------------------

#include "pch.h"
#include "ThreadPool.h"

/// <summary>
/// スレッドプールのコンストラクタ
/// </summary>
/// <param name="numThreads">作成するスレッドの数</param>
ThreadPool::ThreadPool(size_t numThreads) : m_stop(false), m_activeCount(0) 
{
    // 指定された数のワーカースレッドを作成
    for (size_t i = 0; i < numThreads; ++i) 
    {
        m_workers.emplace_back([this]() 
            {
            workerThread();
            }
        );
    }
}

// ワーカースレッドのメイン処理を分離
void ThreadPool::workerThread() 
{
    while (true) 
    {
        std::function<void()> task = GetNextTask();

        // スレッドプールが停止し、タスクがなければ終了
        if (!task) 
        {
            return;
        }

        // タスクを実行
        task();

        // タスク完了処理
        DecrementActiveTaskCount();
    }
}

// タスクキューからタスクを取得
std::function<void()> ThreadPool::GetNextTask() 
{
    std::unique_lock<std::mutex> lock(m_queueMutex);

    // タスクが来るか停止するまで待機
    m_condition.wait(lock, [this]() 
        {
        return m_stop || !m_tasks.empty();
        }
    );

    // 終了条件の確認
    if (m_stop && m_tasks.empty()) 
    {
        return nullptr;
    }

    // タスクを取得してキューから削除
    std::function<void()> task = std::move(m_tasks.front());
    m_tasks.pop();
    return task;
}

// アクティブタスク数を減らし、必要なら完了通知
void ThreadPool::DecrementActiveTaskCount() 
{
    std::unique_lock<std::mutex> lock(m_completionMutex);
    if (--m_activeCount == 0) 
    {
        m_completionCondition.notify_all();
    }
}

/// <summary>
/// スレッドプールのデストラクタ - すべてのスレッドを安全に終了
/// </summary>
ThreadPool::~ThreadPool() 
{
    // スレッドプールの停止を指示
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }

    // 待機中のすべてのスレッドに通知
    m_condition.notify_all();

    // すべてのスレッドの終了を待機
    for (std::thread& worker : m_workers) 
    {
        if (worker.joinable()) 
        {
            worker.join();
        }
    }
}

/// <summary>
/// すべてのタスクの完了を待機
/// </summary>
void ThreadPool::WaitAll() 
{
    std::unique_lock<std::mutex> lock(m_completionMutex);
    m_completionCondition.wait(lock, [this]() 
        {
        return m_activeCount == 0;
        }
    );
}