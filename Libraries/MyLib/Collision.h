// ----------------------------------------------
// 当たり判定に関する処理を記載
//
// ＊モデルの大きさ通りのBoundingBoxを返す関数
// ＊
//
// ----------------------------------------------

#pragma once

#include "pch.h"							// プリコンパイル済みヘッダー
#include "Libraries/Microsoft/DebugDraw.h"	// 当たり判定を可視化するのに使用

class Collision
{
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
