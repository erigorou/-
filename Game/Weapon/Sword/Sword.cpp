// ----------------
//
// 桃太郎の刀
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>
// ヘッダー
#include "Game/CommonResources.h"	
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"
#include "Sword.h"
#include "Game/Player/Player.h"

#include "Game/Weapon/Sword/Header/SwordIdling.h"
#include "Game/Weapon/Sword/Header/SwordAttacking1.h"
#include "Game/Weapon/Sword/Header/SwordAttacking2.h"



// --------------------------------------------
// 固定値
// --------------------------------------------
// 刀の大きさ
const float Sword::SWORD_SCALE = Player::PLAYER_SCALE * 1.5f;


// --------------------------------------------
// コンストラクタ
// --------------------------------------------
Sword::Sword(Player* player)
	:
	m_player(player),
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity },
	m_collision{},
	m_originalBox{},
	m_canAttack{false}
{
}


// --------------------------------------------
// デストラクタ
// --------------------------------------------
Sword::~Sword()
{
}


// --------------------------------------------
// 初期化処理
// --------------------------------------------
void Sword::Initialize()
{
	// モデルの読み込み
	m_model = GameResources::GetInstance()->GetModel("sword");
	// ステートを作成
	CreateState();
	// 当たり判定を作成
	CreateCollision();
}


// --------------------------------------------
// シーンを生成する
// --------------------------------------------
void Sword::CreateState()
{
	// 待機ステートを生成
	m_idling = std::make_unique<SwordIdling>(this);
	// 初期化処理
	m_idling->Initialize();
	// 待機ステートを格納
	m_states.push_back(m_idling.get());

	// 攻撃ステートを生成
	m_attacking1 = std::make_unique<SwordAttacking1>(this);
	// 初期化処理
	m_attacking1->Initialize();
	// 攻撃ステートを格納
	m_states.push_back(m_attacking1.get());

	// 攻撃ステートを生成
	m_attacking2 = std::make_unique<SwordAttacking2>(this);
	// 初期化処理
	m_attacking2->Initialize();
	// 攻撃ステートを格納
	m_states.push_back(m_attacking2.get());

	// 現在のステートを設定
	m_currentState = m_idling.get();
}


// --------------------------------------------
// 衝突判定の生成
// --------------------------------------------
void Sword::CreateCollision()
{
	// モデルの大きさから衝突判定を取得する
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);

	// 衝突データの作成
	CollisionData<DirectX::BoundingOrientedBox> data =
	{
		ObjectType::Sword,		// オブジェクトの種類
		CollisionType::OBB,		// 当たり判定の種類
		this,					// このクラスのポインタ
		m_collision.get()		// 当たり判定
	};

	// 衝突判定をManagerに登録
	EventMessenger::Execute(EventList::AddOBBCollision, &data);
}


// --------------------------------------------
// 更新処理
// --------------------------------------------
void Sword::Update(float elapsedTime)
{
	// 現在のステートの更新処理
	m_currentState->Update(elapsedTime);
}


// --------------------------------------------
// 描画処理
// --------------------------------------------
void Sword::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// --------------------------------------------
// 終了処理
// --------------------------------------------
void Sword::Finalize()
{
}


// --------------------------------------------
// ステートの変更
// --------------------------------------------
void Sword::ChangeState(void* state)
{
	// indexを取得
	int index = *static_cast<int*>(state);
	// 現在と同じステートには変更しない
	if (m_currentState == m_states[index])return;
	// ステートの事後処理
	m_currentState->PostUpdate();
	// 新しいステートに切り替える
	m_currentState = m_states[index];
	// 新しいステートの事前処理を行う
	m_currentState->PreUpdate();
}


// --------------------------------------------
// 当たったときの処理
// --------------------------------------------
void Sword::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}
