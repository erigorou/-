#pragma once

#ifndef GAUGE_UI
#define GAUGE_UI

// プリコンパイルヘッダー
#include "pch.h"
#include "Interface/IUserInterface.h"


// ゲージクラス
class GaugeUI : public IUserInterface
{
public:
	// コンストラクタ
	GaugeUI();
	// デストラクタ
	~GaugeUI()	override;
	// 初期化関数
	void Initialize(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 size)override;
	// 更新処理
	void Update()	override;
	// 描画処理
	void Render()	override;
	// 終了関数
	void Finalize()	override;

	// 減少関数
	void MinusValue(float value)	{ m_value += value; }
	// 上昇関数
	void PlusValue(float value)		{ m_value -= value; }
	// ゲージ量を設定
	void SetMaxValue(float max)		{ m_value = max; }
	// ゲージ量の取得
	float GetValue()				{ return m_value; }

private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// ゲージの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;


	// 座標
	DirectX::SimpleMath::Vector2 m_position;
	// 大きさ
	DirectX::SimpleMath::Vector2 m_size;
	// ゲージの量
	float m_value;
};


#endif