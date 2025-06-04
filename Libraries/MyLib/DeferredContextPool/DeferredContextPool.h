// ----------------------------------------------------------------
//
// ファイル：DeferredContextPool.h
// 機能：遅延コンテキストのプールクラス
// 作成：2025/05/08
//
// ----------------------------------------------------------------
#pragma once

// インクルード
#include "pch.h"

// 前方宣言
class IRenderable;

/// <summary>
/// 遅延コンテキストのプールクラス
/// </summary>
class DeferredContextPool
{
	// -------------------
	// 固定値
	// -------------------
private:
	// 最大コンテキスト数
	static const int MAX_CONTEXTS = 1000;


	// --------------------
	// アクセサ
	// --------------------
public:
	// シングルトンインスタンスの取得
	static DeferredContextPool* GetInstance();

	// IRenderableとID3D11DeviceContextを紐づけする
	void RegisterRenderable(const IRenderable* object);

	// コンテキストを取得する
	ID3D11DeviceContext* GetDeferredContext(const IRenderable* object);

	// 全ての遅延コンテキストを取得する
	std::vector<Microsoft::WRL::ComPtr<ID3D11DeviceContext>> GetAllDeferredContexts() const;

	// コンテキストを解放する
	void ReleaseDeferredContext(const IRenderable* object);

	// 使用中の遅延コンテキストを全てリセットする
	void ResetAllContexts();

	// -------------------
	// メンバ関数（公開）
	// -------------------
public:
	// コンストラクタ
	DeferredContextPool();

	// デストラクタ
	~DeferredContextPool();

	// 全リセット
	void Reset();

	// -------------------
	// メンバ関数（非公開）
	// -------------------
private:
	// コンテキスト群の作成
	void CreateDeferredContexts();

	// 全削除
	void Clear();

	// ------------------
	// メンバ変数
	// ------------------
private:

	// シングルトン
	static DeferredContextPool* s_instance;

	// 未使用のコンテキストのリスト
	std::vector<Microsoft::WRL::ComPtr<ID3D11DeviceContext>> m_unusedContexts;

	// コンテキストのリスト
	std::unordered_map<const IRenderable*, Microsoft::WRL::ComPtr<ID3D11DeviceContext>> m_contextMap;
};