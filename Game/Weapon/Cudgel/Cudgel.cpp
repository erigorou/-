// ----------------
//
// 鬼の金棒
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>
// ヘッダー
#include "Game/CommonResources.h"	
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "Game/GameResources.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Boss/Boss.h"

#include "header/Cudgel_Idling.h"		// 待機
#include "header/Cudgel_Attacking.h"	// 攻撃
#include "header/Cudgel_Sweeping.h"		// 薙ぎ払い

const float Cudgel::CUDGEL_SCALE = Boss::BOSS_SCALE * 1.2f;						// 金棒の拡大率
const DirectX::SimpleMath::Vector3 Cudgel::DIRECTION_ENEMY = { 8.0f, 5.0f, 0.0f };	// 持ち手の距離（敵と比較）
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_LENGTH =	{ 0.0f, 35.0f, 0.0f };	// 金棒の長さ（一番下から）
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_HADLE_POS = { 0.0f, 10.0f, 0.0f };	// 金棒の取っ手の部分（一番上）


// --------------------------------
// コンストラクタ
// --------------------------------
Cudgel::Cudgel(Boss* boss)
	:
	m_boss(boss),
	m_currentState(),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}


// --------------------------------
// デストラクタ
// --------------------------------
Cudgel::~Cudgel()
{
}


// --------------------------------
// 初期化処理
// --------------------------------
void Cudgel::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

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

	// 金棒のポインタを取得
	EventMessenger::AttachGetter(GetterList::GetCudgel, std::bind(&Cudgel::GetCudgelObject, this));
	// 金棒のステートを変更
	EventMessenger::Attach(EventList::ChangeCudgelState, std::bind(&Cudgel::ChangeState, this, std::placeholders::_1));

	// モデルを取得する
	m_model = GameResources::GetInstance()->GetModel("cudgel");

	CreateState();			// ステートの作成
	CreateCollision();		// 当たり判定の生成
}


// --------------------------------
// 状態の生成
// --------------------------------
void Cudgel::CreateState()
{
	// 状態の生成
	m_idling	= std::make_unique<Cudgel_Idling>	(this);		// 待機
	m_attacking = std::make_unique<Cudgel_Attacking>(this);		// 攻撃
	m_sweeping	= std::make_unique<Cudgel_Sweeping>	(this);		// 薙ぎ払い

	// 状態の初期化
	m_idling->		Initialize();	// 待機
	m_attacking->	Initialize();	// 攻撃
	m_sweeping->	Initialize();	// 薙ぎ払い

	// 初期状態を指定
	m_currentState = m_idling.get();

	// ステートのリストに追加
	m_states.push_back(m_idling.get());
	m_states.push_back(m_attacking.get());
	m_states.push_back(m_sweeping.get());
}


// --------------------------------
// 当たり判定の生成
// --------------------------------
void Cudgel::CreateCollision()
{
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);

	// 衝突データの作成
	CollisionData<DirectX::BoundingOrientedBox> data =
	{
		ObjectType::Cudgel,		// オブジェクトの種類
		CollisionType::OBB,		// 当たり判定の種類
		this,					// このクラスのポインタ
		m_collision.get()		// 当たり判定
	};

	// 当たり判定を記録する
	EventMessenger::Execute(EventList::AddOBBCollision, &data);
}


// --------------------------------
// 更新処理
// --------------------------------
void Cudgel::Update(float elapsedTime)
{
	// 現在のステートの更新処理
	m_currentState->Update(elapsedTime);
}

// --------------------------------
// 描画処理
// --------------------------------
void Cudgel::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルを描画
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

#ifdef _DEBUG
#endif // _DEBUG
}


// --------------------------------
// 終了処理
// --------------------------------
void Cudgel::Finalize()
{
}


// --------------------------------
// ステートの変更
// --------------------------------
void Cudgel::ChangeState(void* state)
{
	int index = *static_cast<int*>(state);

	// ステートの変更
	if (m_currentState == m_states[index]) return;

	// ステートの事後処理
	m_currentState->PostUpdate();
	// 新しいステートに切り替える
	m_currentState = m_states[index];
	// 新しいステートの事前処理を行う
	m_currentState->PreUpdate();
}



void Cudgel::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}