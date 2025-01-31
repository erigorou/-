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


// 固定値
const float Sword::SWORD_SCALE = Player::PLAYER_SCALE * 1.5f;


// コンストラクタ
Sword::Sword(Player* player)
	:
	m_player(player),
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity },
	m_collision{},
	m_originalBox{},
	m_canAttack{false}
{
}


// デストラクタ
Sword::~Sword()
{
}


// 初期化処理
void Sword::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();


	// モデルの読み込み
	m_model = GameResources::GetInstance()->GetModel("sword");

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);
	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

	// ステートを作成
	CreateState();
	// 当たり判定を作成
	CreateCollision();

	// ワールド座標の初期化
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(SWORD_SCALE);

}


// シーンを生成する
void Sword::CreateState()
{
	// ステートを生成する
	m_swordIdling		= std::make_unique<Sword_Idling>(this);
	m_swordAttacking_1	= std::make_unique<Sword_Attacking_1>(this);
	m_swordAttacking_2	= std::make_unique<Sword_Attacking_2>(this);

	// ステートを初期化する
	m_swordIdling->Initialize();
	m_swordAttacking_1->Initialize();
	m_swordAttacking_2->Initialize();

	// 現在のステートを設定
	m_currentState = m_swordIdling.get();

	// 待機ステートを格納
	m_states.push_back(m_swordIdling.get());
	// 攻撃ステートを格納
	m_states.push_back(m_swordAttacking_1.get());
	// 攻撃ステートを格納
	m_states.push_back(m_swordAttacking_2.get());
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
	EventMessenger::Execute("AddOrientedCollision", &data);
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
