#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
#include "Game/Boss/States/Header/BossStarting.h"
// 関数を使用するのに必要な引数
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// --------------------------
// コンストラクタ
// --------------------------
BossStarting::BossStarting(Boss* Boss)
	:
	m_boss(Boss),
	m_angle{},
	m_totalSeconds{},
	m_isEndDelay(false)
{
}

// --------------------------
// デストラクタ
// --------------------------
BossStarting::~BossStarting()
{
}

// --------------------------
// 初期化処理
// --------------------------
void BossStarting::Initialize()
{
}


// --------------------------
// 事前更新処理
// --------------------------
void BossStarting::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 武器のステートを変更
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());
	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------------
// 更新処理
// --------------------------
void BossStarting::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_boss->SetAngle(m_angle);

	// アニメーション更新処理
	UpdateAnimation();

	// １秒で行動を変更する
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// 敵をさせる
		m_boss->ChangeState(m_boss->GetBossIdling());
	}
}



// --------------------------
// アニメーション更新処理
// --------------------------
void BossStarting::UpdateAnimation()
{
	if (m_totalSeconds < DELAY_TIME)		UpdateDelay();
	else if (m_totalSeconds < MOVE_TIME)	UpdateMove();
}


// --------------------------
// アニメーションの遅延処理
// --------------------------
void BossStarting::UpdateDelay()
{
	m_position = m_boss->GetPosition();
	m_boss->SetPosition(DirectX::SimpleMath::Vector3(m_position.x, START_HEIGHT, m_position.z));
}


// --------------------------
// アニメーションの移動処理
// --------------------------
void BossStarting::UpdateMove()
{
	// 秒数を正規化
	float t = (m_totalSeconds - DELAY_TIME) / (MOVE_TIME - DELAY_TIME);
	// 高いところに初期位置を設定し、徐々に下に降ろす
	m_position.y = std::fabsf(START_HEIGHT - (START_HEIGHT * Easing::easeInOutBack(t)));
	// 位置を設定
	m_boss->SetPosition(m_position);

	// 一定以下の高さになったらエフェクトを発動
	if (m_position.y < PLAY_EFFECT_HEIGHT)
	{
		PlayEffect();
	}
}


// --------------------------
// エフェクトを再生する
// --------------------------
void BossStarting::PlayEffect()
{
	// 一度だけ実行する
	if (m_isEndDelay) return;

	// エフェクトを再生
	EventMessenger::Execute("CreateBashDust", &m_position);
	// カメラを揺らす
	float shakePower = SHAKE_POWER;
	EventMessenger::Execute("CameraShake", &shakePower);
	// 音声を再生
	Sound::PlaySE(Sound::SE_TYPE::BOSS_MOVE);
	// フラグを有効にする
	m_isEndDelay = true;
}


// --------------------------
// 事後更新処理
// --------------------------
void BossStarting::PostUpdate()
{
}


// --------------------------
// 終了処理
// --------------------------
void BossStarting::Finalize()
{
}