// ----------------------------------------------
//
// 毎回書くのが面倒な処理をクラス化する為の関数
//
// ----------------------------------------------

#pragma once
#include "pch.h"


class Math
{
	// 関数の宣言
private:

	float truncate(const float num, const float prec)
	{
		// 指定された制度に基づいてスケールを計算
		float scale = std::pow(10.f, prec);
		// 小数点以下第 "prec" 位　から先を切り捨てし、返す。
		return std::trunc(num * scale) / scale;
	};














	// シングルトン化させるための情報や関数
private:
	// Math
	static std::unique_ptr<Math> m_math;

public:
	// Mathクラスのインスタンスを取得する
	static Math* const GetInstance()
	{
		if (m_math == nullptr)
		{
			// Mathクラスのインスタンスを生成する
			m_math.reset(new Math());
		}
		// Mathクラスのインスタンスを返す
		return m_math.get();
	}
};

// クラス外で初期化
std::unique_ptr<Math> Math::m_math = nullptr;