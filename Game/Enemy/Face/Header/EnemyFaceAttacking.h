#pragma once
#include "pch.h"
#include "Interface/IFace.h"

class Enemy;

class EnemyFaceAttacking : public IFace
{

// 固定値
public:

// 公開関数
public:
	EnemyFaceAttacking(Enemy* enemy);	// コンストラクタ
	~EnemyFaceAttacking() override;	// デストラクタ


	void DrawFace(								// かおの描画
		const DirectX::SimpleMath::Matrix mat,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	)	override;


// 内部関数
private:
	void CreateModel();	// モデルの生成


// 内部変数
private:
	Enemy* m_enemy;				// 敵の大元クラス
	DirectX::Model* m_model;	// モデル
};
