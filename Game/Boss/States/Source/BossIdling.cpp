#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Weapon/WeaponState.h"

// 関数を使用するのに必要な引数
#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// ヘッダーファイル
#include "Game/Boss/States/Header/BossIdling.h"

// --------------------------
// コンストラクタ
// --------------------------
BossIdling::BossIdling(Boss* boss)
	:
	m_boss(boss),
	m_angle{},
	m_totalSeconds{}
{
}


// --------------------------
// デストラクタ
// --------------------------
BossIdling::~BossIdling()
{
}


// --------------------------
// 初期化処理
// --------------------------
void BossIdling::Initialize()
{
	// プレイヤーの取得
	void* object = EventMessenger::ExecuteGetter("GetPlayerObject");
	m_player = object ? static_cast<IObject*>(object) : nullptr;
}


// --------------------------
// 設定処理(in)
// --------------------------
void BossIdling::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;

	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute("ChangeCudgelState", &state);

	// 顔を変更
	FaceState face = FaceState::Idling;
	EventMessenger::Execute("ChangeBossFace", &face);

	// 回転を取得
	m_angle = m_boss->GetAngle();
}


// --------------------------
// 更新処理
// --------------------------
void BossIdling::Update(const float& elapsedTime)
{
	// 経過時間を加算
	m_totalSeconds += elapsedTime;
	// アニメーションの更新
	UpdateAnimation();
	// 次のステートに遷移するかを検知
	CheckNextState();
}


// --------------------------
// 敵の挙動更新処理
// --------------------------
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



// ----------------------------------
// 次のステートに遷移するかを検知
// ----------------------------------
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
		EventMessenger::Execute("ChangeBossState", &state);
	}
}



// --------------------------
// 設定処理(out)
// --------------------------
void BossIdling::PostUpdate()
{
}


// ---------------------------
// 終了処理
// ---------------------------
void BossIdling::Finalize()
{
}