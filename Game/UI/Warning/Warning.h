// ------------------------------------------------------------------
// 
// 名前:	Warning.cpp
// 機能:	HPが一定値以下になった時に警告を表示するクラス
// 製作:	池田桜輔
// 
// ------------------------------------------------------------------
#ifndef WARNING_DEFINED
#define WARNING_DEFINED
#pragma once
// インクルード
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

class HPSystem;

class Warning
{
	// 固定値
private:
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/WarningVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/WarningPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/WarningGS.cso";

	static constexpr int LOW_HP = 2;

	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	static constexpr float FADE_TIME = 1.2f;

	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	diffuse;
		DirectX::SimpleMath::Vector4	easing;
		DirectX::SimpleMath::Vector4	time;
	};

	// パブリック関数
public:
	Warning(HPSystem* hp);
	~Warning();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	// プライベート変数
private:

	HPSystem* m_hp;

	std::unique_ptr<CustomShader>			m_customShader;		// シェーダー

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;			// コンスタントバッファ
	std::unique_ptr<DirectX::CommonStates>	m_states;			// ステート

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;	// プリミティブバッチ

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// テクスチャ

	float m_elapsedTime;	// フレーム時間
	float m_totalTime;		// 経過時間
};

#endif // WARNING_DEFINED
