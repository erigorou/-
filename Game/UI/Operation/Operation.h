#pragma once
#ifndef OPERATION_DEFINED
#define OPERATION_DEFINED

#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "OperateUI.h"
#include "../UI.h"

class Operation
{
	// 固定値
private:
	static const wchar_t* TEXT_PATH;

	// 変更後:
	static constexpr std::string_view keys[] = {
		"X_KEY", 
		"SHIFT_KEY",
		"UP_KEY",
		"DOWN_KEY",
		"LEFT_KEY",
		"RIGHT_KEY"
	};

	// HPの最低値
	static constexpr int LOW_HP = 2;
	// フェード時間
	static constexpr float FADE_TIME = 1.2f;

	// インプットレイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 定数バッファ
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
	// コンストラクタ
	Operation();
	// デストラクタ
	~Operation();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();

	// プライベート変数
private:

	// シェーダー
	std::unique_ptr<CustomShader> m_customShader;
	// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// ステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// テクスチャ
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// フレーム時間
	float m_elapsedTime;
	// 経過時間
	float m_totalTime;

	// 操作UI
	std::vector<std::unique_ptr<OperateUI>> m_operateUIs;
	// UI
	std::unique_ptr<UI> m_textUI;
};

#endif // Operation_DEFINED
