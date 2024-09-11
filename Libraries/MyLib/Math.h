// ----------------------------------------------
//
// 毎回書くのが面倒な処理をクラス化する為の関数
//
// ----------------------------------------------

#pragma once
#include "pch.h"
#include "cmath"

using namespace DirectX::SimpleMath;

class Math
{
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
		float scale = static_cast<float>(std::pow(10.0f, prec));
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

		Vector3 diffVec = A.Center - B.Center;	// プレイヤーの中心と敵の中心との差分ベクトルを取得する

		float diffLength = diffVec.Length();	// プレイヤーの中心と敵の中心との距離を取得する
		float sumLength = A.Radius + B.Radius;	// プレイヤーと敵の半径の合計を取得する

		diffLength = sumLength - diffLength;	// プレイヤーがめり込んだ距離を計算する
		diffVec.Normalize();					// 差分ベクトルを正規化する

		return diffVec * diffLength;		// 押し戻すベクトルを計算し、返す
	}


	/// <summary>
	/// サイン波を計算する方法（Updateで更新）
	/// </summary>
	/// <param name="totalTime">計算が始まってからの時間</param>
	/// <param name="amplitude">振幅</param>
	/// <param name="frequency">周波数</param>
	/// <returns>サイン波の値</returns>
	static float CalculatingSinWave(const float totalTime , const float amplitude, const float frequency)
	{
		// サイン波の計算結果
		return amplitude * std::sin(2.0f * DirectX::XM_PI * frequency * totalTime);
	}


	/// <summary>
	/// 2つの位置ベクトル間の角度を計算します。
	/// </summary>
	/// <param name="posA">最初の位置ベクトル</param>
	/// <param name="posB">2番目の位置ベクトル</param>
	/// <returns>位置ベクトル間の角度（ラジアン単位）</returns>
	static float CalculationAngle(DirectX::SimpleMath::Vector3 posA, DirectX::SimpleMath::Vector3 const posB)
	{
		using namespace DirectX::SimpleMath;

		Vector3 forward = posB - posA;		// 敵の方向をベクトルで取得
		forward.Normalize();				// 正規化

		Vector3 worldForward = Vector3::Forward;			// ワールド座標の前方ベクトルを作成
		float dotProduct = forward.Dot(worldForward);		// 内積を取得
		float angle = acosf(dotProduct);					// 内積から角度を取得(弧度法)

		Vector3 crossProduct = forward.Cross(worldForward);	// カメラの前方向ベクトルが右方向に向いているかどうかで符号を決定
		angle = (crossProduct.y < 0)? -angle : angle;		// -180 ~ 180に収める。

		return angle;		// 角度（ラジアン単位）を返す
	}
};



// clamp

// 01clamp 0~1の間で値を返してくれるclamp

// イージング