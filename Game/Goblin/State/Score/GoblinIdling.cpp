// --------------------------------------------------
//名前:	GoblinIdling.cpp
//内容:	ゴブリンの待機状態
//作成:	池田桜輔
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "../Header/GoblinIdling.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="goblin">ゴブリンのポインタ</param>
// ---------------------------------------------
GoblinIdling::GoblinIdling(Goblin* goblin)
	: 
	m_goblin{ goblin },
	m_TotalTime{}
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
GoblinIdling::~GoblinIdling()
{
}

// ---------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------
void GoblinIdling::Initialize()
{
	// ランダムにクールタイムを設定する
	m_coolTime = INIT_COOLTIME;
}

// ---------------------------------------------
/// <summary>
/// 初期化処理（in処理）
/// </summary>
// ---------------------------------------------
void GoblinIdling::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// ランダムにクールタイムを設定する
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void GoblinIdling::Update(const float& elapsedTime)
{
	// プレイヤーの方向を向く
	SearchPlayer();

	// 時間を加算する
	m_TotalTime += elapsedTime;

	// 攻撃ステートに遷移する
	if (m_TotalTime > m_coolTime)
	{
		m_goblin->ChangeState(GoblinState::ATTACKING);
	}
}

// ---------------------------------------------
/// <summary>
/// 初期化処理（out処理）
/// </summary>
// ---------------------------------------------
void GoblinIdling::PostUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void GoblinIdling::Finalize()
{
}


// ---------------------------------------------
/// <summary>
/// プレイヤーの探索処理（ゴブリンの回転にLerpを追加）
/// </summary>
// ---------------------------------------------
void GoblinIdling::SearchPlayer()
{
	// プレイヤーを取得
	auto object = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	auto player = static_cast<IObject*>(object);
	// プレイヤーの位置を取得
	DirectX::SimpleMath::Vector3 playerPos = player->GetPosition();
	// 小鬼の位置を取得
	m_position = m_goblin->GetPosition();

	// プレイヤーの方向を計算
	float targetAngle = Math::CalculationAngle(playerPos, m_position);

	// 現在の回転角度を補間して目標角度に徐々に近づける
	m_angle = Math::LerpFloat(m_angle, targetAngle, 0.1f); // 0.1fは補間率（調整可能）

	// 回転行列を更新
	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
	m_goblin->SetAngle(-m_angle);
}
