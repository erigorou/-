#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

// 関数を使用するのに必要な引数
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// ヘッダーファイル
#include "Game/Boss/States/Header/BossIdling.h"

// --------------------------
// コンストラクタ
// --------------------------
BossIdling::BossIdling(Boss* boss)
	:
	m_boss(boss),
	m_angle{},
	m_totalSeconds{}
{
}


// --------------------------
// デストラクタ
// --------------------------
BossIdling::~BossIdling()
{
}


// --------------------------
// 初期化処理
// --------------------------
void BossIdling::Initialize()
{
}


// --------------------------
// 設定処理(in)
// --------------------------
void BossIdling::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 武器のステートを変更
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceIdling());

	m_angle = m_boss->GetAngle();
}


// --------------------------
// 更新処理
// --------------------------
void BossIdling::Update(const float& elapsedTime)
{
	// 経過時間を加算
	m_totalSeconds += elapsedTime;
	// アニメーションの更新
	UpdateAnimation();
	// 次のステートに遷移するかを検知
	CheckNextState();
}


// --------------------------
// 敵の挙動更新処理
// --------------------------
void BossIdling::UpdateAnimation()
{
	// プレイヤーの座標を取得
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// アングルを計算
	float targetAngle = Math::CalculationAngle(parentPos, playerPos);
	// 角度差を計算
	float angleDiff = targetAngle - m_angle;

	// 角度差が180度を超える場合、回転方向を反転
	if (angleDiff > DirectX::XM_PI)
	{
		angleDiff -= DirectX::XM_2PI;
	}
	else if (angleDiff < -DirectX::XM_PI)
	{
		angleDiff += DirectX::XM_2PI;
	}

	// 線形補間で最短距離で回転
	m_angle += angleDiff * 0.1f;
	// 角度を設定
	m_boss->SetAngle(m_angle);
}



// ----------------------------------
// 次のステートに遷移するかを検知
// ----------------------------------
void BossIdling::CheckNextState()
{
	// プレイヤーの座標を取得
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// １秒で行動を変更する
	if (m_totalSeconds >= TOTAL_TIME)
	{
		int random = Math::RandomInt(0, TOTAL_RATE);
		float distance = Vector3::Distance(parentPos, playerPos);
		// 遠い距離の場合
		if (distance > FAR_DISTANCE)
		{
			if (random % 2 == 0)	m_boss->ChangeState(m_boss->GetBossApproaching());	// 追従
			else					m_boss->ChangeState(m_boss->GetBossDashAttacking());	// ダッシュ攻撃
		}
		// 近い距離の場合
		else
		{
			// ランダムで行動を変更する
			if (random <= SWEEPING_RATE)
			{
				// 薙ぎ払い
				m_boss->ChangeState(m_boss->GetBossSweeping());
			}
			else if (random <= ATTACKING_RATE)
			{
				// 攻撃
				m_boss->ChangeState(m_boss->GetBossAttacking());
			}
			else if (random <= DASH_ATTACK_RATE)
			{
				// ダッシュ攻撃
				m_boss->ChangeState(m_boss->GetBossDashAttacking());
			}
			else if (random == IDLING_RATE)
			{
				// 何もしない
				m_boss->ChangeState(m_boss->GetBossIdling());
			}
		}
	}
}



// --------------------------
// 設定処理(out)
// --------------------------
void BossIdling::PostUpdate()
{
	// 修正点があればここに記載
}


// ---------------------------
// 終了処理
// ---------------------------
void BossIdling::Finalize()
{
}