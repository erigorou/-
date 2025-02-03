#include "pch.h"
#include <cassert>
#include "Interface/IObject.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
#include "Game/Boss/States/Header/BossStarting.h"
#include "Game/Weapon/WeaponState.h"
// 関数を使用するのに必要な引数
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// --------------------------
// コンストラクタ
// --------------------------
BossStarting::BossStarting(Boss* boss)
	:
	m_boss(boss),
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
	// プレイヤーの取得
	void* object = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = object ? static_cast<IObject*>(object) : nullptr;


}


// --------------------------
// 事前更新処理
// --------------------------
void BossStarting::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔のステートを変更
	FaceState face = FaceState::Idling;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);

}


// --------------------------
// 更新処理
// --------------------------
void BossStarting::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	Vector3 playerPos = m_player->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_boss->SetAngle(m_angle);

	// アニメーション更新処理
	UpdateAnimation();

	// １秒で行動を変更する
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// ステートを変更（待機状態）
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);
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
	float easing = (m_totalSeconds - DELAY_TIME) / (MOVE_TIME - DELAY_TIME);
	// 高いところに初期位置を設定し、徐々に下に降ろす
	m_position.y = std::fabsf(START_HEIGHT - (START_HEIGHT * Easing::easeInOutBack(easing)));
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
	EventMessenger::Execute(EventList::CreateBashDust, &m_position);
	// カメラを揺らす
	float shakePower = SHAKE_POWER;
	EventMessenger::Execute(EventList::ShakeCamera, &shakePower);
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