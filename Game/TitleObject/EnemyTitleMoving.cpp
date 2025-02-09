// ---------------------------------------------------------
//
// 名前:	EnemyTitleMoving.cpp
// 内容:	敵の攻撃状態
//			ステートパターンで実装
// 作成;	池田桜輔
//
// ---------------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/Messenger/EventMessenger.h"
#include "Libraries/MyLib/Math.h"
#include "TitleEnemy.h"
#include "EnemyTitleMoving.h"

// ------------------------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵のポインタ</param>
// ------------------------------------------------------------------------------
EnemyTitleMoving::EnemyTitleMoving(TitleEnemy* enemy)
	: 
	m_enemy(enemy),
	m_position{},
	m_velocity{},
	m_angle{},
	m_worldMat(DirectX::SimpleMath::Matrix::Identity),
	m_totalSeconds{},
	m_isJump(false),
	m_canShake(false),
	m_shakePower(SHAKE_POWER)
{
}

// ------------------------------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ------------------------------------------------------------------------------
EnemyTitleMoving::~EnemyTitleMoving()
{
}

// ------------------------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ------------------------------------------------------------------------------
void EnemyTitleMoving::Initialize()
{
	// 速度を設定（前にしか動かない）
	m_velocity = DirectX::SimpleMath::Vector3::Forward;
}

// ------------------------------------------------------------------------------
/// <summary>
/// 事前更新処理
/// </summary>
// ------------------------------------------------------------------------------
void EnemyTitleMoving::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 座標を初期化
	m_position = Vector3::Zero;
}

// ------------------------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ------------------------------------------------------------------------------
void EnemyTitleMoving::Update(const float& elapsedTime)
{
	// 合計の時間を計算する
	m_totalSeconds += elapsedTime;

	// サイン波の計算(上下移動)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, AMPLITUDE, FREQUENCY);

	// 絶対値を取ることでジャンプしているような挙動をする
	m_position.y = fabsf(m_position.y);

	// 移動でカメラを揺らす
	if (m_position.y <= MINIMAL)
	{
		// カメラを揺らす
		EventMessenger::Execute(EventList::ShakeCamera, &m_shakePower);

		// 土埃を発生させる
		EventMessenger::Execute(EventList::CreateBashDust, &m_position);

		// ステートを変更する
		m_enemy->ChangeState(TitleEnemy::BossState::IDLING);
	}

	// 回転角を設定する
	m_enemy->SetAngle(m_angle);
	// 座標を設定する
	m_enemy->SetPosition(m_position);
}

// ------------------------------------------------------------------------------
/// <summary>
/// 事後更新処理
/// </summary>
// ------------------------------------------------------------------------------
void EnemyTitleMoving::PostUpdate()
{
	// ワールド行列を全体に設定する
	m_enemy->SetWorldMatrix(m_worldMat);
	// 敵の位置を0で固定する
	m_position.y = 0.0f;
	m_enemy->SetPosition(m_position);
}

// ------------------------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ------------------------------------------------------------------------------
void EnemyTitleMoving::Finalize()
{
}
