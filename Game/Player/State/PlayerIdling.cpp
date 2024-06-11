#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Player/Player.h"
#include "Game/Player/State/PlayerIdling.h"

// コンストラクタ
PlayerIdling::PlayerIdling(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
{
}

// デストラクタ
PlayerIdling::~PlayerIdling()
{
}


// 初期化処理
void PlayerIdling::Initialize()
{
}

// 事前更新処理
void PlayerIdling::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;
}

// 更新処理
void PlayerIdling::Update(const float& elapsedTime)
{
	// 子のステートになってからの時間を計測
	m_totalSeconds += elapsedTime;

	// 移行更新処理を書く
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

}


// 終了処理
void PlayerIdling::Finalize()
{
}

