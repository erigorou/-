// ---------===========------------
// ヒットストップ関連クラス
// ---------===========------------

#ifndef HITSTOP_H
#define HITSTOP_H

#pragma once
#include "pch.h"

class HitStop
{
// 固定値 ================
public:

	static constexpr float DELAY = 0.05f;					// ヒットストップの遅延時間
	static constexpr float HITSTOP_TIME = DELAY + 0.1f;	// ヒットストップ時間


// 静的関数　=============
	// インスタンスの取得
	static HitStop* const GetInstance();

// アクセサ関数 ==========
public:
	// ヒットストップが有効か
	bool IsActive() const { return m_isActive; }
	// ヒットストップを有効にする
	void SetActive() { m_isActive = true; }


	inline float GetSmoothDeltaTime() const { return m_smoothDeltaTime; }

// 公開関数 ============
public:
	// コンストラクタ
	HitStop();
	// デストラクタ
	~HitStop();
	// 更新処理
	void Update(float elapsedTime);


// 非公開関数 ==========
private:
	void CalculateSmoothDeltaTime(float elapsedTime);


// 内部変数 ==========
private:
	// インスタンス
	static std::unique_ptr<HitStop> s_hitStop;

	// ヒットストップが有効か
	bool m_isActive;
	// ヒットストップ関連の時間
	float m_smoothDeltaTime;
	// 経過時間
	float m_totalSeconds;


};

#endif // !HITSTOP_H