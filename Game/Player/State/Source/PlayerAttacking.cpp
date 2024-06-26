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

#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerAttacking.h"

// アタックする時間
const float PlayerAttacking::ATTACKING_TIME = 1.0f;

// コンストラクタ
PlayerAttacking::PlayerAttacking(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
{
}

// デストラクタ
PlayerAttacking::~PlayerAttacking()
{
}


// 初期化処理
void PlayerAttacking::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
}

// 事前更新処理
void PlayerAttacking::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;
}

// 更新処理
void PlayerAttacking::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// parentPos使わないけどエラー出さないでねって文
	UNREFERENCED_PARAMETER(parentPos);
	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, ATTACKING_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// 事後更新処理
void PlayerAttacking::PostUpdate()
{
	// 修正点があればここに記載
}


// 描画処理
void PlayerAttacking::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();
}


// 終了処理
void PlayerAttacking::Finalize()
{
}
