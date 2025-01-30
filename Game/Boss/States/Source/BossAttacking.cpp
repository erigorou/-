#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Boss/States/Header/BossAttacking.h"

// --------------------------------------
// コンストラクタ
// --------------------------------------
BossAttacking::BossAttacking(Boss* boss)
	:
	m_angle{},
	m_boss(boss),
	m_totalSeconds{}
{
}

// --------------------------------------
// デストラクタ
// --------------------------------------
BossAttacking::~BossAttacking()
{
}


// --------------------------------------
// 初期化処理
// --------------------------------------
void BossAttacking::Initialize()
{
}


// --------------------------------------
// ステート更新処理（in）
// --------------------------------------
void BossAttacking::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 武器のステートを変更
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetAttacking());

	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceAttacking());
}


// --------------------------------------
// 更新処理
// --------------------------------------
void BossAttacking::Update(const float& elapsedTime)
{
	// 経過時間を加算
	m_totalSeconds += elapsedTime;

	// プレイヤーの座標を取得
	DirectX::SimpleMath::Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	// 敵の座標を取得
	DirectX::SimpleMath::Vector3 parentPos = m_boss->GetPosition();
	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);

	// 攻撃中まではプレイヤーを追尾するようにする
	if (m_totalSeconds <= Cudgel_Attacking::ATTACK_TIME)
	{
		// プレイヤーを追尾
		m_boss->SetAngle(m_angle);
	}

	if (m_totalSeconds >= TOTAL_TIME)
	{
		m_boss->ChangeState(m_boss->GetBossIdling());	// 待機状態に遷移
	}
}


// --------------------------------------
// ステート変更処理(out)
// --------------------------------------
void BossAttacking::PostUpdate()
{
	// 武器のステートを変更する
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------------------------
// 終了処理
// --------------------------------------
void BossAttacking::Finalize()
{
}