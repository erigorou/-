/// ---------------------------------------
///
/// プレイヤーが攻撃をする処理
///
/// ---------------------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Libraries/MyLib/EasingFunctions.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/State/Header/Player_Attacking_3.h"


// コンストラクタ
PlayerAttacking_3::PlayerAttacking_3(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
{
}

// デストラクタ
PlayerAttacking_3::~PlayerAttacking_3()
{
}


// 初期化処理
void PlayerAttacking_3::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
}

// 事前更新処理
void PlayerAttacking_3::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;

	// 剣の攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_3State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);

	m_position = m_player->GetPosition();
}

// 更新処理
void PlayerAttacking_3::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// アニメーションの更新
	UpdateAnimation();


	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// アニメーションの更新
void PlayerAttacking_3::UpdateAnimation()
{
	float t = 0;

	// 切り上げモーション中
	if (m_totalSeconds < Player::NORMAL_ATTACK_TIME)
	{
		DirectX::SimpleMath::Vector3 currentAnimPos = DirectX::SimpleMath::Vector3::Zero;

		// イージングで使用するための変数 0-1
		t = m_totalSeconds / Player::NORMAL_ATTACK_TIME;

		// 回転量の計算を行う
		float currentAngle = m_player->GetAngle();
		currentAnimPos.x = -20 * Easing::easeOutElastic(t) + currentAngle;

		// radianに変換
		currentAnimPos.x = DirectX::XMConvertToRadians(currentAnimPos.x);

		// プレイヤーに設定する
		m_player->SetAnimationRotate(currentAnimPos);
	}


	// イージングで使用するための変数 0 - 1
	t = m_totalSeconds / Player::APPLIED_ATTACK_TIME;

	float JUMP_VALUE = 6.0f;

	// イージングの経過時間をサイン波として扱う
	t = Math::CalculatingSinWave(t / 2, 1.0f, 1.0f);

	// 0以下にならないようにイージングを作る
	float jump = std::max(JUMP_VALUE * Easing::easeOutCubic(t), 0.0f);

	// プレイヤーに設定する
	m_player->SetPosition(m_position + DirectX::SimpleMath::Vector3(0, jump, 0));
}



// キー入力
void PlayerAttacking_3::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X			&& m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerAttackingState4());
}



void PlayerAttacking_3::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// 事後更新処理
void PlayerAttacking_3::PostUpdate()
{
	// 武器を攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);

	// アニメーションを初期化
	m_player->SetAnimationRotate(DirectX::SimpleMath::Vector3::Zero);
}


// 描画処理
void PlayerAttacking_3::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);



	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();


	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
}


// 終了処理
void PlayerAttacking_3::Finalize()
{
}
