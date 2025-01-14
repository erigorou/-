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

	m_tiltAngle = -90.0f * Easing::easeOutBounce(t);

	// 傾きを設定
	m_enemy->SetBodyTilt(DirectX::XMConvertToRadians(m_tiltAngle));
}


// ---------------------------
// 変更処理(out)
// ---------------------------
void EnemyDead::PostUpdate()
{
	// 修正点があればここに記載
}


// ---------------------------
// 終了処理
// ---------------------------
void EnemyDead::Finalize()
{
}