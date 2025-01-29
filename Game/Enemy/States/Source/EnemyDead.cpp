#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries//MyLib/EasingFunctions.h"

// 関数を使用するのに必要な引数
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/EnemyManager/EnemyManager.h"
#include "Game/Messenger/EventMessenger.h"

// ヘッダーファイル
#include "Game/Enemy/States/Header/EnemyDead.h"


// ---------------------------
// コンストラクタ
// ---------------------------
EnemyDead::EnemyDead(Enemy* enemy)
	:
	m_angle(0.0f),
	m_enemy(enemy),
	m_totalSeconds(),
	m_tiltAngle(0.0f)
{
}


// ---------------------------
// デストラクタ
// ---------------------------
EnemyDead::~EnemyDead()
{
	Finalize();
}


// ---------------------------
// 初期化処理
// ---------------------------
void EnemyDead::Initialize()
{
}


// ---------------------------
// 変更処理(in)
// ---------------------------
void EnemyDead::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0;

	// 回転を取得
	m_angle = m_enemy->GetAngle();
	// 状態開始時の傾きを取得
	m_startTilt = m_enemy->GetBodyTilt();

	// 武器のステートを変更
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceIdling());

	// 全ての敵のHPを0にする
	m_enemy->GetPlayScene()->GetEnemyManager()->AllGoblinHPZero();
}


// ---------------------------
// 更新処理
// ---------------------------
void EnemyDead::Update(const float& elapsedTime)
{
	// 経過時間を加算
	m_totalSeconds += elapsedTime;

	// １秒で行動を変更する
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// 敵をさせる
		m_enemy->DeadAction();
		return;
	}

	// アニメーションの更新
	UpdateAnimation();
}


// ---------------------------
// アニメーションの更新
// ---------------------------
void EnemyDead::UpdateAnimation()
{
	// 正規化した時間を求める
	float t = m_totalSeconds / TOTAL_TIME;

	// イージングアニメーションを用いて傾きを求める
	m_tiltAngle = m_startTilt + ( MAX_TILT_ANGLE - m_startTilt) * Easing::easeOutBounce(t);

	// 傾きを設定
	m_enemy->SetBodyTilt(DirectX::XMConvertToRadians(m_tiltAngle));

	// カメラを揺らすタイミングを図る
	if (m_tiltAngle <= CAMERA_SHAKE_TIMING)
	{
		float shakePower = CAMERA_SHAKE_POWER;
		EventMessenger::Execute("CameraShake", &shakePower);

		DirectX::SimpleMath::Vector3 enemyPos = m_enemy->GetPosition();

		EventMessenger::Execute("CreateSpamDust", &enemyPos);
	}
}


// ---------------------------
// 変更処理(out)
// ---------------------------
void EnemyDead::PostUpdate()
{
}


// ---------------------------
// 終了処理
// ---------------------------
void EnemyDead::Finalize()
{
}