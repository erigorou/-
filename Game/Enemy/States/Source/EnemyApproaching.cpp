#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Enemy/States/Header/EnemyApproaching.h"


// コンストラクタ
EnemyApproaching::EnemyApproaching(Enemy* enemy)
	:
	m_enemy(enemy),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)* Enemy::ENEMY_SPEED),
	m_angle(0.0f),
	m_worldMat(DirectX::SimpleMath::Matrix::Identity),
	m_totalSeconds(0.0f),
	m_amplitude(1.0f),
	m_finishTime(1.0f),
	m_frequency(1.0f),
	m_shakePower(1.0f)
{
	m_particles = m_enemy->GetPlayScene()->GetParticle();
}


// デストラクタ
EnemyApproaching::~EnemyApproaching()
{
}


// 初期化処理
void EnemyApproaching::Initialize()
{
	// 速度を設定（前にしか動かない）
	m_velocity = DirectX::SimpleMath::Vector3::Forward;
}


// 事前更新処理
void EnemyApproaching::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.f; 

	m_position	= m_enemy->GetPosition();		// 座標の取得
	m_angle		= m_enemy->GetAngle();			// 回転の取得
	m_worldMat	= m_enemy->GetWorldMatrix();	// ワールド行列の取得
}


// 更新処理
void EnemyApproaching::Update(const float& elapsedTime)
{

	using namespace DirectX::SimpleMath;
	
	// 合計の時間を計算する
	m_totalSeconds += elapsedTime;

	// サイン波の計算(上下移動)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// 絶対値を取ることでジャンプしているような挙動をする　※
	m_position.y = fabsf(m_position.y);

	// プレイヤーの座標を取得
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();

	// 移動でカメラを揺らす
	if (m_position.y <= MINIMAL)
	{
		// カメラを揺らす
		EventMessenger::Execute("CameraShake", &m_shakePower);
		// サウンドを再生
		Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);


		// プレイヤーとの距離が近い場合
		if (DirectX::SimpleMath::Vector3::Distance(m_position, playerPos) <= 20.0f)
		{
			// 効果音を鳴らす
			Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);

			int random = Math::RandomInt(0, 4);

			if (random <= 1)
				m_enemy->ChangeState(m_enemy->GetEnemySweeping());		// 掃討
			else if (random == 2)
				m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// 攻撃
			else if (random == 3)
				m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// 何もしない

			return;
		}
	}

	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(m_position, playerPos);
	
	// 回転行列の作成
	Matrix angleMat  = Matrix::CreateScale(Enemy::ENEMY_SCALE)
					*= Matrix::CreateRotationY(-m_angle);

	// 前方に移動
	m_position += Vector3::Transform(m_velocity * elapsedTime * NORMALIZE_VELOCITY, angleMat);

	// 2秒経過で待機モーションに変更
	if (m_totalSeconds >= 2.0f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}


	// 回転角を設定する
	m_enemy->SetAngle(m_angle);

	// 座標を設定する
	m_enemy->SetPosition(m_position);
}




// 事後更新処理
void EnemyApproaching::PostUpdate()
{
	// ワールド行列を全体に設定する
	m_enemy->SetWorldMatrix(m_worldMat);
	// 敵の位置を0で固定する
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);

	EventMessenger::Execute("CameraShake", &m_shakePower);
	EventMessenger::Execute("CreateSlamDust", &m_position);
}


void EnemyApproaching::Finalize()
{
}