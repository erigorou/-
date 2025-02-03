#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Game/Weapon/WeaponState.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Boss/States/Header/BossSweeping.h"


// ----------------------------------
// コンストラクタ
// ----------------------------------
BossSweeping::BossSweeping(Boss* Boss)
	: 
	m_angle{},
	m_boss(Boss),
	m_totalSeconds{}
{
}


// ----------------------------------
// デストラクタ
// ----------------------------------
BossSweeping::~BossSweeping()
{
}


// ----------------------------------
// 初期化処理
// ----------------------------------
void BossSweeping::Initialize()
{
}



// ----------------------------------
// 事前更新処理
// ----------------------------------
void BossSweeping::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	m_angle = DirectX::XMConvertToDegrees(m_boss->GetAngle());

	// 武器のステートを変更
	CudgelState state = CudgelState::Sweep;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔のステートを変更
	FaceState face = FaceState::Attacking;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}


// ----------------------------------
// 更新処理
// ----------------------------------
void BossSweeping::Update(const float& elapsedTime)
{
	// 経過時間の加算
	m_totalSeconds += elapsedTime;
	// アニメーションの更新
	UpdateAnimation();
	// 角度の更新
	m_boss->SetAngle(DirectX::XMConvertToRadians(m_targetAngle));
}


// ----------------------------------
// アニメーションの更新
// ----------------------------------
void BossSweeping::UpdateAnimation()
{
	// ためモーションを実行
	if (Math::InTime(0.0f, m_totalSeconds, CHARGE_TIME))
		UpdateChargeMotion();

	// 時間内にいるなら薙ぎ払いモーションを実行
	else if (Math::InTime(WINDUP_TIME, m_totalSeconds, ATTACK_TIME))
		UpdateSweepMotion();

	
	if (m_totalSeconds > END_TIME)
	{
		// ステートを変更（待機状態）
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);

	}

}

// ----------------------------------
// ためモーションの更新
// ----------------------------------
void BossSweeping::UpdateChargeMotion()
{
	// ためる時間に対する経過時間の割合
	float easing = m_totalSeconds / CHARGE_TIME;
	// イージングを用いた角度の計算
	m_targetAngle = m_angle - ROTATE_ANGLE * Easing::easeOutCirc(easing);
}



// ----------------------------------
// 薙ぎ払いモーションの更新
// ----------------------------------
void BossSweeping::UpdateSweepMotion()
{
	// 薙ぎ払いモーションの経過時間に対する割合
	float easing = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
	// イージングを用いた角度の計算
	m_targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * Easing::easeOutBack(easing);
}



// ----------------------------------
// 事後更新処理
// ----------------------------------
void BossSweeping::PostUpdate()
{
	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔のステートを変更
	FaceState face = FaceState::Idling;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}



// ----------------------------------
// 終了処理
// ----------------------------------
void BossSweeping::Finalize()
{
}