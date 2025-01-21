// ステージの境界壁
#pragma once

#include "Interface/IObject.h"

class PlayScene;

class Wall : public IObject
{
public: 
	// ------------------------
	// 固定値
	// ------------------------
	static const float WALL_SCALE;						// 大きさ（固定値）
	
	static constexpr float COLLISION_RADIUS = 160.0f;	// 当たり判定の半径
	static constexpr float COLLISION_SCALE = 10.0f;		// 当たり判定の大きさ

	static constexpr DirectX::SimpleMath::Vector3 WALL_POS = { 0.0f, -60.0f, 0.0f };	// モデルを移動させる位置


	// ------------------------
	// 公開関数
	// ------------------------
	// 壁の衝突判定を取得
	DirectX::BoundingSphere* GetCollision() { return m_collision.get();	}
	// 壁の座標の取得
	DirectX::SimpleMath::Vector3 GetPosition() override	{ return DirectX::SimpleMath::Vector3::Zero; }

	// コンストラクタ
	Wall(PlayScene* playScene);
	// デストラクタ
	~Wall();
	// 初期化
	void Initialize();
	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);
	// 終了処理
	void Finalize();

	// 衝突処理
	void HitAction(InterSectData data)override;


private:

	// 衝突判定の生成
	void CreateCollision();

	// ワールド行列更新処理
	void UpdateWorldMatrix();

	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド行列
	
	DirectX::Model* m_model;	// モデル
	std::unique_ptr<DirectX::BoundingSphere> m_collision;	// 当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_overCollision; // 外の当たり判定に見立てたもの

	PlayScene* m_playScene;	// プレイシーン
};
