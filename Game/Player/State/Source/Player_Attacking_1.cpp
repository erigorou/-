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
#include "Game/Weapon/Sword/Sword.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/State/Header/Player_Attacking_1.h"



// コンストラクタ
PlayerAttacking_1::PlayerAttacking_1(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_attackElapsedTime()
	,m_model()
{
}

// デストラクタ
PlayerAttacking_1::~PlayerAttacking_1()
{
}


// 初期化処理
void PlayerAttacking_1::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;

}

// 事前更新処理
void PlayerAttacking_1::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;
	
	// 武器を攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_1State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK);
}

// 更新処理
void PlayerAttacking_1::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	UpdateAnimation();

	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// アニメーションの更新
void PlayerAttacking_1::UpdateAnimation()
{
	if (m_totalSeconds > Player::NORMAL_ATTACK_TIME) return;

	// イージングで使用するための変数 0-1
	float t = m_totalSeconds / Player::NORMAL_ATTACK_TIME;

	// プレイヤーに回転を与える
	float currentAngle = m_player->GetAngle();
	currentAngle += -30 + 60 * Easying::easeOutExpo(t);

	m_player->SetAnimationAngle(currentAngle);
}


// キー入力
void PlayerAttacking_1::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X			&& m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerAttackingState2());
	if (key == DirectX::Keyboard::LeftShift	&& m_totalSeconds >= Player::X_COOL_TIME)	m_player->ChangeState(m_player->GetPlayerDodgingState());
}

void PlayerAttacking_1::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}



// 事後更新処理
void PlayerAttacking_1::PostUpdate()
{

	// 武器を攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);
}


// 描画処理
void PlayerAttacking_1::Render(
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

#ifdef _DEBUG
	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();
	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // _DEBUG
}


// 終了処理
void PlayerAttacking_1::Finalize()
{
}
