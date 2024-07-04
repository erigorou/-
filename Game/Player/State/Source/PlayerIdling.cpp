#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
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
	// 体の当たり判定の生成
	m_boundingSphereBody = DirectX::BoundingSphere();
	// 体の当たり判定のサイズや座標を設定
	m_boundingSphereBody.Radius = Player::PLAYER_SCALE * 12.f;
}


// 事前更新処理
void PlayerIdling::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;
	// 当たり判定をプレイヤーの位置に修正
	m_boundingSphereBody.Center = m_player->GetPosition();
}


// 更新処理
void PlayerIdling::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime使わないけどエラー出さないでねって文
	UNREFERENCED_PARAMETER(elapsedTime);

	// キーボードの入力を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// プレイヤーの移動
	m_player->MovePlayer();

	// 左シフトの入力　&& 移動中なら
	if (m_keyboardState.LeftShift && m_player->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
	// Xの入力
	if (m_keyboardState.X)
	{
		m_player->ChangeState(m_player->GetPlayerAttackingState1());
	}


	// 埋め込み量の計算をした後にそれを反映させる
	parentPos += CalculatingPushBack();
	m_boundingSphereBody.Center = parentPos;
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
}


// 終了処理
void PlayerIdling::Finalize()
{
}



// 体に当たったときに押し戻しをする
DirectX::SimpleMath::Vector3 PlayerIdling::CalculatingPushBack()
{
	// プレイヤーを一度変換する
	auto player = dynamic_cast<Player*>(m_player);
	// ボディを取得						プレイヤー → シーン → エネミー → 現在のステート → 体の当たり判定
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// 押し戻し量の計測
	return Math::pushBack_BoundingSphere(m_boundingSphereBody, enemyBody);
}