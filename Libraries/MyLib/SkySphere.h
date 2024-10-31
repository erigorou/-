#pragma once

#include "pch.h"

class SkySphere
{
public:
	// 固定値
	static const float SKYSPHERE_SCALE;

public:
	// コンストラクタ
	SkySphere();
	// デストラクタ
	~SkySphere();

	// 更新処理
	void Update();

	// モデルを取得する
	void LoadSkySphereModel(ID3D11Device* device);

	// モデルの描画
	void DrawSkySphere(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection
	);


private:
	// SkyeSphereのモデル
	std::unique_ptr<DirectX::Model> m_skySphereModel;
};
