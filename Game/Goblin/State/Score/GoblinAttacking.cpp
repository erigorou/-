// --------------------------------------------------
//名前:	GoblinAttacking.cpp
//内容:	ゴブリンの攻撃状態
//作成:	池田桜輔
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "../Header/GoblinAttacking.h"
#include "../../Goblin.h"
#include "Game/Messenger/EventMessenger.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="goblin">ゴブリンのポインタ</param>
// ---------------------------------------------
GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin{ goblin }
	, m_totalTime{}
	, m_angle{}
	, m_rotMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_moveValue{}
	, m_position{}
	, m_currentAction{}
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
GoblinAttacking::~GoblinAttacking()
{
}

// ---------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------
void GoblinAttacking::Initialize()
{
	// アクションを登録する
	RegistoryAction();
}

// ---------------------------------------------
/// <summary>
/// 事前更新処理
/// </summary>
// ---------------------------------------------
void GoblinAttacking::PreUpdate()
{
	// プレイヤーの方向を向く
	SearchPlayer();
	// 経過時間をリセットする
	m_totalTime = 0.0f;

	// 攻撃インデックスをリセットする
	m_currentAction = 0;
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void GoblinAttacking::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;

	// アニメーションの更新
	UpdateAnimation();
}

// ---------------------------------------------
/// <summary>
/// 貯めアニメーション処理
/// </summary>
// ---------------------------------------------
void GoblinAttacking::UpdateAnimation()
{
	// 攻撃中フラグをおろす
	m_goblin->SetIsAttacking(false);

	// アクションが最後の場合は終了する
	if (m_currentAction >= m_actions.size())
	{
		m_goblin->ChangeState(GoblinState::IDLING);
		return;
	}

	// アクションを実行する
	if (m_actions[m_currentAction]())
	{
		// 次のアクションに移行する
		m_currentAction++;
	}
}

// ---------------------------------------------
/// <summary>
/// 攻撃アニメーション処理
/// </summary>
// ---------------------------------------------
bool GoblinAttacking::ChargeAnimation()
{
	// 秒数を正規化
	float t = m_totalTime / CHARGE_TIME;

	bool flag = false;
	EventMessenger::Execute(EventList::PlayerCanDamageGoblin, &flag);

	// 大きさyをsin波で変更
	float sin = Math::NormalizeSin(t);
	float sizeY = 1.0f - (sin * MAX_MINUS_SIZE);

	// 大きさを設定
	m_goblin->SetScale(DirectX::SimpleMath::Vector3(1.0f, sizeY, 1.0f));

	// プレイヤーの方向を向く
	SearchPlayer();

	// 評価する
	return m_totalTime >= CHARGE_TIME;
}

// ---------------------------------------------
/// <summary>
/// 攻撃アニメーション処理
/// </summary>
// ---------------------------------------------
bool GoblinAttacking::AttackAnimation()
{
	bool flag = true;
	EventMessenger::Execute(EventList::PlayerCanDamageGoblin, &flag);

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

	// 評価する
	return m_totalTime >= ATTACK_TIME;
}

// ---------------------------------------------
/// <summary>
/// 元に戻るアニメーション処理
/// </summary>
// ---------------------------------------------
bool GoblinAttacking::ReturnAnimation()
{
	return m_totalTime >= RETURN_TIME;
}

// ---------------------------------------------
/// <summary>
/// 事後更新処理
/// </summary>
// ---------------------------------------------
void GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// ---------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------
void GoblinAttacking::Finalize()
{
}

// ---------------------------------------------
/// <summary>
/// アクションを登録する
/// </summary>
// -//------------------------------------------
void GoblinAttacking::RegistoryAction()
{
	// アクションを登録する
	m_actions = std::vector<std::function<bool()>>{
		std::bind(&GoblinAttacking::ChargeAnimation, this),
		std::bind(&GoblinAttacking::AttackAnimation, this),
		std::bind(&GoblinAttacking::ReturnAnimation, this)
	};

	// 最初のアクションを最初にする
	m_currentAction = 0;
}

// ---------------------------------------------
/// <summary>
/// プレイヤーの探索処理
/// </summary>
// ---------------------------------------------
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