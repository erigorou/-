#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
// 関数を使用するのに必要な引数
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// ヘッダーファイル
#include "Game/Enemy/States/Header/EnemyStarting.h"


// コンストラクタ
EnemyStarting::EnemyStarting(Enemy* enemy)
	:
	m_angle(0.f),
	m_enemy(enemy),
	m_totalSeconds(),
	m_isEndDelay(false)
{
}


// デストラクタ
EnemyStarting::~EnemyStarting()
{
}


// 初期化処理
void EnemyStarting::Initialize()
{
}


// 事前更新処理
void EnemyStarting::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0;
	// 武器のステートを変更
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// 更新処理
void EnemyStarting::Update(const float& elapsedTime)
{

	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_enemy->GetPosition();

	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_enemy->SetAngle(m_angle);

	UpdateAnimation();

	// １秒で行動を変更する
	if (m_totalSeconds >= TOTAL_TIME)
	{
		int random = Math::RandomInt(0, 10);
		float distance = Vector3::Distance(parentPos, playerPos);
		// 遠い距離の場合
		if (distance > 20.0f)
		{
			if (random % 2 == 0)	m_enemy->ChangeState(m_enemy->GetEnemyApproaching());	// 追従
			else					m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// ダッシュ攻撃
		}

		// 近い距離の場合
		else
		{
			if (random <= 3)		m_enemy->ChangeState(m_enemy->GetEnemySweeping());		// 掃討
			else if(random <= 7)	m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// 攻撃
			else if (random == 10)	m_enemy->ChangeState(m_enemy->GetEnemyDashAttacking());	// ダッシュ攻撃
		}
	}
}


void EnemyStarting::UpdateAnimation()
{
	if (m_totalSeconds < DELAY_TIME)		UpdateDelay();
	else if (m_totalSeconds < MOVE_TIME)	UpdateMove();
}


void EnemyStarting::UpdateDelay()
{
	m_position = m_enemy->GetPosition();
	m_enemy->SetPosition(DirectX::SimpleMath::Vector3(m_position.x, START_HEIGHT, m_position.z));
}


void EnemyStarting::UpdateMove()
{
	// 秒数を正規化
	float t = (m_totalSeconds - DELAY_TIME) / (MOVE_TIME - DELAY_TIME);
	// 高いところに初期位置を設定し、徐々に下に降ろす
	m_position.y = std::fabsf(START_HEIGHT - (START_HEIGHT * Easing::easeInOutBack(t)));
	// 位置を設定
	m_enemy->SetPosition(m_position);

	// 一定以下の高さになったらエフェクトを発動
	if (m_position.y < PLAY_EFFECT_HEIGHT)
	{
		PlayEffect();
	}
}


void EnemyStarting::PlayEffect()
{
	// 一度だけ実行する
	if (m_isEndDelay) return;

	// エフェクトを発動
	m_enemy->GetPlayScene()->GetParticle()->CreateSlamDust(m_position);

	float shakePower = 2.0f;
	EventMessenger::Execute("CameraShake", &shakePower);


	// 音声を再生
	Sound::PlaySE(Sound::SE_TYPE::ENEMY_MOVE);

	// フラグを有効にする
	m_isEndDelay = true;
}





// 事後更新処理
void EnemyStarting::PostUpdate()
{
	// 修正点があればここに記載
}

void EnemyStarting::Finalize()
{
}