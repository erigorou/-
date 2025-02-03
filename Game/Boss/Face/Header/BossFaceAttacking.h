#pragma once
#include "pch.h"
#include "Interface/IFace.h"

class Boss;

class BossFaceAttacking : public IFace
{
	// ---------------------------
	// メンバ関数（公開）
	// ---------------------------
public:
	// コンストラクタ
	BossFaceAttacking(Boss* Boss);
	// デストラクタ
	~BossFaceAttacking() override;
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
	Boss* m_Boss;
	// モデル
	DirectX::Model* m_model;
};
