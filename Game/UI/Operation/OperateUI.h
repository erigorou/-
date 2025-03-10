// --------------------------------------------
// 名前:	OperateUI.cpp
// 機能:	操作説明UIのクラス
// 製作:	池田桜輔
// --------------------------------------------
#pragma once
// インクルード
#include "pch.h"
#include "Interface/IObserver.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"


class OperateUI : public IObserver, public IObject
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// シェーダーファイルのパス
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/OperationVS.cso";
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/OperationPS.cso";
	static constexpr wchar_t GS_PATH[] = L"Resources/cso/OperationGS.cso";

	// フェード時間
	static constexpr float DOWN_FADE_TIME = 0.01667f * 10.0f;

	// インプットレイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// フェードの時間
	static constexpr float FADE_TIME = 1.2f;

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

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	OperateUI(std::string_view key);
	// デストラクタ
	~OperateUI();
	// 初期化処理
	void Initialize();
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();
	// どのキーが押されたか
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// どのキーが押されているか
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;
	// 衝突判定
	void HitAction(InterSectData data) override;
	// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() override;

	// -----------------------------
	// メンバ変数
	// -----------------------------
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
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// フレーム時間
	float m_elapsedTime;
	// 経過時間
	float m_totalTime;
	// キーが押されたか
	bool m_downKey;
};