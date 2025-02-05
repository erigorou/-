#include "pch.h"
#include "Libraries//MyLib/EasingFunctions.h"
#include "Game/Weapon/WeaponState.h"

// 関数を使用するのに必要な引数
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Messenger/EventMessenger.h"

// ヘッダーファイル
#include "Game/Boss/States/Header/BossDead.h"
#include "Game/Camera/Camera.h"

// ---------------------------
// コンストラクタ
// ---------------------------
BossDead::BossDead(Boss* boss)
	:
	m_boss(boss),
	m_angle{},
	m_totalSeconds{},
	m_tiltAngle{}
{
}

// ---------------------------
// デストラクタ
// ---------------------------
BossDead::~BossDead()
{
	Finalize();
}

// ---------------------------
// 初期化処理
// ---------------------------
void BossDead::Initialize()
{
	auto object = EventMessenger::ExecuteGetter(GetterList::GetCudgel);
	m_cudgel = object ? static_cast<Cudgel*>(object) : nullptr;
}

// ---------------------------
// 変更処理(in)
// ---------------------------
void BossDead::PreUpdate()
{
	// カメラのステートを変更
	CameraState camera = CameraState::Clear;
	EventMessenger::Execute(EventList::ChangeCamera,&camera);

	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 回転を取得
	m_angle = m_boss->GetAngle();
	// 状態開始時の傾きを取得
	m_startTilt = m_boss->GetBodyTilt();
	// 顔のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeBossFace, &state);

	// 全ての敵のHPを0にする
	EventMessenger::Execute(EventList::DeleteAllGoblin, nullptr);
}

// ---------------------------
// 更新処理
// ---------------------------
void BossDead::Update(const float& elapsedTime)
{
	// 経過時間を加算
	m_totalSeconds += elapsedTime;

	// １秒で行動を変更する
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// 敵をさせる
		m_boss->DeadAction();
		return;
	}
	// アニメーションの更新
	UpdateAnimation();
}

// ---------------------------
// アニメーションの更新
// ---------------------------
void BossDead::UpdateAnimation()
{
	// 正規化した時間を求める
	float t = m_totalSeconds / TOTAL_TIME;

	// イージングアニメーションを用いて傾きを求める
	m_tiltAngle = m_startTilt + (MAX_TILT_ANGLE - m_startTilt) * Easing::easeOutBounce(t);

	// 傾きを設定
	m_boss->SetBodyTilt(DirectX::XMConvertToRadians(m_tiltAngle));

	// カメラを揺らすタイミングを図る
	if (m_tiltAngle <= CAMERA_SHAKE_TIMING)
	{
		float shakePower = CAMERA_SHAKE_POWER;
		EventMessenger::Execute(EventList::ShakeCamera, &shakePower);

		DirectX::SimpleMath::Vector3 BossPos = m_boss->GetPosition();

		EventMessenger::Execute(EventList::CreateBashDust, &BossPos);
	}
}

// ---------------------------
// 変更処理(out)
// ---------------------------
void BossDead::PostUpdate()
{
}

// ---------------------------
// 終了処理
// ---------------------------
void BossDead::Finalize()
{
}