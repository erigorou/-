// ----------------------------------------------
// 名前:	PlayerAttacking1.h
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
#include "Game/Player/State/Header/PlayerAttacking1.h"

// ---------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">親のポインタ</param>
// ---------------------------------------
PlayerAttacking1::PlayerAttacking1(Player* player)
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
PlayerAttacking1::~PlayerAttacking1()
{
}

// ---------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------
void PlayerAttacking1::Initialize()
{
}

// ---------------------------------------------
/// <summary>
/// ステート更新処(in)
/// </summary>
// ---------------------------------------------
void PlayerAttacking1::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;

	// 武器を攻撃状態に変更
	SwordState state = SwordState::Attack1;
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
void PlayerAttacking1::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// アニメーションの更新
	UpdateAnimation();

	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, PlayerState::Idling, elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// アニメーション更新
/// </summary>
// ---------------------------------------------
void PlayerAttacking1::UpdateAnimation()
{
	if (m_totalSeconds > Player::NORMAL_ATTACK_TIME) return;

	// アニメーションの回転量を取得
	DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;
	// プレイヤーの回転量を取得
	float currentAngle = m_player->GetAngle();
	// イージングで使用するための変数 0-1
	float easing = m_totalSeconds / Player::NORMAL_ATTACK_TIME;
	// 回転量の計算を行う
	currentAnimPos.y = INIT_ROT - END_ROT * Easing::easeOutExpo(easing) + currentAngle;
	// radianに変換
	currentAnimPos.y = DirectX::XMConvertToRadians(currentAnimPos.y);
	// プレイヤーに設定する
	m_player->SetAnimationRotate(currentAnimPos);
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押された瞬間)
/// </summary>
/// <param name="key">押されたキー</param>
// ----------------------------------------------
void PlayerAttacking1::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// 連続攻撃
	if (key == DirectX::Keyboard::X && m_totalSeconds >= Player::X_COOL_TIME){
		m_player->ChangeState(PlayerState::Attacking2);
	}
	// 回避
	if (key == DirectX::Keyboard::LeftShift){
		m_player->ChangeState(PlayerState::Dodging);
	}
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押され続けている間)
/// </summary>
/// <param name="key">押されているキー</param>
// ---------------------------------------------
void PlayerAttacking1::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------
/// <summary>
/// ステート更新処理(out)
/// </summary>
// ---------------------------------------------
void PlayerAttacking1::PostUpdate()
{
	// アニメーションの更新
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);

	// 武器を待機状態に変更
	SwordState state = SwordState::Idle;
	EventMessenger::Execute(EventList::ChangeSwordState, &state);
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void PlayerAttacking1::Finalize()
{
}