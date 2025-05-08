// ----------------------------------------------------------------
//
// 名前：RenderTaskManager.h
// 機能：レンダリングタスクを管理するクラス
// 作成：2025/05/07
//
// ----------------------------------------------------------------

// インクルード
#pragma once
#include "pch.h"
#include "Libraries/MyLib/ThreadPool/ThreadPool.h"
#include "Interface/IRenderable.h"
#include <DirectXMath.h>
#include <queue>
#include <mutex>
#include <functional>

/// <summary>
/// レンダリングタスクを管理するクラス
/// </summary>
class RenderTaskManager
{
    // ------------------
    // メンバ関数（公開）
    // ------------------
public:
    // インスタンスを取得する
    static RenderTaskManager* const GetInstance();

    // コンストラクタ
    RenderTaskManager();

    // デストラクタ
    ~RenderTaskManager();

    // タスクを追加する
    void AddTask(IRenderable* renderable, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

    // 全てのタスクを実行し、即時コンテキストに結合する
    void ExecuteTasks(ID3D11DeviceContext* immediateContext, const std::vector<ID3D11DeviceContext*>& deferredContexts);

    // スレッドプールを取得する
    ThreadPool* GetThreadPool();

    // ------------------
    // メンバ変数
    // ------------------
private:
    // シングルトンインスタンス
    static std::unique_ptr<RenderTaskManager> m_RTM;
    // スレッドプール
    ThreadPool m_threadPool;
    // タスクキュー
    std::queue<std::function<void(ID3D11DeviceContext*)>> m_taskQueue;
    // タスクキューのミューテックス
    std::mutex m_taskMutex;
    // タスク待機用の条件変数
    std::condition_variable m_taskCV;
};