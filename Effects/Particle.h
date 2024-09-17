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


class DustTrialParticle;
class SwordTrialParticle;


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

	// 剣の軌跡のシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShaderSword;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderSword;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShaderSword;

	// 煙のパーティクルのシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShaderDust;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderDust;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShaderDust;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// ビルボード行列
	DirectX::SimpleMath::Matrix m_billboard;

	// 頂点カラーテクスチャー
	std::vector<DirectX::VertexPositionColorTexture> m_dustVertices;

	// カメラの座標
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// 焦点
	DirectX::SimpleMath::Vector3 m_cameraTarget;

	// *******************************************************
	// 軌跡ダスト
	std::list<DustTrialParticle> m_dustTrail;
	// 軌跡ダストの時間
	float m_timerDustTrail;
	// ******************************************************
	// 剣の軌跡パーティクル ※※※※※※※
	std::list<SwordTrialParticle> m_swordTrial;
	// 剣の軌跡の時間
	float m_timerSwordTrial;


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
	void Update(float elapsedTimer,const DirectX::SimpleMath::Vector3 playerPosition, const DirectX::SimpleMath::Vector3 playerVelocity);
	// パーティクルの描画処理
	void Render(DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	

	// ビルボード行列の作成
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);

	// 軌跡の土埃を生成する処理
	void CreateTrailDust(float elapsedTimer);
	// 剣の軌跡を生成する処理
	void CreateSwordTrial(DirectX::VertexPositionTexture ver[4]);
	// たたきつけの煙パーティクルの生成
	void CreateSlamDust(DirectX::SimpleMath::Vector3 center);

private:
	// 剣パーティクルの描画設定
	void DrawSwordParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 cameraDir);
	// 煙パーティクルの描画設定
	void DrawDustParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 cameraDir);


	// シェーダーの作成
	void CreateShader();

};