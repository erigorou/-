// -----------------------------------------------------
// 名前:	BossIdling.cpp
// 内容:	ボスの待機ステートクラス
// 作成:	池田桜輔
// -----------------------------------------------------
// インクルード
#include "pch.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Weapon/WeaponState.h"
#include "Game/Boss/Boss.h"
#include "Game/Boss/States/Header/BossIdling.h"

// --------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// --------------------------------------
BossIdling::BossIdling(Boss* boss)
	:
	m_boss(boss),
	m_angle{},
	m_totalSeconds{}
{
}

// --------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// --------------------------------------
BossIdling::~BossIdling()
{
}

// --------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// --------------------------------------
void BossIdling::Initialize()
{
	// プレイヤーの取得
	void* object = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = object ? static_cast<IObject*>(object) : nullptr;
}

// --------------------------------------
/// <summary>
/// 設定処理(in)
/// </summary>
// --------------------------------------
void BossIdling::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;

	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔を変更
	FaceState face = FaceState::Idling;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);

	// 回転を取得
	m_angle = m_boss->GetAngle();
}

// --------------------------------------
/// <summary>
/// 更新処理
/// </summary>
// --------------------------------------
void BossIdling::Update(const float& elapsedTime)
{
	// 経過時間を加算
	m_totalSeconds += elapsedTime;
	// アニメーションの更新
	UpdateAnimation();
	// 次のステートに遷移するかを検知
	CheckNextState();
}

// --------------------------------------
/// <summary>
/// 敵の挙動更新処理
/// </summary>
// --------------------------------------
void BossIdling::UpdateAnimation()
{
	// プレイヤーの座標を取得
	Vector3 playerPos = m_player->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// アングルを計算
	float targetAngle = Math::CalculationAngle(parentPos, playerPos);
	// 角度差を計算
	float angleDiff = targetAngle - m_angle;

	// 角度差が180度を超える場合、回転方向を反転
	if (angleDiff > DirectX::XM_PI)
	{
		angleDiff -= DirectX::XM_2PI;
	}
	else if (angleDiff < -DirectX::XM_PI)
	{
		angleDiff += DirectX::XM_2PI;
	}
	// 線形補間で最短距離で回転
	m_angle += angleDiff * LERP_RATE;
	// 角度を設定
	m_boss->SetAngle(m_angle);
}

// --------------------------------------
/// <summary>
/// 次のステートに遷移するかを検知
/// </summary>
// --------------------------------------
void BossIdling::CheckNextState()
{
	// プレイヤーの座標を取得
	Vector3 playerPos = m_player->GetPosition();
	Vector3 parentPos = m_boss->GetPosition();

	// １秒で行動を変更する
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// ランダムで行動を変更する
		int random = Math::RandomInt(0, TOTAL_RATE);
		// プレイヤーとの距離を計算
		float distance = Vector3::Distance(parentPos, playerPos);
		// 次のボスのステート
		BossState state = BossState::Idling;

		// 遠い距離の場合
		if (distance > FAR_DISTANCE)
		{
			// 追従
			if (random % 2 == 0) state = BossState::Approaching;
			// ダッシュ攻撃
			else state = BossState::DashAttacking;
		}
		// 近い距離の場合
		else
		{
			// 薙ぎ払い
			if (random <= SWEEPING_RATE) state = BossState::Sweeping;
			// 攻撃
			else if (random <= ATTACKING_RATE) state = BossState::Attacking;
			// ダッシュ攻撃
			else if (random <= DASH_ATTACK_RATE) state = BossState::DashAttacking;
			// 何もしない
			else if (random <= IDLING_RATE) state = BossState::Idling;
		}

		// ボスのステートを変更
		EventMessenger::Execute(EventList::ChangeBossState, &state);
	}
}

// --------------------------------------
/// <summary>
/// 設定処理(out)
/// </summary>
// --------------------------------------
void BossIdling::PostUpdate()
{
}

// --------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// --------------------------------------
void BossIdling::Finalize()
{
}