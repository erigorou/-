// -----------------------------------------------------
// 名前:	BossSweeping.cpp
// 内容:	ボスの薙ぎ払い攻撃ステートクラス
// 作成:	池田桜輔
// -----------------------------------------------------
// インクルード
#include "pch.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Weapon/WeaponState.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Boss/Boss.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Boss/States/Header/BossSweeping.h"

// ----------------------------------
// コンストラクタ
// ----------------------------------
BossSweeping::BossSweeping(Boss* Boss)
	:
	m_angle{},
	m_boss(Boss),
	m_totalSeconds{},
	m_currentAction{}
{
}

// ----------------------------------
// デストラクタ
// ----------------------------------
BossSweeping::~BossSweeping()
{
}

// ----------------------------------
// 初期化処理
// ----------------------------------
void BossSweeping::Initialize()
{
	// アクションの登録処理を行う
	RegistoryAction();
}


// ----------------------------------
/// <summary>
/// アクションの登録処理
/// </summary>
// ----------------------------------
void BossSweeping::RegistoryAction()
{
	m_actions = std::vector<std::function<bool()>>{
		std::bind(&BossSweeping::UpdateChargeMotion,this),
		std::bind(&BossSweeping::UpdateSweepMotion,	this),
		std::bind(&BossSweeping::UpdateIdleMotion,	this)
	};
}


// ----------------------------------
// 事前更新処理
// ----------------------------------
void BossSweeping::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	m_angle = DirectX::XMConvertToDegrees(m_boss->GetAngle());

	// 武器のステートを変更
	CudgelState state = CudgelState::Sweep;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔のステートを変更
	FaceState face = FaceState::Attacking;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);

	// アクションをリセットする
	m_currentAction = 0;
}

// ----------------------------------
// 更新処理
// ----------------------------------
void BossSweeping::Update(const float& elapsedTime)
{
	// 経過時間の加算
	m_totalSeconds += elapsedTime;
	// アニメーションの更新
	UpdateAnimation();
	// 角度の更新
	m_boss->SetAngle(DirectX::XMConvertToRadians(m_targetAngle));
}


// ----------------------------------
// アニメーションの更新
// ----------------------------------
void BossSweeping::UpdateAnimation()
{
	// ノードが最終まで到達している場合は終了処理を行う
	if (m_currentAction >= m_actions.size())
	{
		// ステートを変更（待機状態）
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);
		return;
	}

	// アクションを実行する
	if (m_actions[m_currentAction]())
	{
		m_currentAction++;
	}
}

// ----------------------------------
// ためモーションの更新
// ----------------------------------
bool BossSweeping::UpdateChargeMotion()
{
	// ためる時間に対する経過時間の割合
	float easing = m_totalSeconds / CHARGE_TIME;
	// イージングを用いた角度の計算
	m_targetAngle = m_angle - ROTATE_ANGLE * Easing::easeOutCirc(easing);

	return m_totalSeconds >= CHARGE_TIME;
}

// ----------------------------------
// 薙ぎ払いモーションの更新
// ----------------------------------
bool BossSweeping::UpdateSweepMotion()
{
	// 薙ぎ払いモーションの経過時間に対する割合
	float easing = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
	// イージングを用いた角度の計算
	m_targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * Easing::easeOutBack(easing);

	return m_totalSeconds >= ATTACK_TIME;
}


// ----------------------------------
// 後隙モーションの更新
// ----------------------------------
bool BossSweeping::UpdateIdleMotion()
{
	return m_totalSeconds >= END_TIME;
}

// ----------------------------------
// 事後更新処理
// ----------------------------------
void BossSweeping::PostUpdate()
{
	// 武器のステートを変更
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// 顔のステートを変更
	FaceState face = FaceState::Idling;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}

// ----------------------------------
// 終了処理
// ----------------------------------
void BossSweeping::Finalize()
{
}