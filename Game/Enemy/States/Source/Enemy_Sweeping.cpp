#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Enemy/States/Header/Enemy_Sweeping.h"


// ----------------------------------
// コンストラクタ
// ----------------------------------
Enemy_Sweeping::Enemy_Sweeping(Enemy* enemy)
	: m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
}


// ----------------------------------
// デストラクタ
// ----------------------------------
Enemy_Sweeping::~Enemy_Sweeping()
{
}


// ----------------------------------
// 初期化処理
// ----------------------------------
void Enemy_Sweeping::Initialize()
{
}



// ----------------------------------
// 事前更新処理
// ----------------------------------
void Enemy_Sweeping::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	m_angle = DirectX::XMConvertToDegrees(m_enemy->GetAngle());

	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetSweeping());			// 金棒のステートを変更

	// 顔
	m_enemy->SetFace(m_enemy->GetFaceAttacking());
}


// ----------------------------------
// 更新処理
// ----------------------------------
void Enemy_Sweeping::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;


	// 経過時間の計算
	m_totalSeconds += elapsedTime;

	float t = 0.0f;						// イージングに使用する変数
	static float targetAngle = 0.0f;	// 目標の角度

	if (m_totalSeconds <= CHARGE_TIME)	// 開始からためモーション中なら
	{
		t = m_totalSeconds / CHARGE_TIME;
		targetAngle = m_angle - ROTATE_ANGLE * Easing::easeOutCirc(t);					// ためモーションを行う
	}

	else if (
		m_totalSeconds >= WINDUP_TIME &&	// 待機時間を過ぎた　かつ
		m_totalSeconds <= ATTACK_TIME)		// 攻撃時間中なら
	{
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * Easing::easeOutBack(t);	// 薙ぎ払いモーションを行う
	}


	if(m_totalSeconds > END_TIME)	m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// 待機状態に遷移


	m_enemy->SetAngle(DirectX::XMConvertToRadians(targetAngle));							// 角度を設定
}


// ----------------------------------
// 事後更新処理
// ----------------------------------
void Enemy_Sweeping::PostUpdate()
{
	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// ----------------------------------
// 描画処理
// ----------------------------------
void Enemy_Sweeping::CheckHitPlayerBody()
{

}


// ----------------------------------
// 終了処理
// ----------------------------------
void Enemy_Sweeping::Finalize()
{
}
