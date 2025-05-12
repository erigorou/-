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
		ID3D11DeviceContext* defferedConetxt) = 0;
};