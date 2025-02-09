// -----------------------------------------------------------------
// 
// 名前: Wall.cpp
// 内容: ステージの境界壁を描画するクラス
// 作成: 池田桜輔
// 
// -----------------------------------------------------------------

#pragma once
// インクルード
#include "Interface/IObject.h"

/// <summary>
/// ステージの境界壁を描画するクラス
/// </summary>
class Wall : public IObject
{
	// ------------------------
	// 固定値
	// ------------------------
public:
	// 大きさ（固定値）
	static constexpr float WALL_SCALE = 2.0f;
	// 当たり判定の半径
	static constexpr float COLLISION_RADIUS = 160.0f;
	// 当たり判定の大きさ
	static constexpr float COLLISION_SCALE = 10.0f;
	// モデルを移動させる位置
	static constexpr DirectX::SimpleMath::Vector3 WALL_POS = { 0.0f, -60.0f, 0.0f };

	// ------------------------
	// メンバ関数(公開)
	// ------------------------
public:
	// 壁の衝突判定を取得
	DirectX::BoundingSphere* GetCollision() { return m_collision.get(); }
	// 壁の座標の取得
	DirectX::SimpleMath::Vector3 GetPosition() override { return DirectX::SimpleMath::Vector3::Zero; }
	// コンストラクタ
	Wall();
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

	// ------------------------
	// メンバ関数(非公開)
	// ------------------------
private:
	// 衝突判定の生成
	void CreateCollision();
	// ワールド行列更新処理
	void UpdateWorldMatrix();

	// ------------------------
	// メンバ変数
	// ------------------------
private:
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	DirectX::Model* m_model;
	// 当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_collision;
	// 外の当たり判定に見立てたもの
	std::unique_ptr<DirectX::BoundingSphere> m_overCollision;
};
