// ----------------------------------------------
//
// 毎回書くのが面倒な処理をクラス化する為の関数
//
// ----------------------------------------------

#pragma once
#include "pch.h"

using namespace DirectX::SimpleMath;

class Math
{
	// 関数の宣言
public:

	/// <summary>
	/// 小数点第〇〇位より下を切り捨てる処理
	/// </summary>
	/// <param name="num">切り捨てる数</param>
	/// <param name="prec">小数点第〇〇位</param>
	/// <returns>切り捨てた値</returns>
	static float truncate_float(const float num, const int prec)
	{
		// 指定された制度に基づいてスケールを計算
		float scale = std::pow(10.f, prec);
		// 小数点以下第 "prec" 位　から先を切り捨てし、返す。
		return std::trunc(num * scale) / scale;
	};


	/// <summary>
	/// 小数点第〇〇位より下を切り捨てる処理
	/// </summary>
	/// <param name="num">切り捨てる数</param>
	/// <param name="prec">小数点第〇〇位</param>
	/// <returns>切り捨てた値</returns>
	static Vector3 truncate_vector(Vector3 num, const int prec)
	{
		// x,y,zをそれぞれ切り捨てする
		num.x = truncate_float(num.x, prec);
		num.y = truncate_float(num.y, prec);
		num.z = truncate_float(num.z, prec);
		// 計算結果を返す
		return num;
	}


	/// <summary>
	/// プレイヤーと当たっている時に押し戻しの量を計測する
	/// </summary>
	/// <param name="A">押し戻される側のSphere</param>
	/// <param name="B">固定されるほうのSphere</param>
	/// <returns>押し戻しの値　[ Aの座標　+= return値 ]</returns>
	static Vector3 pushBack_BoundingSphere(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
	{
		// AとBが当たっていなければ早期リターン
		if (!A.Intersects(B))	return Vector3::Zero;

		// 衝突判定　プレイヤーが押し戻される--[====================>

		// プレイヤーの中心と敵の中心との差分ベクトル
		Vector3 diffVec = A.Center - B.Center;
		// プレイヤーの中心と敵の中心との距離を取得
		float diffLength = diffVec.Length();
		// プレイヤーと敵の半径の合計を取得
		float sumLength = A.Radius + B.Radius;
		// プレイヤーがめり込んだ距離
		diffLength = sumLength - diffLength;
		// 差分ベクトルを正規化する
		diffVec.Normalize();
		// 押し戻すベクトルを計算し、返す
		return diffVec * diffLength;
	}
};