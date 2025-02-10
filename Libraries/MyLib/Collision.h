// ---------------------------------------------------------------------------------
//
// 名前:	Collision.cpp
// 内容:	指定されたモデルから有向境界ボックスを生成
// 制作:	池田桜輔
//
// ---------------------------------------------------------------------------------
#pragma once
// インクルード
#include "pch.h"

/// <summary>
/// 衝突判定クラス
/// </summary>
class Collision
{
	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// -------------------------------------------------------------------------
	/// <summary>
	/// 指定されたモデルから向き付き境界ボックス（OBB）を取得
	/// </summary>
	/// <param name="model">境界ボックスを取得する対象のモデル</param>
	/// <returns>大きさを獲得したOBB</returns>
	// -------------------------------------------------------------------------
	static DirectX::BoundingOrientedBox Get_BoundingOrientedBox_FromMODEL(DirectX::Model* model)
	{
		// モデルがnullの時に中断をする
		assert(model != nullptr && "model is null.");

		DirectX::BoundingBox initialAABB;
		DirectX::BoundingOrientedBox initialOBB;

		// モデルから境界ボックスを取得する
		for (auto it = model->meshes.cbegin(); it != model->meshes.cend(); it++)
		{
			if (it == model->meshes.cbegin())
			{
				// 最初のメッシュの境界ボックスを設定する
				initialAABB = (*it)->boundingBox;
			}
			else
			{
				// それ以降のメッシュの境界ボックスをマージする
				DirectX::BoundingBox::CreateMerged(initialAABB, initialAABB, (*it)->boundingBox);
			}
		}

		// AABBからOBBを生成する
		DirectX::BoundingOrientedBox::CreateFromBoundingBox(initialOBB, initialAABB);

		// obbを返す
		return initialOBB;
	}
};
