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

	DirectX::SimpleMath::Vector3 pos = m_player->GetPosition();
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

	// ビヘイビアツリーの実行
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// ボスの行動を更新
		m_boss->RunBehaviorTree();
	}
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
	// プレイヤーの削除
	m_player = nullptr;
}