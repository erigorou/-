#pragma once
#include "pch.h"
#include "Interface/IFace.h"

class Enemy;

class EnemyFaceIdling : public IFace
{
// ---------------------------
// メンバ関数（公開）
// ---------------------------
public:
	// コンストラクタ
	EnemyFaceIdling(Enemy* enemy);
	// デストラクタ
	~EnemyFaceIdling() override;

	// 顔の描画
	void DrawFace(
		const DirectX::SimpleMath::Matrix mat,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	)	override;

	
// ---------------------------
// メンバ変数
// ---------------------------
private:
	// 敵の大元クラス
	Enemy* m_enemy;
	// モデル
	DirectX::Model* m_model;
};
