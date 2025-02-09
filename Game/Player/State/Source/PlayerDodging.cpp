// ----------------------------------------------
// 名前:	PlayerDodging.h
// 内容:	プレイヤーの回避状態
// 作成:	池田
// ----------------------------------------------
// インクルード
#pragma once
#include "pch.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerDodging.h"

// ---------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">親のポインタ</param>
// ---------------------------------------
PlayerDodging::PlayerDodging(Player* player)
	:
	m_player(player),
	m_totalSeconds{},
	m_finishTime{},
	m_rollingValue{}
{
}

// ---------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------
PlayerDodging::~PlayerDodging()
{
}

// ---------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------
void PlayerDodging::Initialize()
{
}

// ---------------------------------------------
/// <summary>
/// 事前更新処理
/// </summary>
// ---------------------------------------------
void PlayerDodging::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	// 移動方向の設定
	m_velocity = DirectX::SimpleMath::Vector3::Forward * DODGING_SPEED;
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void PlayerDodging::Update(const float& elapsedTime)
{
	float t = elapsedTime;
	// 経過時間を加算
	m_totalSeconds += t;
	// アニメーションの更新
	UpdateAnimation(m_totalSeconds);
	// ステート開始から時間を計測、一定時間で別のStateへ遷移させる
	m_player->TimeComparison(m_totalSeconds, ANIMATION_END_TIME, PlayerState::Idling, elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// アニメーション更新
/// </summary>
/// <param name="totalTime">経過時間</param>
// ---------------------------------------------
void PlayerDodging::UpdateAnimation(float totalTime)
{
	// プレイヤーの座標を取得
	m_position = m_player->GetPosition();

	// ローリングアニメーション
	RollingAnimation(totalTime);
	// キャラクターの移動を調整
	AdjustCharacterTransition(totalTime);
}

// -----------------------------------------
/// <summary>
/// ローリングアニメーション
/// </summary>
/// <param name="totalTime">経過時間</param>
// -----------------------------------------
void PlayerDodging::RollingAnimation(float totalTime)
{
	// イージングの要素数
	float value;

	// 回転処理の部分
	if (totalTime <= SPIN_TIME)
	{
		value = Easing::easeInOutQuad(totalTime / SPIN_TIME);

		m_rollingValue.x = DirectX::XMConvertToRadians(value * ROLLING_ROT + ROLLING_REST_ROT);
	}
	// ローリング後の硬直処理
	else if (totalTime <= SPIN_REST_TIME)
	{
		// SPIN_TIME を越えた分の時間で割合を計算
		value = (totalTime - SPIN_TIME) / (SPIN_REST_TIME - SPIN_TIME);
		float easeValue = Easing::easeOutBack(value);

		float rot = ROLLING_REST_ROT - (easeValue * ROLLING_REST_ROT);

		// 回転をラジアンに変換
		m_rollingValue.x = DirectX::XMConvertToRadians(rot);
	}
	// 回転を適用
	m_player->SetAnimationRotate(m_rollingValue);
}

// -----------------------------------------
/// <summary>
/// プレイヤーの移動を調整
/// </summary>
/// <param name="totalTime">経過時間</param>
// -----------------------------------------
void PlayerDodging::AdjustCharacterTransition(float totalTime)
{
	// イージングの要素数
	float value = 0.0f;

	// ローリング処理の部分
	if (totalTime <= ANIMATION_TIME)
	{
		// イージングを掛けるための時間
		value = Easing::easeOutCubic(totalTime / ANIMATION_TIME);
		m_position.y = value * UP_VALUE;
	}
	// ローリング後の硬直処理
	else if (totalTime - ANIMATION_TIME <= DODGING_RECOVERY_TIME - ANIMATION_TIME)
	{
		// 後処理を行う
		value = Easing::easeInOutQuad((totalTime - ANIMATION_TIME) / (DODGING_RECOVERY_TIME - ANIMATION_TIME));
		m_position.y = UP_VALUE - (value * UP_VALUE);
	}

	// プレイヤーの移動を適用
	ApplyPlayerMovement(m_position);
}

// -----------------------------------------
/// <summary>
/// プレイヤーの移動を適用
/// </summary>
/// <param name="parentPos">プレイヤーの座標</param>
// -----------------------------------------
void PlayerDodging::ApplyPlayerMovement(DirectX::SimpleMath::Vector3& parentPos)
{
	// プレイヤーの回転を取得
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// 速度を設定
	m_velocity *= DODGE_FUNCTION;
	// 端数を消し飛ばす。
	m_velocity = Math::truncate_vector(m_velocity, TRUNCATION_DIGIT);
	// 移動量を座標に反映させながら座標を移動させる。
	parentPos += Vector3::Transform(m_velocity, angle);
	// プレイヤーの座標を更新
	m_player->SetPosition(parentPos);
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押された瞬間)
/// </summary>
/// <param name="key">押されたキー</param>
// ----------------------------------------------
void PlayerDodging::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------
/// <summary>
/// キー入力処理 (キーが押され続けている間)
/// </summary>
/// <param name="key">押されているキー</param>
// ---------------------------------------------
void PlayerDodging::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ---------------------------------------------
/// <summary>
/// 事後更新処理
/// </summary>
// ---------------------------------------------
void PlayerDodging::PostUpdate()
{
	m_position.y = 0.0f;
	// プレイヤーの座標を更新
	m_player->SetPosition(m_position);
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void PlayerDodging::Finalize()
{
}