#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries//MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Weapon/WeaponState.h"

#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "../../States/Header/BossDashAttacking.h"

// --------------------
// コンストラクタ
// --------------------
BossDashAttacking::BossDashAttacking(Boss* boss)
	:
	m_boss(boss),
	m_angle{},
	m_bodyTilt{},
	m_totalSeconds{}
{
}

// --------------------
// デストラクタ
// --------------------
BossDashAttacking::~BossDashAttacking()
{
}

// --------------------
// 初期化処理
// --------------------
void BossDashAttacking::Initialize()
{
	// プレイヤーの取得
	void* player = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = player ? static_cast<IObject*>(player) : nullptr;

	// 金棒
	void* cudgel = EventMessenger::ExecuteGetter(GetterList::GetCudgel);
	m_cudgel = cudgel ? static_cast<Cudgel*>(cudgel) : nullptr;
}

// --------------------
// 事前更新処理
// --------------------
void BossDashAttacking::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;

	// 顔のステートを変更
	FaceState face = FaceState::Attacking;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);

	// 最初は攻撃中ではない
	m_isAttacking = false;

	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);
}

// --------------------
// 更新処理
// --------------------
void BossDashAttacking::Update(const float& elapsedTime)
{
	// 経過時間を更新
	m_elapsedTime = elapsedTime;
	m_totalSeconds += m_elapsedTime;

	// 敵の挙動を更新する
	UpdateAction();

	// 待機状態に遷移
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// ステートを変更（待機状態）
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);
	}
}

// --------------------
// 敵の挙動更新処理
// --------------------
void BossDashAttacking::UpdateAction()
{
	// ためモーションの時
	if (m_totalSeconds <= CHARGE_TIME)	ChargeAction();	// 貯め
	else if (m_totalSeconds <= DASH_TIME)	DashAction();	// ダッシュ
	else if (m_totalSeconds <= WAIT_TIME)	WaitAction();	// 待機
	else if (m_totalSeconds <= RETURN_TIME)	ReturnAction();	// 元に戻る
}

// --------------------
// 貯め
// --------------------
void BossDashAttacking::ChargeAction()
{
	// プレイヤーに攻撃を受けられるフラグを無効化
	EventMessenger::Execute(EventList::PlayerCanHit, &m_isAttacking);

	// プレイヤーの座標を取得
	Vector3 playerPos = m_player->GetPosition();
	// 敵の座標を取得
	Vector3 parentPos = m_boss->GetPosition();
	// 敵から見たプレイヤーの位置を設定する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);
	m_boss->SetAngle(m_angle);

	// イージング用の変数
	float t = m_totalSeconds / CHARGE_TIME;
	// 体の傾きの角度設定
	m_bodyTilt = DirectX::XMConvertToRadians(-TILT_ANGLE * Easing::easeOutBack(t));
	m_boss->SetBodyTilt(m_bodyTilt);
}

// --------------------
// ダッシュ
// --------------------
void BossDashAttacking::DashAction()
{
	// アタック中
	m_isAttacking = true;

	// プレイヤーに攻撃を受けられるフラグを有効化
	EventMessenger::Execute(EventList::PlayerCanHit, &m_isAttacking);

	// 現在の時間に基づいてサイン波で加速度を計算
	float easing = (m_totalSeconds - CHARGE_TIME) / (DASH_TIME - CHARGE_TIME);

	// 座標の更新 *
	float accelerationFactor = sin(static_cast<float>(easing * M_PI)); // サイン波で速度を変化

	// 敵の向きに基づいて前方向を計算
	Vector3 position = m_boss->GetPosition();
	m_velocity = Vector3(0, 0, -MAX_SPEED * accelerationFactor);
	position += Vector3::Transform(m_velocity, m_rotMatrix) * m_elapsedTime;

	// サイン波で上下運動
	float y = fabsf(sin(easing * SIN_SPEED)) * accelerationFactor;
	position.y = y;

	// 計算した座標を本体に設定する
	m_boss->SetPosition(position);

	// プレイヤーを傾ける
	m_bodyTilt = DirectX::XMConvertToRadians(-TILT_ANGLE + TILT_ANGLE_DASH * Easing::easeOutBack(easing));
	m_boss->SetBodyTilt(m_bodyTilt);
}

// --------------------
// 待機
// --------------------
void BossDashAttacking::WaitAction()
{
	// アタック終わり
	m_isAttacking = false;

	// プレイヤーに攻撃を受けられるフラグを有効化
	EventMessenger::Execute(EventList::PlayerCanHit, &m_isAttacking);

	// イージングに使用する秒数を計算（秒数のNormalize)
	float easing = (m_totalSeconds - DASH_TIME) / (WAIT_TIME - DASH_TIME);

	// プレイヤーを傾ける
	m_bodyTilt = DirectX::XMConvertToRadians(TILT_ANGLE - TILT_ANGLE * Easing::easeOutBounce(easing));
	m_boss->SetBodyTilt(m_bodyTilt);
}

// --------------------
// 元に戻る
// --------------------
void BossDashAttacking::ReturnAction()
{
	// 時間の正規化
	float easing = (m_totalSeconds - WAIT_TIME) / (RETURN_TIME - WAIT_TIME);

	// プレイヤーの座標を取得
	Vector3 playerPos = m_player->GetPosition();
	// 敵の座標を取得
	Vector3 parentPos = m_boss->GetPosition();
	// 敵から見たプレイヤーの位置を設定する
	float angle = Math::CalculationAngle(parentPos, playerPos);

	m_angle = Math::LerpFloat(m_angle, angle, easing);

	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);
	m_boss->SetAngle(m_angle);
}

// --------------------
// 事後更新処理
// --------------------
void BossDashAttacking::PostUpdate()
{
	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔のステートを変更
	FaceState face = FaceState::Idling;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}

// --------------------
// 終了処理
// --------------------
void BossDashAttacking::Finalize()
{
}