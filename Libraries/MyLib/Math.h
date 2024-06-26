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
};