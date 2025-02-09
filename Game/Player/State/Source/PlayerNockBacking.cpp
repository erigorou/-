// ----------------------------------------------
// 名前:	PlayerNockBacking
// 内容:	プレイヤーのやられ状態
// 作成:	池田
// ----------------------------------------------
// インクルード
#pragma once
#include "pch.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/WeaponState.h"
#include "Game/Player/State/Header/PlayerNockBacking.h"
#include "Game/Messenger/EventMessenger.h"

// ---------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">親のポインタ</param>
// ---------------------------------------
PlayerNockBacking::PlayerNockBacking(Player* player)
	: m_player{ player },
	m_totalSeconds{},
	m_elapsedTime{},
	m_velocity{ 0.0f, 0.0f, -SPEED },
	m_bodyTilt{}
{
}

// ---------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------
PlayerNockBacking::~PlayerNockBacking()
{
}

// ---------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------
void PlayerNockBacking::Initialize()
{
}

// ---------------------------------------
/// <summary>
/// ステート更新処(in)
/// </summary>
// ---------------------------------------
void PlayerNockBacking::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void PlayerNockBacking::Update(const float& elapsedTime)
{
	// 経過時間を記録
	m_elapsedTime = elapsedTime;
	// プレイヤーの位置を取得
	m_position = m_player->GetPosition();
	// ターゲットの位置を取得
	Vector3 targetPos = *(Vector3*)EventMessenger::ExecuteGetter(GetterList::GetTargetPosition);
	// プレイヤーの向きを敵に向ける
	m_player->SetAngle(Math::CalculationAngle(m_position, targetPos));
	// アニメーションの更新
	UpdateAnimation();
	// ステート開始から時間を計測、一定時間で別のStateへ遷移させる
	m_player->TimeComparison(m_totalSeconds, END_TIME, PlayerState::Idling, elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// アニメーション更新
/// </summary>
// ---------------------------------------------
void PlayerNockBacking::UpdateAnimation()
{
	// 経過時間を加算
	m_totalSeconds += m_elapsedTime;

	// ノックバックのアニメーション
	if (m_totalSeconds <= NOCKBACKING_TIME){
		NockBackAnimation();
	}
	// 再帰アニメーション
	else if (m_totalSeconds <= RETURN_TIME){
		ReturnAnimation();
	}
}

// ---------------------------------------------
/// <summary>
/// ノックバックアニメーション
/// </summary>
// ---------------------------------------------
void PlayerNockBacking::NockBackAnimation()
{
	// 時間を正規化する
	float t = std::min(1.0f, m_totalSeconds / NOCKBACKING_TIME);

	// ターゲットの位置を取得
	Vector3 targetPos = *(Vector3*)EventMessenger::ExecuteGetter(GetterList::GetTargetPosition);
	Vector3 playerPos = m_player->GetPosition();

	// どっちの方向にノックバックするのか
	m_nockBackAngle = Math::CalculationAngle(targetPos, playerPos);
	Matrix rotMatrix = Matrix::CreateRotationY(-m_nockBackAngle);

	// ノックバック量
	m_position += Vector3::Transform(m_velocity, rotMatrix) * m_elapsedTime;

	// sin波で上下移動を実装
	m_position.y = Math::NormalizeSin(t) * MAX_UP_VALUE;

	// 傾き
	m_bodyTilt.x = DirectX::XMConvertToRadians(-90 * Easing::easeOutBack(t));

	// プレイヤーの位置を設定
	m_player->SetPosition(m_position);
	// アニメーション用の設定
	m_player->SetAnimationRotate(m_bodyTilt);
}

// -------------------------------------------
/// <summary>
/// 再帰アニメーション
/// </summary>
// -------------------------------------------
void PlayerNockBacking::ReturnAnimation()
{
	// 時間を正規化する
	float t = std::min(1.0f, (m_totalSeconds - NOCKBACKING_TIME) / (RETURN_TIME - NOCKBACKING_TIME));
	// ノックバック量
	Matrix rotMatrix = Matrix::CreateRotationY(-m_nockBackAngle);
	m_position += Vector3::Transform(m_velocity, rotMatrix) * m_elapsedTime;
	// sin波で上下移動を実装
	m_position.y = Math::NormalizeSin(t) * RETURN_UP_VALUE;
	// 傾き
	m_bodyTilt.x = DirectX::XMConvertToRadians(-90 - 270 * Easing::easeOutBack(t));
	// プレイヤーの位置を設定
	m_player->SetPosition(m_position);
	// アニメーション用の設定
	m_player->SetAnimationRotate(m_bodyTilt);
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押された瞬間)
/// </summary>
/// <param name="key">押されたキー</param>
// ----------------------------------------------
void PlayerNockBacking::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押され続けている間)
/// </summary>
/// <param name="key">押されているキー</param>
// ---------------------------------------------
void PlayerNockBacking::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------
/// <summary>
/// ステート更新処理(out)
/// </summary>
// ---------------------------------------------
void PlayerNockBacking::PostUpdate()
{	
	// 武器を待機状態に変更
	SwordState state = SwordState::Idle;
	EventMessenger::Execute(EventList::ChangeSwordState, &state);
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void PlayerNockBacking::Finalize()
{
}