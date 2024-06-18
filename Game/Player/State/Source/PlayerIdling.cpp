#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerIdling.h"

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
	m_totalSeconds = 0.f;
}

// 更新処理
void PlayerIdling::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime使わないけどエラー出さないでねって文
	UNREFERENCED_PARAMETER(elapsedTime);

	// キー入力を受け付ける。
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	// プレイヤーの移動
	m_player->MovePlayer();

	// 左シフトで回避
	if (keyboardState.LeftShift)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
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
	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();


	// デバッグ文字の描画
	auto debugString = resources->GetDebugString();
	debugString->AddString("PlayerIdling");
}


// 終了処理
void PlayerIdling::Finalize()
{
}

