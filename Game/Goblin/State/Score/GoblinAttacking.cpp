#include "pch.h"
#include "../Header/GoblinAttacking.h"
#include "../../Goblin.h"
#include "Game/Player/Player.h"
#include "Game/Messenger/EventMessenger.h"

#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/DebugString.h"

// -------------------------------
// コンストラクタ
// -------------------------------
GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
	, m_angle(0.0f)
	, m_rotMatrix(DirectX::SimpleMath::Matrix::Identity)
	, m_moveValue()
	, m_position()
{
}

// -------------------------------
// 終了処理
// -------------------------------
GoblinAttacking::~GoblinAttacking()
{
}

// -------------------------------
// 初期化処理
// -------------------------------
void GoblinAttacking::Initialize()
{
}

// -------------------------------
// 事前更新
// -------------------------------
void GoblinAttacking::PreUpdate()
{
	// プレイヤーの方向を向く
	SearchPlayer();
	// 経過時間をリセットする
	m_totalTime = 0.0f;
}

// -------------------------------
// 更新処理
// -------------------------------
void GoblinAttacking::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;

	// アニメーションの更新
	UpdateAnimation();

	if (m_totalTime > STATE_TIME)
	{
		m_goblin->ChangeState(GoblinState::IDLING);
	}
}

// -------------------------------
// アニメーションの更新処理
// -------------------------------
void GoblinAttacking::UpdateAnimation()
{
	// 攻撃中フラグをおろす
	m_goblin->SetIsAttacking(false);

	ChargeAnimation();
	AttackAnimation();
	ReturnAnimation();
}

// --------------------------------
// 貯めアニメーション
// --------------------------------
void GoblinAttacking::ChargeAnimation()
{
	// 時間内出ない場合は終了
	if (!Math::InTime(0, m_totalTime, CHARGE_TIME)) return;

	// 秒数を正規化
	float t = m_totalTime / CHARGE_TIME;

	// 大きさyをsin波で変更
	float sin = Math::NormalizeSin(t);
	float sizeY = 1.0f - (sin * MAX_MINUS_SIZE);

	// 大きさを設定
	m_goblin->SetScale(DirectX::SimpleMath::Vector3(1.0f, sizeY, 1.0f));
}

// -------------------------------
// 攻撃アニメーション
// -------------------------------
void GoblinAttacking::AttackAnimation()
{
	// 攻撃時間を過ぎたら終了
	if (!Math::InTime(CHARGE_TIME, m_totalTime, ATTACK_TIME)) return;

	// 攻撃中フラグを立てる
	m_goblin->SetIsAttacking(true);

	// 秒数を正規化
	float t = (m_totalTime - CHARGE_TIME) / (ATTACK_TIME - CHARGE_TIME);

	// sinとイージングを掛けて上下移動を実装
	float sin = Math::NormalizeSin(t);
	float easingUD = std::fabs(1 - Easing::easeOutBack(t));
	float posY = sin * easingUD * MAX_Y_POS;

	// 横軸の移動
	float easingLR = Easing::easeOutCubic(t);

	// 移動位置を設定
	m_movePosition = m_position + m_moveValue * easingLR;
	m_movePosition.y = posY;

	// 移動量を設定
	m_goblin->SetPosition(m_movePosition);
}

// -------------------------------
// 元に戻るアニメーション
// -------------------------------
void GoblinAttacking::ReturnAnimation()
{
	if (!Math::InTime(ATTACK_TIME, m_totalTime, RETURN_TIME))return;
}

// -------------------------------
// 事後更新
// -------------------------------
void GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// -------------------------------
// 終了処理
// -------------------------------
void GoblinAttacking::Finalize()
{
}

// -------------------------------
// プレイヤーの探索を行う処理
// -------------------------------
void GoblinAttacking::SearchPlayer()
{
	// プレイヤーを取得
	auto object = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	auto player = static_cast<IObject*>(object);
	// プレイヤーの位置を取得
	DirectX::SimpleMath::Vector3 playerPos = player->GetPosition();
	// 小鬼の位置を取得
	m_position = m_goblin->GetPosition();

	// プレイヤーの位置を探索
	m_angle = Math::CalculationAngle(playerPos, m_position);
	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
	m_goblin->SetAngle(-m_angle);

	// ゴブリンとプレイヤーの距離を取得
	m_moveValue = playerPos - m_position;
}