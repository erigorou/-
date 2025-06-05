// ----------------------------------------------------------------
//
// ファイル：ThreadedRenderer.h
// 機能：マルチスレッドレンダリングシステムのメインクラス
// 作成：2025/05/08
//
// ----------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Libraries/MyLib/ThreadPool/ThreadPool.h"
#include "Libraries/MyLib/RenderJob/RenderJob.h"
#include "Interface/IRenderable.h"
#include <vector>
#include <memory>
#include <mutex>
#include <wrl/client.h>

/// <summary>
/// マルチスレッドレンダラークラス - レンダリングプロセス全体を管理
/// </summary>
class ThreadedRenderer 
{
    // -----------------------
    // アクセサ
    // -----------------------
public:
    // シングルトンインスタンスの取得
    static ThreadedRenderer* GetInstance();

    // スクリーンショットを取る
	void TakeScreenshot();

	// -----------------------
	// メンバ関数（公開）
	// -----------------------
public:
    // システムの初期化
    void Initialize(ID3D11Device* device);

    // レンダラブルオブジェクトの登録
    void RegisterRenderable(IRenderable* renderable);

    // レンダラブルオブジェクトの登録解除
    void UnregisterRenderable(IRenderable* renderable);

    // シーン全体の描画処理
    void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

    // --------------------
	// メンバ変数（非公開）
	// --------------------
private:
    /// コンストラクタ - プライベート（シングルトンパターン）
    ThreadedRenderer();

    /// デストラクタ
    ~ThreadedRenderer();

    // システムに最適なスレッド数を取得
    size_t GetOptimalThreadCount() const;

    
	// ------------------------
	// メンバ変数（非公開）
	// ------------------------
private:
    // シングルトンインスタンス
    static ThreadedRenderer* s_instance;

    // スレッドプール
    std::unique_ptr<ThreadPool> m_threadPool;
    // レンダラブルオブジェクトのリスト
    std::vector<IRenderable*> m_renderables;
    // レンダラブルリスト用ミューテックス
    std::mutex m_renderablesMutex;
    // DirectXデバイス
    ID3D11Device* m_device;
    // イミディエートコンテキスト
    ID3D11DeviceContext* m_immediateContext;

    // リザルトに乗せるスクショ撮影のフラグ
	bool m_takeScreenshot;
};