// ----------------------------------------------
// 名前:	PlayerAttacking2.h
// 内容:	プレイヤーの攻撃状態
// 作成:	池田
// ----------------------------------------------
// インクルード
#pragma once
#include "pch.h"
#include "Game/Sound/Sound.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Weapon/WeaponState.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerAttacking2.h"

// ---------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">親のポインタ</param>
// ---------------------------------------
PlayerAttacking2::PlayerAttacking2(Player* player)
	:
	m_player{ player },
	m_totalSeconds{}
{
}

// ---------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------
PlayerAttacking2::~PlayerAttacking2()
{
}

// ---------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------
void PlayerAttacking2::Initialize()
{
}

// ---------------------------------------------
/// <summary>
/// ステート変更処理(in)
/// </summary>
// ---------------------------------------------
void PlayerAttacking2::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;

	// 武器を攻撃状態に変更
	SwordState state = SwordState::Attack2;
	EventMessenger::Execute(EventList::ChangeSwordState, &state);

	// 効果音の再生
	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void PlayerAttacking2::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	UpdateAnimation();

	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, PlayerState::Idling, elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// アニメーション更新
/// </summary>
// ---------------------------------------------
void PlayerAttacking2::UpdateAnimation()
{
	if (m_totalSeconds > Player::NORMAL_ATTACK_TIME) return;

	// アニメーションの回転量を取得
	DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;
	// プレイヤーの回転量を取得
	float currentAngle = m_player->GetAngle();
	// イージングで使用するための変数 0-1
	float easing = m_totalSeconds / Player::NORMAL_ATTACK_TIME;
	// 回転量の計算を行う
	currentAnimPos.y = -INIT_ROT + END_ROT * Easing::easeOutBack(easing) + currentAngle;
	// radianに変換
	currentAnimPos.y = DirectX::XMConvertToRadians(currentAnimPos.y);
	// プレイヤーの設定
	m_player->SetAnimationRotate(currentAnimPos);
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押された瞬間)
/// </summary>
/// <param name="key">押されたキー</param>
// ----------------------------------------------
void PlayerAttacking2::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// 回避
	if (key == DirectX::Keyboard::LeftShift && m_totalSeconds >= Player::X_COOL_TIME) {
		m_player->ChangeState(PlayerState::Dodging);
	}
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押され続けている間)
/// </summary>
/// <param name="key">押されているキー</param>
// ---------------------------------------------
void PlayerAttacking2::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------
/// <summary>
/// ステート大更新処理(out)
/// </summary>
// ---------------------------------------------
void PlayerAttacking2::PostUpdate()
{
	// アニメーションを初期化
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);

	// 攻撃状態に変更
	SwordState state = SwordState::Idle;
	EventMessenger::Execute(EventList::ChangeSwordState, &state);
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void PlayerAttacking2::Finalize()
{
}