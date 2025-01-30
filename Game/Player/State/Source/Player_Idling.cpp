#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"

#include "Game/Player/State/Header/Player_Idling.h"


// -------------------------------
// コンストラクタ
// -------------------------------
PlayerIdling::PlayerIdling(Player* player)
	: m_player(player)
	, m_totalSeconds(0.0f)
{
}


// -------------------------------
// デストラクタ
// -------------------------------
PlayerIdling::~PlayerIdling()
{
}


// -------------------------------
// 初期化処理
// -------------------------------
void PlayerIdling::Initialize()
{
}


// -------------------------------
// 変更処理（in）
// -------------------------------
void PlayerIdling::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;

	// 移動速度の初期化
	m_player->SetSpeed(DirectX::SimpleMath::Vector3::Zero);
	// 移動加速度の初期化
	m_player->SetAcceleration(DirectX::SimpleMath::Vector3::Zero);
}


// -------------------------------
// 更新処理
// -------------------------------
void PlayerIdling::Update(const float& elapsedTime)
{
	// 経過時間の加算
	m_totalSeconds += elapsedTime;

	// プレイヤーの向きを敵に向ける
	DirectX::SimpleMath::Vector3 enemyPos = m_player->GetPlayScene()->GetTargetPosition();
	m_player->SetAngle(m_player->CalucratePlayerRotation(enemyPos));

	// プレイヤーの移動
	m_player->MovePlayer();
}


// -------------------------------
// 変更処理（out）
// -------------------------------
void PlayerIdling::PostUpdate()
{
}



// -------------------------------
// キーボード入力(押した瞬間)
// -------------------------------
void PlayerIdling::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	if (key == DirectX::Keyboard::X			)	m_player->ChangeState(m_player->GetPlayerAttackingState1());
	if (key == DirectX::Keyboard::LeftShift	)	m_player->ChangeState(m_player->GetPlayerDodgingState());
}


// -------------------------------
// キーボード入力(押してる間)
// -------------------------------
void PlayerIdling::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}



// -------------------------------
// 終了処理
// -------------------------------
void PlayerIdling::Finalize()
{
}
