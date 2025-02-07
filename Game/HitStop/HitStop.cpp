// --------------------------------
// ヒットストップを計算するクラス
// --------------------------------

#include "pch.h"
#include "HitStop.h"
#include "Libraries/MyLib/EasingFunctions.h"

std::unique_ptr<HitStop> HitStop::s_hitStop = nullptr;


// ---------------------------------------------
/// <summary>
/// インスタンスの取得
/// </summary>
/// <returns>ポインタ</returns>
// ---------------------------------------------
HitStop* const HitStop::GetInstance()
{
	// インスタンスが生成されていない場合
	if (s_hitStop == nullptr)
	{
		//　生成する
		s_hitStop.reset(new HitStop);
	}
	// インスタンスを返す
	return s_hitStop.get();
}

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------
HitStop::HitStop()
	: m_isActive(false)
	, m_smoothDeltaTime(0.f)
	, m_totalSeconds(0.f)
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
HitStop::~HitStop()
{
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void HitStop::Update(float elapsedTime)
{
	// 経過時間を登録
	m_smoothDeltaTime = elapsedTime;
	// 何もない場合終了
	if (!m_isActive) return;
	// ヒットストップ時の計算を行う
	CalculateSmoothDeltaTime(elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// ヒットストップの計算（残り時間）
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void HitStop::CalculateSmoothDeltaTime(float elapsedTime)
{
	// 経過時間の記録
	m_totalSeconds += elapsedTime;
	// ヒットストップ解除の判定
	m_isActive = (m_totalSeconds < HITSTOP_TIME);

	if (m_totalSeconds < DELAY) return;

	// イージング用の時間を計算
	float t = Easing::easeInQuint((m_totalSeconds - DELAY) / (HITSTOP_TIME - DELAY));

	// ヒットストップの状態に応じて経過時間を設定　TRUE : FALSE
	m_smoothDeltaTime = m_isActive ? elapsedTime * t : (m_totalSeconds = 0.0f);
}