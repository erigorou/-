#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

// 関数を使用するのに必要な引数
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// ヘッダーファイル
#include "Game/Enemy/States/Header/EnemyIdling.h"

// --------------------------
// コンストラクタ
// --------------------------
EnemyIdling::EnemyIdling(Enemy* enemy)
	:
	m_angle(0.f),
	m_enemy(enemy),
	m_totalSeconds()
{
}


// --------------------------
// デストラクタ
// --------------------------
EnemyIdling::~EnemyIdling()
{
}


// --------------------------
// 初期化処理
// --------------------------
void EnemyIdling::Initialize()
{
}


// --------------------------
// 設定処理(in)
// --------------------------
void EnemyIdling::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0;
	// 武器のステートを変更
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// --------------------------
// 更新処理
// --------------------------
void EnemyIdling::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_enemy->GetPosition();

	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_enemy->SetAngle(m_angle);

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
				m_enemy->ChangeState(m_enemy->GetEnemyApproaching());	// 追従
			}
			else
			{
				m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// ダッシュ攻撃
			}
		}
		// 近い距離の場合
		else
		{
			if (random <= 3)
				m_enemy->ChangeState(m_enemy->GetEnemySweeping());		// 掃討
			else if(random <= 7)
				m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// 攻撃
			else if (random <= 8)
				m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// ダッシュ攻撃
			else if(random == 10)
				m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// 何もしない
		}
	}
}

// --------------------------
// 設定処理(out)
// --------------------------
void EnemyIdling::PostUpdate()
{
	// 修正点があればここに記載
}


// ---------------------------
// 終了処理
// ---------------------------
void EnemyIdling::Finalize()
{
}