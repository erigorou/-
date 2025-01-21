// ----------------------------------------------
//
// 毎回書くのが面倒な処理をクラス化する為の関数
//
// ----------------------------------------------

#pragma once
#include "pch.h"
#include "random"

using namespace DirectX::SimpleMath;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Math
{
public:
	// ====================================================================================================
	/// <summary>
	/// 小数点第〇〇位より下を切り捨てる処理
	/// </summary>
	/// <param name="num">切り捨てる数</param>
	/// <param name="prec">小数点第〇〇位</param>
	/// <returns>切り捨てた値</returns>
	// ====================================================================================================
	static float truncate_float(const float num, const int prec)
	{
		// 指定された制度に基づいてスケールを計算
		float scale = static_cast<float>(std::pow(10.0f, prec));
		// 小数点以下第 "prec" 位　から先を切り捨てし、返す。
		return std::trunc(num * scale) / scale;
	};


	// ====================================================================================================
	/// <summary>
	/// 小数点第〇〇位より下を切り捨てる処理
	/// </summary>
	/// <param name="num">切り捨てる数</param>
	/// <param name="prec">小数点第〇〇位</param>
	/// <returns>切り捨てた値</returns>
	// ====================================================================================================
	static Vector3 truncate_vector(Vector3 num, const int prec)
	{
		// x,y,zをそれぞれ切り捨てする
		num.x = truncate_float(num.x, prec);
		num.y = truncate_float(num.y, prec);
		num.z = truncate_float(num.z, prec);
		// 計算結果を返す
		return num;
	}


	// ====================================================================================================
	/// <summary>
	/// プレイヤーと当たっている時に押し戻しの量を計測する
	/// </summary>
	/// <param name="A">押し戻される側のSphere</param>
	/// <param name="B">固定されるほうのSphere</param>
	/// <returns>押し戻しの値　[ Aの座標　+= return値 ]</returns>
	// ====================================================================================================
	static Vector3 pushBack_BoundingSphere(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
	{
		Vector3 diffVec = A.Center - B.Center;	// Aの中心とBの中心との差分ベクトルを取得する

		float diffLength = diffVec.Length();	// Aの中心とBの中心との距離を取得する
		float sumLength = A.Radius + B.Radius;	// AとBの半径の合計を取得する

		diffLength = sumLength - diffLength;	// Aがめり込んだ距離を計算する
		diffVec.Normalize();					// 差分ベクトルを正規化する

		return diffVec * diffLength;			// 押し戻すベクトルを計算し、返す
	}



	// ====================================================================================================
	/// <summary>
	/// ゲームのステージ等の内側にいるときに外に出れないように押し戻しの量を計算する
	/// </summary>
	/// <param name="A">押し戻されるほうのSphere</param>
	/// <param name="B">動かないほうのSphere</param>
	/// <returns>押し戻しの値 [ Aの座標 += return値 ]</returns>
	// ====================================================================================================
	static Vector3 pushFront_BoundingSphere(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
	{
		Vector3 diffVec = A.Center - B.Center; // Aの中心とBの中心との差分ベクトルを取得する

		float diffLength = diffVec.Length(); // Aの中心とBの中心との距離を取得する
		float boundaryLength = B.Radius - A.Radius; // AがBの内側に収まるための距離を計算する

		// AがBの外側にいる場合
		if (diffLength > boundaryLength)
		{
			diffLength = boundaryLength - diffLength; // Aがめり込んだ距離を計算する
			diffVec.Normalize(); // 差分ベクトルを正規化する

			// 押し戻しベクトルを計算し返す
			return diffVec * diffLength;
		}

		// 押し戻しが不要な場合はゼロベクトルを返す
		return Vector3(0, 0, 0);
	}





	// ====================================================================================================
	/// <summary>
	/// サイン波を計算する方法（Updateで更新）
	/// </summary>
	/// <param name="totalTime">計算が始まってからの時間</param>
	/// <param name="amplitude">振幅</param>
	/// <param name="frequency">周波数</param>
	/// <returns>サイン波の値</returns>
	// ====================================================================================================
	static float CalculatingSinWave(const float totalTime , const float amplitude, const float frequency)
	{
		// サイン波の計算結果
		return amplitude * std::sin(2.0f * DirectX::XM_PI * frequency * totalTime);
	}

	// ====================================================================================================
	/// <summary>
	/// 2つの位置ベクトル間の角度を計算します。
	/// </summary>
	/// <param name="posA">最初の位置ベクトル</param>
	/// <param name="posB">2番目の位置ベクトル</param>
	/// <returns>位置ベクトル間の角度（ラジアン単位）</returns>
	// ====================================================================================================
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

	// ====================================================================================================
	/// <summary>
	/// min <= return <= max の範囲で乱数を生成する
	/// </summary>
	/// <param name="min">最低値</param>
	/// <param name="max">最高値</param>
	// ====================================================================================================
	static int RandomInt(int min, int max)
	{
		if (min > max) std::swap(min, max);

		std::random_device seed;							// ランダムなシード値を生成
		std::default_random_engine engine(seed());			// シード値を元に乱数生成エンジンを定義
		std::uniform_int_distribution<> random(min, max);	// minからmaxまでの一様乱数生成器を定義
		return random(engine);								// 乱数を生成して返す
	}

	// ====================================================================================================
	/// <summary>
	/// min <= return <= max の範囲で浮動小数点数型の乱数を生成する
	/// </summary>
	/// <param name="min">最低値</param>
	/// <param name="max">最高値</param>
	/// <returns>生成された乱数</returns>
	// ====================================================================================================
	static float RandomFloat(float min, float max)
	{
		if (min > max) std::swap(min, max);

		std::random_device seed;								// ランダムなシード値を生成
		std::default_random_engine engine(seed());				// シード値を元に乱数生成エンジンを定義
		std::uniform_real_distribution<float> random(min, max); // minからmaxまでの一様乱数生成器を定義
		return random(engine);									// 乱数を生成して返す
	}

	// ====================================================================================================
	/// <summary>
	/// min <= return <= max の範囲で浮動小数点数型の乱数を生成する
	/// </summary>
	/// <param name="min">最低値</param>
	/// <param name="max">最高値</param>
	/// <returns>生成された乱数</returns>
	// ====================================================================================================
	static DirectX::SimpleMath::Vector3 RandomVector3(DirectX::SimpleMath::Vector3 min, DirectX::SimpleMath::Vector3 max)
	{
		std::random_device seed;								// ランダムなシード値を生成
		std::default_random_engine engine(seed());				// シード値を元に乱数生成エンジンを定義
		std::uniform_real_distribution<float> randomX(min.x, max.x); // minからmaxまでの一様乱数生成器を定義
		std::uniform_real_distribution<float> randomY(min.y, max.y); // minからmaxまでの一様乱数生成器を定義
		std::uniform_real_distribution<float> randomZ(min.z, max.z); // minからmaxまでの一様乱数生成器を定義

		return Vector3(randomX(engine), randomY(engine), randomZ(engine)); // 乱数を生成して返す
	}


	// ====================================================================================================
	/// <summary>
	/// 0 ~ 1 で正規化されたサイン波を返す
	/// </summary>
	/// <param name="t">秒数</param>
	/// <returns>正規化されたサイン波</returns>
	// ====================================================================================================
	static float NormalizeSin(float t)
	{
		return static_cast<float>(sin(t * M_PI));
	}



	// ====================================================================================================
	/// <summary>
	/// 線形補間を行い、開始値と終了値の間の値を返します。
	/// </summary>
	/// <param name="start">補間の開始値</param>
	/// <param name="end">補間の終了値</param>
	/// <param name="t">補間の割合（0.0 ～ 1.0）</param>
	/// <returns>補間された値</returns>
	// ====================================================================================================
	static float LerpFloat(float start, float end, float t)
	{
		// t を範囲 [0, 1] にクランプ
		t = Clamp(t, 0.0f, 1.0f);

		// 線形補間の計算
		return (1.0f - t) * start + t * end;
	}


	// ====================================================================================================
	/// <summary>
	/// 線形補間を行い、2つのベクトル間の中間ベクトルを返します。
	/// </summary>
	/// <param name="start">補間の開始ベクトル</param>
	/// <param name="end">補間の終了ベクトル</param>
	/// <param name="t">補間の割合（0.0 ～ 1.0）</param>
	/// <returns>補間されたベクトル</returns>
	// ====================================================================================================
	static DirectX::SimpleMath::Vector3 LerpVector(
		const DirectX::SimpleMath::Vector3& start,
		const DirectX::SimpleMath::Vector3& end,
		float t
	)
	{
		// t を範囲 [0, 1] にクランプ
		t = Clamp(t, 0.0f, 1.0f);

		// 各成分を線形補間
		return {
			(1.0f - t) * start.x + t * end.x,
			(1.0f - t) * start.y + t * end.y,
			(1.0f - t) * start.z + t * end.z
		};
	}


	// ====================================================================================================
	/// <summary>
	/// クランプ関数 min max の間に制限する
	/// </summary>
	/// <typeparam name="T">テンプレート</typeparam>
	/// <param name="value">基準となる値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>結果</returns>
	// ====================================================================================================
	template <typename T>
	static T Clamp(const T& value, const T& min, const T& max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}


	// ====================================================================================================
	/// <summary>
	/// 時間内にいるのかを判定する
	/// </summary>
	/// <param name="first">開始時間</param>
	/// <param name="now">現在</param>
	/// <param name="end">終了時間</param>
	/// <returns>判定(bool)</returns>
	// ====================================================================================================
	static bool InTime(float first, float now, float end)
	{
		return first < now && now <= end;
	}
};

