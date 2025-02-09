// ----------------------------------------------
// 名前:	PlayerIdling.h
// 内容:	プレイヤーの待機状態
// 作成:	池田
// ----------------------------------------------
// インクルード
#pragma once
#include "pch.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerIdling.h"
#include "Game/Messenger/EventMessenger.h"

// ---------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">親のポインタ</param>
// ---------------------------------------
PlayerIdling::PlayerIdling(Player* player)
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
PlayerIdling::~PlayerIdling()
{
}

// ---------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------
void PlayerIdling::Initialize()
{
}

// ---------------------------------------------
/// <summary>
/// ステート変更処理(in)
/// </summary>
// ---------------------------------------------
void PlayerIdling::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;

	// 移動速度の初期化
	m_player->SetSpeed(DirectX::SimpleMath::Vector3::Zero);
	// 移動加速度の初期化
	m_player->SetAcceleration(DirectX::SimpleMath::Vector3::Zero);
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void PlayerIdling::Update(const float& elapsedTime)
{
	// 経過時間の加算
	m_totalSeconds += elapsedTime;
	// ターゲットの位置を取得
	DirectX::SimpleMath::Vector3 targetPos =
		*(DirectX::SimpleMath::Vector3*)EventMessenger::ExecuteGetter(GetterList::GetTargetPosition);
	// プレイヤーの向きを敵に向ける
	m_player->SetAngle(m_player->CalucratePlayerRotation(targetPos));
	// プレイヤーの移動
	m_player->MovePlayer();
}

// ---------------------------------------------
/// <summary>
/// ステート変更処理(out)
/// </summary>
// ---------------------------------------------
void PlayerIdling::PostUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押された瞬間)
/// </summary>
/// <param name="key">押されたキー</param>
// ----------------------------------------------
void PlayerIdling::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// 攻撃挙動
	if (key == DirectX::Keyboard::X) {
		m_player->ChangeState(PlayerState::Attacking1);
	}
	// 回避挙動
	if (key == DirectX::Keyboard::LeftShift) {
		m_player->ChangeState(PlayerState::Dodging);
	}
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押され続けている間)
/// </summary>
/// <param name="key">押されているキー</param>
// ---------------------------------------------
void PlayerIdling::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void PlayerIdling::Finalize()
{
}