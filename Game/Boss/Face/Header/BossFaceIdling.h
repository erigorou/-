// ---------------------------------------------
// 名前:	BossFaceIdling.h
// 内容:	ボスの通常時の顔クラス
// 作成:	池田桜輔
// ---------------------------------------------
// インクルード
#pragma once
#include "pch.h"
#include "Interface/IFace.h"

// 前方宣言
class Boss;

/// <summary>
/// ボスの通常時の顔クラス
/// </summary>
class BossFaceIdling : public IFace
{
	// ---------------------------
	// メンバ関数（公開）
	// ---------------------------
public:
	// コンストラクタ
	BossFaceIdling(Boss* Boss);
	// デストラクタ
	~BossFaceIdling() override;

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
