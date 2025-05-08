#pragma once
#include "pch.h"
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

/// <summary>
/// マルチスレッドレンダリング用のスレッドプールクラス
/// </summary>
class ThreadPool
{
    // -----------------
    // メンバ関数（公開）
    // -----------------
public:
    // コンストラクタ
    explicit ThreadPool(unsigned int threadCount = std::thread::hardware_concurrency());

    // デストラクタ
    ~ThreadPool();

    // タスクを追加する
    template <typename F, typename... Args>
    auto Enqueue(F&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        using ReturnType = decltype(func(args...));

        // タスクをラッピングしてfutureで返す
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...)
        );
        std::future<ReturnType> result = task->get_future();

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_isRunning)
            {
                throw std::runtime_error("スレッドプールは既に停止しています");
            }
            m_tasks.emplace([task]() { (*task)(); });
        }

        m_cv.notify_one();
        return result;
    }

    // 全てのタスクが完了するまで待機する
    void WaitAll();

    // スレッド数を取得する
    unsigned int GetThreadCount() const;

    // -----------------
    // メンバ関数（非公開）
    // -----------------
private:
    // ワーカースレッド
    void WorkerThread();

    // -----------------
    // メンバ変数
    // -----------------
private:
    // スレッドプール
    std::vector<std::thread> m_threads;
    // タスクキュー
    std::queue<std::function<void()>> m_tasks;
    // タスクキューアクセス用ミューテックス
    std::mutex m_mutex;
    // タスクキューの待機条件変数
    std::condition_variable m_cv;
    // スレッド実行フラグ
    bool m_isRunning;
};