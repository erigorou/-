// ----------------------------------------------------------------
//
// 名前：IRenderable.h
// 機能：マルチスレッドレンダリングを実現するためのインターフェース
// 作成：2025/05/01
//
// ----------------------------------------------------------------

// インクルード
#pragma once
#include "pch.h"

/// <summary>
/// マルチスレッドレンダリングを実現するためのインターフェースクラス
/// </summary>
class IRenderable
{
// -------------------
// レイヤー
// -------------------
public:
	// レイヤーの定義
	enum class Layer : UINT
	{
		SkySphere = 0,
		Sea,
		Wall,
		Floor,
		Object,
		Fade,
		UI,
	};

// ------------------
// メンバ関数（公開）
// ------------------
public:
	// デストラクタ
	virtual ~IRenderable() = default;
	// 描画処理
	virtual void RecordRenderCommands(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		ID3D11DeviceContext* deferredContext) = 0;
	// 表示レイヤーを取得する
	virtual Layer GetLayer() const = 0;
};