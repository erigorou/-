#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

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
	m_boundingSphereBody.Radius = 0.3f;
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
	// 左シフトの入力　&& 移動中なら
	if (keyboardState.LeftShift && m_player->GetVelocity() != DirectX::SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetPlayerDodgingState());
	}
	// Xの入力
	if (keyboardState.X)
	{
		m_player->ChangeState(m_player->GetPlayerAttackingState());
	}
	// 当たり判定の座標を更新
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

	//// デバッグ文字の描画
	//auto debugString = resources->GetDebugString();
	//debugString->AddString("PlayerIdling");
}


// 終了処理
void PlayerIdling::Finalize()
{
}


// 体同士の当たり判定
bool PlayerIdling::CheckBoundingSphereCollision()
{
	// 体の当たったかどうかのフラグ
	bool hit = false;

	auto player = dynamic_cast<Player*>(m_player);
	// ボディを取得						プレイヤー → シーン → エネミー → 現在のステート → 体の当たり判定
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// ボディ同士の当たり判定を実行する
	if (m_boundingSphereBody.Intersects(enemyBody))
	{
		// 当たってるならTrue
		hit = true;
	}

	// ヒットフラグの中身を返す
	return hit;
}


// 体同士で当たった場合の処理
void PlayerIdling::HitBody()
{
	using namespace DirectX::SimpleMath;

	// プレイヤーを一度変換する
	auto player = dynamic_cast<Player*>(m_player);
	// ボディを取得						プレイヤー → シーン → エネミー → 現在のステート → 体の当たり判定
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// 当たってない場合は早期リターン
	if (!m_boundingSphereBody.Intersects(enemyBody))	return;

	// 衝突判定　プレイヤーが押し戻される--[====================>

	// プレイヤーの中心と敵の中心との差分ベクトル
	Vector3 diffVec = m_boundingSphereBody.Center - enemyBody.Center;
	// プレイヤーの中心と敵の中心との距離を取得
	float diffLength = diffVec.Length();
	// プレイヤーと敵の半径の合計
	float sumLength = m_boundingSphereBody.Radius + enemyBody.Radius;
	//
}