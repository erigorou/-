#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"

#include "Game/Player/State/Header/Player_Idling.h"

// コンストラクタ
PlayerIdling::PlayerIdling(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
{
}


// デストラクタ
PlayerIdling::~PlayerIdling()
{
}


// 初期化処理
void PlayerIdling::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
}


// 事前更新処理
void PlayerIdling::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;

	m_player->SetSpeed(DirectX::SimpleMath::Vector3::Zero);
	m_player->SetAcceleration(DirectX::SimpleMath::Vector3::Zero);
}


// 更新処理
void PlayerIdling::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime使わないけどエラー出さないでねって文
	UNREFERENCED_PARAMETER(elapsedTime);

	// キーボードの入力を取得する
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	// プレイヤーの移動
	m_player->MovePlayer();

	// 左シフトの入力　&& 移動中なら
	if (keyboardState.LeftShift && m_player->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
	// Xの入力
	if (keyboardState.X) 
	{
		m_player->ChangeState(m_player->GetPlayerAttackingState1());
	}
}



// 事後更新処理
void PlayerIdling::PostUpdate()
{
	// 修正点があればここに記載
}


// 描画処理
void PlayerIdling::Render(
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

	//// コモンリソースを取得する
	//CommonResources* resources = CommonResources::GetInstance();
}


// 終了処理
void PlayerIdling::Finalize()
{
}
