#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Boss/States/Header/BossAttacking.h"
#include "Game/Weapon/WeaponState.h"

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
	// プレイヤーの取得
	void* object = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = object ? static_cast<IObject*>(object) : nullptr;
}


// --------------------------------------
// ステート更新処理（in）
// --------------------------------------
void BossAttacking::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 武器のステートを変更
	CudgelState state = CudgelState::Attack;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔のステートを変更
	FaceState face = FaceState::Attacking;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}


// --------------------------------------
// 更新処理
// --------------------------------------
void BossAttacking::Update(const float& elapsedTime)
{
	// 経過時間を加算
	m_totalSeconds += elapsedTime;

	// プレイヤーの座標を取得
	DirectX::SimpleMath::Vector3 playerPos = m_player->GetPosition();
	// 敵の座標を取得
	DirectX::SimpleMath::Vector3 parentPos = m_boss->GetPosition();
	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	// 攻撃中まではプレイヤーを追尾するようにする
	if (Math::InTime(0.0f, m_totalSeconds, CHASE_TIME))
	{
		// プレイヤーを追尾
		m_boss->SetAngle(m_angle);
	}

	// 状態遷移
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// ステートを変更（待機状態）
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);
	}
}




// --------------------------------------
// ステート変更処理(out)
// --------------------------------------
void BossAttacking::PostUpdate()
{
	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);
	// 顔のステートを変更
	FaceState face = FaceState::Idling;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}


// --------------------------------------
// 終了処理
// --------------------------------------
void BossAttacking::Finalize()
{
}