#pragma once

#include "pch.h"

class SkyeSphere
{
public:
	// コンストラクタ
	SkyeSphere();
	// デストラクタ
	~SkyeSphere();

	// 更新処理
	void Update();

	// モデルを取得する
	void LoadSkyeSphereModel(ID3D11Device* device);

	// モデルの描画
	void DrawSkyeSphere(
		ID3D11DeviceContext1* context
		, DirectX::DX11::CommonStates* states
		, DirectX::SimpleMath::Matrix view
		, DirectX::SimpleMath::Matrix projection
	);


private:
	// SkyeSphereのモデル
	std::unique_ptr<DirectX::Model> m_skyeSphereModel;
};
