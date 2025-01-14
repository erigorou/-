#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "TitleEnemy.h"
#include "EnemyTitleMoving.h"
#include "EnemyTitleIdling.h"


// コンストラクタ
EnemyTitleMoving::EnemyTitleMoving(TitleEnemy* enemy)
	: m_enemy(enemy)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f))
	, m_angle(0.0f)
	, m_worldMat(DirectX::SimpleMath::Matrix::Identity)
	, m_totalSeconds(0.0f)
	, m_amplitude(2.0f)
	, m_finishTime(1.0f)
	, m_frequency(1.0f)
	, m_isJump(false)
	, m_canShake(false)
{
}


// デストラクタ
EnemyTitleMoving::~EnemyTitleMoving()
{
}


// 初期化処理
void EnemyTitleMoving::Initialize()
{
	// 速度を設定（前にしか動かない）
	m_velocity = DirectX::SimpleMath::Vector3::Forward;
}


// 事前更新処理
void EnemyTitleMoving::PreUpdate()
{
	using namespace DirectX::SimpleMath;

	// 経過時間を初期化
	m_totalSeconds = 0.0f; 

	m_position = Vector3::Zero;
}


// 更新処理
void EnemyTitleMoving::Update(const float& elapsedTime)
{	
	// 合計の時間を計算する
	m_totalSeconds += elapsedTime;

	// サイン波の計算(上下移動)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// 絶対値を取ることでジャンプしているような挙動をする　※
	m_position.y = fabsf(m_position.y);

	// 移動でカメラを揺らす
	if (m_position.y <= MINIMAL)
	{
		//m_enemy->GetTitleScene()->SetShakeCamera();
		m_enemy->ChangeState(m_enemy->GetTitleEnemyIdling());
	}

	// 回転角を設定する
	m_enemy->SetAngle(m_angle);

	// 座標を設定する
	m_enemy->SetPosition(m_position);
}




// 事後更新処理
void EnemyTitleMoving::PostUpdate()
{
	// ワールド行列を全体に設定する
	m_enemy->SetWorldMatrix(m_worldMat);
	// 敵の位置を0で固定する
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);

	// スパン
	//m_enemy->GetTitleScene()->CleateSpamDust(m_position);
}


void EnemyTitleMoving::Finalize()
{
}