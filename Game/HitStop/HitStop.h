// --------------------------------------------------
// 名前:	HitStop.cpp
// 内容:	ヒットストップ実行クラス
// 作成:	池田桜輔
// --------------------------------------------------
#pragma once
#ifndef HITSTOP_H
#define HITSTOP_H
// インクルード
#include "pch.h"

/// <summary>
/// 火っとストップクラス
/// </summary>
class HitStop
{

	// ------------------------------
	// 定数
	// ------------------------------

public:
	// ヒットストップの遅延時間
	static constexpr float DELAY = 0.05f;
	// ヒットストップ時間
	static constexpr float HITSTOP_TIME = DELAY + 0.2f;


	// ------------------------------
	// アクセサ
	// ------------------------------
	// インスタンスの取得
	static HitStop* const GetInstance();
	// ヒットストップが有効か
	bool IsActive() const { return m_isActive; }
	// ヒットストップを有効にする
	void SetActive() { m_isActive = true; }
	// ヒットストップを計算した時間を取得
	inline float GetSmoothDeltaTime() const { return m_smoothDeltaTime; }

	// ------------------------------
	// メンバ関数(公開)
	// ------------------------------
public:
	// コンストラクタ
	HitStop();
	// デストラクタ
	~HitStop();
	// 更新処理
	void Update(float elapsedTime);

	// ------------------------------
	// メンバ関数(非公開)
	// ------------------------------
private:
	// ヒットストップの計算（残り時間）
	void CalculateSmoothDeltaTime(float elapsedTime);

	// ------------------------------
	// メンバ変数
	// ------------------------------
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