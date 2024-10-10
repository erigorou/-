// ステージの境界壁
#pragma once

#include "Interface/IObject.h"

class Wall : public IObject
{
public: 
	// 固定値
	static const float WALL_SCALE;					// 大きさ（固定値）
	static constexpr float COLLISION_RADIUS = 1.0f;	// 当たり判定の半径

	// コンストラクタ
	Wall();
	// デストラクタ
	~Wall();
	// 初期化
	void Initialize();
	// 描画処理
	void Render(
		 ID3D11DeviceContext* context
		,DirectX::CommonStates* states
		,const DirectX::SimpleMath::Matrix& view
		,const DirectX::SimpleMath::Matrix& projection
	);
	// 終了処理
	void Finalize();

	// ワールド行列更新処理
	void UpdateWorldMatrix();


	// インターフェースで使用するからしゃーなしでおいている。
	void HitAction(InterSectData data)override;
	DirectX::SimpleMath::Vector3 GetPosition()override;


private:

	void CreateCollision();	// 当たり判定の生成


	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド行列
	std::unique_ptr<DirectX::Model> m_model;	// モデル

	std::unique_ptr<DirectX::BoundingSphere> m_collision;	// 当たり判定



	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;										// ベーシックエフェクト
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;	// プリミティブバッチ
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;									// 入力レイアウト
};
