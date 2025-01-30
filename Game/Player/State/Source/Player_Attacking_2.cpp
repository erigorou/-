/// ---------------------------------------
///
/// プレイヤーが攻撃をする処理
///
/// ---------------------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Libraries/MyLib/EasingFunctions.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"
#include "Game/Boss/Boss.h"
#include "Game/Player/State/Header/Player_Attacking_2.h"



// コンストラクタ
PlayerAttacking_2::PlayerAttacking_2(Player* player)
	:
	m_player(player)
	, m_totalSeconds()
	, m_attackElapsedTime()
	, m_model()
{
}

// デストラクタ
PlayerAttacking_2::~PlayerAttacking_2()
{
}


// 初期化処理
void PlayerAttacking_2::Initialize()
{


}

// 事前更新処理
void PlayerAttacking_2::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;

	// 武器を攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_2State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);
}

// 更新処理
void PlayerAttacking_2::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	UpdateAnimation();

	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// アニメーションの更新
void PlayerAttacking_2::UpdateAnimation()
{
	if (m_totalSeconds > Player::NORMAL_ATTACK_TIME) return;

	DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;

	// イージングで使用するための変数 0-1
	float t = m_totalSeconds / Player::NORMAL_ATTACK_TIME;

	// 回転量の計算を行う
	float currentAngle = m_player->GetAngle();
	currentAnimPos.y = -40 + 80 * Easing::easeOutBack(t) + currentAngle;

	// radianに変換
	currentAnimPos.y = DirectX::XMConvertToRadians(currentAnimPos.y);

	// 付与
	m_player->SetAnimationRotate(currentAnimPos);

}


// キー入力
void PlayerAttacking_2::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::LeftShift && m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerDodgingState());
}


void PlayerAttacking_2::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}



// 事後更新処理
void PlayerAttacking_2::PostUpdate()
{
	// 武器を攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);

	// アニメーションを初期化
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);
}



// 終了処理
void PlayerAttacking_2::Finalize()
{
}
