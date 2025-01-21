#pragma once
#include "pch.h"
#include "Interface/IFace.h"

class Enemy;

class EnemyFaceIdling : public IFace
{

// 固定値
public:
	const wchar_t* MODEL_PATH = L"Resources/Models/Oni/Face/IdlingFace.cmo";

// 公開関数
public:
	EnemyFaceIdling(Enemy* enemy);	// コンストラクタ
	~EnemyFaceIdling() override;	// デストラクタ


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
