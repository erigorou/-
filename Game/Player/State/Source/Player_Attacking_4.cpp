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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/State/Header/Player_Attacking_4.h"
 

// --------------------------------
//  コンストラクタ
// --------------------------------
PlayerAttacking_4::PlayerAttacking_4(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_attackElapsedTime(0.0f)
	,m_model()
{
}

// --------------------------------
//  デストラクタ
// --------------------------------
PlayerAttacking_4::~PlayerAttacking_4()
{
}

// --------------------------------
//  初期化処理
// --------------------------------
void PlayerAttacking_4::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
}

// --------------------------------
//  状態開始処理
// --------------------------------
void PlayerAttacking_4::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;


	// 剣の攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetAttacking_4State()
	);

	Sound::PlaySE(Sound::SE_TYPE::PLAYER_ATTACK2);
}

// --------------------------------
//  更新処理
// --------------------------------
void PlayerAttacking_4::Update(const float& elapsedTime)
{
	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// --------------------------------
//  キー入力処理（瞬間）
// --------------------------------
void PlayerAttacking_4::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// --------------------------------
//  キー入力処理（継続）
// --------------------------------
void PlayerAttacking_4::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// --------------------------------
//  状態終了処理
// --------------------------------
void PlayerAttacking_4::PostUpdate()
{
	// 武器を攻撃状態に変更
	m_player->GetPlayScene()->GetSword()->ChangeState(
		m_player->GetPlayScene()->GetSword()->GetIdlingState()
	);
}


// --------------------------------
//  描画処理
// --------------------------------
void PlayerAttacking_4::Render(
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


// --------------------------------
//  終了処理
// --------------------------------
void PlayerAttacking_4::Finalize()
{
}