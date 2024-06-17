// -------------------------------------------
//
// パーティクルを実行：描画するクラス
//
// -------------------------------------------
#pragma once

#include "pch.h"
#include <DeviceResources.h>
#include <list>
#include "Game/CommonResources.h"
#include "Effects/Header/DustTrailParticle.h"

class Particle
{
public:
	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

private:
	// 変数
	DX::DeviceResources* m_pDR;
	// バッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	//	テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// ビルボード行列
	DirectX::SimpleMath::Matrix m_billboard;
	// 頂点カラーテクスチャー
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;

	// カメラの座標
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// 焦点
	DirectX::SimpleMath::Vector3 m_cameraTarget;

	// *******************************************************
	// 軌跡ダスト
	std::list<DustTrialParticle> m_dustTrailParticle;
	// 軌跡ダストの時間
	float m_timer_dustTrail;
	// ******************************************************

	// プレイヤーの座標
	DirectX::SimpleMath::Vector3 m_playerPosition;
	// プレイヤーの速度
	DirectX::SimpleMath::Vector3 m_playerVelocity;

public:
	// 関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	Particle();
	~Particle();

	// テクスチャの読み込み
	void LoadTexture(const wchar_t* path);
	// パーティクルの生成を行う
	void Create();
	// パーティクルの更新処理
	void Update(float elapsedTimer, const DirectX::SimpleMath::Vector3 playerPosition, const DirectX::SimpleMath::Vector3 playerVelocity);
	// パーティクルの描画処理
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// ビルボード行列の作成
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);

private:
	// シェーダーの作成
	void CreateShader();
	// 軌跡の土埃を生成する処理
	void CreateTrailDust(float elapsedTimer);
};