#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"

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

	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetSweeping());			// 金棒のステートを変更

	// 顔
	m_boss->SetFace(m_boss->GetFaceAttacking());
}


// ----------------------------------
// 更新処理
// ----------------------------------
void BossSweeping::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;


	// 経過時間の計算
	m_totalSeconds += elapsedTime;

	float t = 0.0f;						// イージングに使用する変数
	static float targetAngle = 0.0f;	// 目標の角度

	if (m_totalSeconds <= CHARGE_TIME)	// 開始からためモーション中なら
	{
		t = m_totalSeconds / CHARGE_TIME;
		targetAngle = m_angle - ROTATE_ANGLE * Easing::easeOutCirc(t);	// ためモーションを行う
	}

	else if (
		m_totalSeconds >= WINDUP_TIME &&	// 待機時間を過ぎた　かつ
		m_totalSeconds <= ATTACK_TIME)		// 攻撃時間中なら
	{
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * Easing::easeOutBack(t);	// 薙ぎ払いモーションを行う
	}


	if(m_totalSeconds > END_TIME)	m_boss->ChangeState(m_boss->GetBossIdling());		// 待機状態に遷移


	m_boss->SetAngle(DirectX::XMConvertToRadians(targetAngle));							// 角度を設定
}


// ----------------------------------
// 事後更新処理
// ----------------------------------
void BossSweeping::PostUpdate()
{
	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceIdling());
}



// ----------------------------------
// 終了処理
// ----------------------------------
void BossSweeping::Finalize()
{
}
