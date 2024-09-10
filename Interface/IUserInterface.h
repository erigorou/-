// -----------------------------------------------
//
// ゲージ系UIのインターフェースクラス
//
// -----------------------------------------------

#pragma once

#ifndef IGaugeUI_DEFINED
#define IGaugeUI_DEFINED

#include "pch.h"

class IUserInterface
{
public:

	// デストラクタ
	virtual ~IUserInterface() = default;
	// 初期化関数
	virtual void Initialize(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 size) = 0;
	// 更新関数
	virtual void Update() = 0;
	// 描画関数5
	virtual void Render() = 0;
	// 終了関数
	virtual void Finalize() = 0;

	// 減少関数
	virtual void MinusValue(float value) = 0;
	// 上昇関数
	virtual void PlusValue(float value) = 0;

	// ゲージの量を設定
	virtual void SetValue(float max) = 0;
	// ゲージの量を取得
	virtual void GetValue(float value) = 0;

}; 

#endif
