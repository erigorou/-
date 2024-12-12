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
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Collision.h"
#include "Sword.h"
#include "Game/Player/Player.h"


// 固定値
const float Sword::SWORD_SCALE = Player::PLAYER_SCALE * 1.5f;


// コンストラクタ
Sword::Sword(PlayScene* playScene)
	:
	  m_playScene(playScene)
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

	// モデルを生成
	m_model = std::make_unique<DirectX::Model>();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Weapon/Sword/sword.cmo", *fx);


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

	CreateCollision();

	// ワールド座標の初期化
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(SWORD_SCALE);

}


// シーンを生成する
void Sword::CreateState()
{
	// ステートを生成する
	m_swordIdling		= std::make_unique<Sword_Idling>		(this);
	m_swordAttacking_1	= std::make_unique<Sword_Attacking_1>	(this);
	m_swordAttacking_2	= std::make_unique<Sword_Attacking_2>	(this);
	m_swordAttacking_3	= std::make_unique<Sword_Attacking_3>	(this);
	m_swordAttacking_4	= std::make_unique<Sword_Attacking_4>	(this);


	// ステートを初期化する
	m_swordIdling->Initialize();
	m_swordAttacking_1->Initialize();
	m_swordAttacking_2->Initialize();
	m_swordAttacking_3->Initialize();
	m_swordAttacking_4->Initialize();


	// 現在のステートを設定
	m_currentState = m_swordIdling.get();
}


/// <summary>
/// 当たり判定の生成
/// </summary>
void Sword::CreateCollision()
{
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model.get());
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);

	// 当たり判定を記録する
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Sword,		// オブジェクトの種類
		CollisionType::OBB,		// 当たり判定の種類
		this,					// このクラスのポインタ
		m_collision.get()		// 当たり判定
	);

	m_originalBox.Center;
}


// --------------------------------------------
/// <summary>
/// 桃太郎の刀の更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// --------------------------------------------
void Sword::Update(float elapsedTime)
{
	// 現在のステートの更新処理
	m_currentState->Update(elapsedTime);
}


// 描画処理
void Sword::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// 現在のステートの描画処理
	m_currentState->Render(context,states,view,projection);


#ifdef _DEBUG
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // _DEBUG
}

// 終了処理
void Sword::Finalize()
{
}


// シーン変更処理
void Sword::ChangeState(IWeapon* state)
{
	// 同じステートに変更しようとしたら早期リターン
	if (m_currentState == state)return;
	// ステートの事後処理
	m_currentState->PostUpdate();
	// 新しいステートに切り替える
	m_currentState = state;
	// 新しいステートの事前処理を行う
	m_currentState->PreUpdate();
}

void Sword::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}
