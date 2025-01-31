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
#include "Game/Messenger/EventMessenger.h"
#include "Game/Weapon/WeaponState.h"

#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Player/State/Header/Player_Attacking_1.h"


// -----------------------
// コンストラクタ
// -----------------------
PlayerAttacking_1::PlayerAttacking_1(Player* player)
	:
	m_player(player)
	, m_totalSeconds()
	, m_model()
{
}


// -----------------------
// デストラクタ
// -----------------------
PlayerAttacking_1::~PlayerAttacking_1()
{
}


// -----------------------
// 初期化処理
// -----------------------
void PlayerAttacking_1::Initialize()
{
}


// -----------------------
// 事前更新処理
// -----------------------
void PlayerAttacking_1::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;

	// 武器を攻撃状態に変更
	SwordState state = SwordState::Attack1;
	EventMessenger::Execute("ChangeSwordState", &state);

	// 効果音の再生
	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);
}


// -----------------------
// 更新処理
// -----------------------
void PlayerAttacking_1::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// アニメーションの更新
	UpdateAnimation();

	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}



// -----------------------
// アニメーション更新
// -----------------------
void PlayerAttacking_1::UpdateAnimation()
{
	if (m_totalSeconds > Player::NORMAL_ATTACK_TIME) return;

	DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;

	// イージングで使用するための変数 0-1
	float t = m_totalSeconds / Player::NORMAL_ATTACK_TIME;

	// 回転量の計算を行う
	float currentAngle = m_player->GetAngle();
	currentAnimPos.y = 40 - 80 * Easing::easeOutExpo(t) + currentAngle;

	// radianに変換
	currentAnimPos.y = DirectX::XMConvertToRadians(currentAnimPos.y);

	// プレイヤーに設定する
	m_player->SetAnimationRotate(currentAnimPos);
}


// -----------------------
// キー入力
// -----------------------
void PlayerAttacking_1::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X && m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerAttackingState2());

	// 回避
	if (key == DirectX::Keyboard::LeftShift)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
}

// -----------------------
// キー入力
// -----------------------
void PlayerAttacking_1::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// -----------------------
// 事後更新
// -----------------------
void PlayerAttacking_1::PostUpdate()
{
	// アニメーションの更新
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);

	// 武器を待機状態に変更
	SwordState state = SwordState::Idle;
	EventMessenger::Execute("ChangeSwordState", &state);
}


// -----------------------
// 終了処理
// -----------------------
void PlayerAttacking_1::Finalize()
{
}