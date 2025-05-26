// ----------------------------------------------------------------
//
// ファイル：ThreadPool.h
// 機能：マルチスレッド処理を効率的に管理するスレッドプール
// 作成：2025/05/08
//
// ----------------------------------------------------------------

#pragma once
#include "pch.h"
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <atomic>

/// <summary>
/// スレッドプールクラス - マルチスレッド処理を効率的に管理
/// </summary>
class ThreadPool 
{

	// -----------------------
	// メンバ関数（公開）
	// -----------------------
public:
    // コンストラクタ - 指定された数のスレッドでプールを初期化
    ThreadPool(size_t numThreads);
    // threadを動かす
    void workerThread();
    // タスクの取得
    std::function<void()> GetNextTask();
    // 完了したタスクを確認
    void DecrementActiveTaskCount();
    // デストラクタ - すべてのスレッドを安全に終了
    ~ThreadPool();
    // タスクをキューに追加するテンプレート関数
    template<class F>
    void Enqueue(F&& f);
    // すべてのタスクの完了を待機
    void WaitAll();


    // -----------------------
    // メンバ変数
    // -----------------------
private:
    // ワーカースレッドのコレクション
    std::vector<std::thread> m_workers;
    // タスクキュー
    std::queue<std::function<void()>> m_tasks;
    // タスクキュー操作のためのミューテックス
    std::mutex m_queueMutex;
    // スレッド同期用の条件変数
    std::condition_variable m_condition;
    // 実行中のタスク数カウンター
    std::atomic<int> m_activeCount;
    // タスク完了通知用の条件変数
    std::condition_variable m_completionCondition;
    // 完了通知用のミューテックス
    std::mutex m_completionMutex;
    // スレッドプール停止フラグ
    bool m_stop;
};


/// <summary>
/// タスクをスレッドプールに追加するテンプレート関数
/// </summary>
/// <typeparam name="F">テンプレートfunction</typeparam>
/// <param name="f">テンプレートfunction</param>
template<class F>
void ThreadPool::Enqueue(F&& f) 
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_tasks.emplace(std::forward<F>(f));
        m_activeCount++;
    }
    m_condition.notify_one();
}