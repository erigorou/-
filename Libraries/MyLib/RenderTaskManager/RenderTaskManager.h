// --------------------------------------
//
// 名前: RenderTaskManager.h
// 概要: レンダリングタスクを管理するクラス（完成版）
// 作成: 2025/05/07
//
// --------------------------------------

// インクルード
#pragma once
#include "pch.h"
#include <thread>
#include <mutex>
#include <condition_variable> 
#include <functional>
#include <queue>
#include "Interface/IRenderable.h"
#include "Game/CommonResources.h"

/// <summary>
/// レンダータスクマネージャー（スレッドプール実装）
/// </summary>
class RenderTaskManager
{
private:
    using RenderFunction = std::function<void(ID3D11DeviceContext*)>;

    // タスク構造体
    struct Task
    {
        RenderFunction function;
    };

public:
    // インスタンスを取得する
    static RenderTaskManager* const GetInstance();

    // コンストラクタとデストラクタ
    RenderTaskManager();
    ~RenderTaskManager();

    // タスクの追加
    void AddTask(IRenderable* renderable, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
    // タスクの実行
    void ExecuteTasks(ID3D11DeviceContext* immediateContext);

private:
    // タスクを実行するスレッド
    void WorkerThread();

    // メンバ変数
    static std::unique_ptr<RenderTaskManager> m_RTM;
    std::vector<std::thread> m_threads;
    std::queue<Task> m_taskQueue;
    std::mutex m_taskMutex;
    std::condition_variable m_taskCV;
    std::atomic<bool> m_isRunning;
};