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
#include "Game/Player/State/Header/PlayerAttacking_1.h"


// コンストラクタ
PlayerAttacking_1::PlayerAttacking_1(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_model()
	,m_boundingSphereBody()
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
	// 体の境界球を作成
	m_boundingSphereBody = DirectX::BoundingSphere();
	m_boundingSphereBody.Radius = Player::PLAYER_SCALE * 12.f;

}

// 事前更新処理
void PlayerAttacking_1::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;
}

// 更新処理
void PlayerAttacking_1::Update(const float& elapsedTime,  DirectX::SimpleMath::Vector3& parentPos)
{
	// parentPos使わないけどエラー出さないでねって文
	UNREFERENCED_PARAMETER(parentPos);

	// キーボードのトラッカー
	DirectX::Keyboard::KeyboardStateTracker tracker = m_player->GetKeyboardTracker();

	// １回目の攻撃中に攻撃ボタンを押す
	if (tracker.IsKeyPressed(DirectX::Keyboard::X))
	{
		// ２回目の攻撃にステートを変更する
		m_player->ChangeState(m_player->GetPlayerAttackingState2());
	}

	// 時間を計測し、一定時間経過でステートを遷移
	m_player->TimeComparison(m_totalSeconds, Player::APPLIED_ATTACK_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
	// 体の境界球の位置を更新
	m_boundingSphereBody.Center = parentPos;
}


// 事後更新処理
void PlayerAttacking_1::PostUpdate()
{
	// 修正点があればここに記載
}


// 描画処理
void PlayerAttacking_1::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context, states, view, projection, m_model);
	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();


	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("Attack 1");
}


// 終了処理
void PlayerAttacking_1::Finalize()
{
}
