// ---------------------------------------------------------
//
// 名前:	EnemyTitleIdling.cpp
// 内容:	敵の待機状態
//			ステートパターンで実装
// 作成;	池田桜輔
//
// ---------------------------------------------------------
// インクルード
#include "pch.h"
#include "TitleEnemy.h"
#include "EnemyTitleIdling.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
EnemyTitleIdling::EnemyTitleIdling(TitleEnemy* enemy)
	: 
	m_enemy{ enemy },
	m_position{},
	m_velocity{},
	m_angle{},
	m_worldMat(DirectX::SimpleMath::Matrix::Identity),
	m_totalSeconds{}
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
EnemyTitleIdling::~EnemyTitleIdling()
{
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void EnemyTitleIdling::Initialize()
{
	// 速度を設定（前にしか動かない）
	m_velocity = DirectX::SimpleMath::Vector3::Forward;
}

// ---------------------------------------------------------
/// <summary>
/// 事前更新処理
/// </summary>
// ---------------------------------------------------------
void EnemyTitleIdling::PreUpdate()
{
	using namespace DirectX::SimpleMath;

	// 経過時間を初期化
	m_totalSeconds = 0.0f;

	m_position = Vector3::Zero;
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
// ---------------------------------------------------------
void EnemyTitleIdling::Update(const float& elapsedTime)
{
	// 合計の時間を計算する
	m_totalSeconds += elapsedTime;
	// 一定時間経過後、ステートを変更
	if (m_totalSeconds > TitleEnemy::COOL_TIME){
		m_enemy->ChangeState(TitleEnemy::BossState::MOVING);
	}
	// 回転行列の作成
	DirectX::SimpleMath::Matrix angleMat = 
		// スケール行列 * 回転行列
		DirectX::SimpleMath::Matrix::CreateScale(TitleEnemy::TITLE_ENEMY_SCALE) * 
		DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);

	// 回転角を設定する
	m_enemy->SetAngle(m_angle);
	// 座標を設定する
	m_enemy->SetPosition(m_position);
}

// ---------------------------------------------------------
/// <summary>
/// 事後更新処理
/// </summary>
// ---------------------------------------------------------
void EnemyTitleIdling::PostUpdate()
{
	// ワールド行列を全体に設定する
	m_enemy->SetWorldMatrix(m_worldMat);
	// 敵の位置を0で固定する
	m_position.y = 0.0f;
	m_enemy->SetPosition(m_position);
}

// ---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void EnemyTitleIdling::Finalize()
{
}