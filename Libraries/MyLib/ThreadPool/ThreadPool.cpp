// ----------------------------------------------------------------
//
// 名前：ThreadPool.cpp
// 機能：マルチスレッドレンダリング用のスレッドプール
// 作成：2025/05/07
//
// ----------------------------------------------------------------

// インクルード
#include "pch.h"
#include "ThreadPool.h"

// ---------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------
/// <summary>
/// 指定されたスレッド数でスレッドプールを初期化する
/// </summary>
/// <param name="threadCount">スレッドの数</param>
ThreadPool::ThreadPool(unsigned int threadCount)
    :
    m_isRunning(true)
{
    // スレッド数が0または過剰に大きい場合は調整
    if (threadCount == 0 || threadCount > 64)
    {
        threadCount = std::thread::hardware_concurrency();
    }

    try
    {
        for (unsigned int i = 0; i < threadCount; ++i)
        {
            m_threads.emplace_back(&ThreadPool::WorkerThread, this);
        }
    }
    catch (const std::exception& e)
    {
        // スレッド作成に失敗した場合のエラー処理
        m_isRunning = false;
        m_cv.notify_all();

        // 作成済みのスレッドを終了
        for (auto& thread : m_threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        throw std::runtime_error(std::string("スレッドプールの初期化に失敗: ") + e.what());
    }
}

// ---------------------------------------------------------
// デストラクタ
// ---------------------------------------------------------
/// <summary>
/// スレッドプールを終了し、全てのスレッドを停止する
/// </summary>
ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isRunning = false;
    }
    m_cv.notify_all();

    for (auto& thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

// ---------------------------------------------------------
// 全てのタスクが完了するまで待機する
// ---------------------------------------------------------
/// <summary>
/// 全てのタスクが完了するまで待機する
/// </summary>
void ThreadPool::WaitAll()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this]() { return m_tasks.empty(); });
}

// ---------------------------------------------------------
// スレッド数を取得する
// ---------------------------------------------------------
/// <summary>
/// スレッドプール内のスレッド数を取得する
/// </summary>
/// <returns>スレッドの数</returns>
unsigned int ThreadPool::GetThreadCount() const
{
    return static_cast<unsigned int>(m_threads.size());
}

// ---------------------------------------------------------
// ワーカースレッド
// ---------------------------------------------------------
/// <summary>
/// タスクキューからタスクを取得して実行するワーカースレッド
/// </summary>
void ThreadPool::WorkerThread()
{
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            // タスクが空で、プールが稼働中であれば待機する
            m_cv.wait(lock, [this]() { return !m_isRunning || !m_tasks.empty(); });

            // スレッドプールが停止し、タスクが空ならスレッド終了
            if (!m_isRunning && m_tasks.empty())
            {
                return;
            }

            // タスクを取得
            task = std::move(m_tasks.front());
            m_tasks.pop();
        }

        // タスクを実行（例外処理を追加して安全性を向上）
        try
        {
            task();
        }
        catch (const std::exception& e)
        {
            // タスク実行中の例外をキャッチして、ログを出力
            std::string errorMsg = "スレッドプールタスク実行中にエラー発生: ";
            errorMsg += e.what();
            OutputDebugStringA(errorMsg.c_str());
        }

        // 全てのタスクが完了していれば通知
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_tasks.empty())
            {
                m_cv.notify_all();
            }
        }
    }
}