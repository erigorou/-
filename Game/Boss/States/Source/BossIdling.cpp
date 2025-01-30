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
	m_totalSeconds = 0;
	// 武器のステートを変更
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------------
// 更新処理
// --------------------------
void BossIdling::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_boss->SetAngle(m_angle);

	// １秒で行動を変更する
	if (m_totalSeconds >= 1.0f)
	{
		int random = Math::RandomInt(0, 10);
		float distance = Vector3::Distance(parentPos, playerPos);
		// 遠い距離の場合
		if (distance > 20.0f)
		{
			if (random % 2 == 0)
			{
				m_boss->ChangeState(m_boss->GetBossApproaching());	// 追従
			}
			else
			{
				m_boss->ChangeState(m_boss->GetBossDashAttacking());	// ダッシュ攻撃
			}
		}
		// 近い距離の場合
		else
		{
			if (random <= 3)
				m_boss->ChangeState(m_boss->GetBossSweeping());		// 掃討
			else if(random <= 7)
				m_boss->ChangeState(m_boss->GetBossAttacking());		// 攻撃
			else if (random <= 8)
				m_boss->ChangeState(m_boss->GetBossDashAttacking());	// ダッシュ攻撃
			else if(random == 10)
				m_boss->ChangeState(m_boss->GetBossIdling());		// 何もしない
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