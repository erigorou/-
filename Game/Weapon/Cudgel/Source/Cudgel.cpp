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
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Collision.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


const float Cudgel::CUDGEL_SCALE = Enemy::ENEMY_SCALE * 1.1f;						// 金棒の拡大率
const DirectX::SimpleMath::Vector3 Cudgel::DIRECTION_ENEMY = { 6.0f, 5.0f, 0.0f };	// 持ち手の距離（敵と比較）
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_LENGTH =	{ 0.0f, 22.0f, 0.0f };	// 金棒の長さ（一番下から）
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_HADLE_POS = { 0.0f, 7.0f, 0.0f };	// 金棒の取っ手の部分（一番上）


// --------------------------------
// コンストラクタ
// --------------------------------
Cudgel::Cudgel(PlayScene* playScene)
	:
	m_playScene(playScene)
	,m_currentState()
	,m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
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
	auto states = resources->GetCommonStates();

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

	CreateModel(device);	// モデルの生成
	CreateState();			// ステートの作成
	CreateCollision();		// 当たり判定の生成
}


// モデルを生成する
void Cudgel::CreateModel(ID3D11Device1* device)
{

	m_model = std::make_unique<DirectX::Model>();													// モデルを作成

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);	// エフェクトファクトリーを作成
	fx->SetDirectory(L"Resources/Models");															// モデルのディレクトリを設定	
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/cudgel.cmo", *fx);			// パスのモデルの読み込みを行う
}


// --------------------------------
// 状態の生成
// --------------------------------
void Cudgel::CreateState()
{
	// 状態の生成
	m_idling = std::make_unique<Cudgel_Idling>(this);			// 待機
	m_attacking = std::make_unique<Cudgel_Attacking>(this);		// 攻撃
	m_sweeping = std::make_unique<Cudgel_Sweeping>(this);		// 薙ぎ払い

	// 状態の初期化
	m_idling->Initialize();		// 待機
	m_attacking->Initialize();	// 攻撃
	m_sweeping->Initialize();	// 薙ぎ払い

	// 初期状態を指定
	m_currentState = m_idling.get();
}

void Cudgel::CreateCollision()
{
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model.get());
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);
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
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// 現在のステートの描画処理
	m_currentState->Render(context, states, view, projection);

	// 境界ボックスの描画
	DrawBoundingBox(device, context, states, view, projection);
}

// --------------------------------
// 境界ボックスを表示
// --------------------------------
void Cudgel::DrawBoundingBox(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(device);


	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);
	// 描画
	m_primitiveBatch->Begin();
	DX::Draw(
		m_primitiveBatch.get(),				// プリミティブバッチ
		*m_collision.get(),					// 描画する境界ボックス	
		Colors::White						// 色
	);
	m_primitiveBatch->End();
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
void Cudgel::ChangeState(IWeapon* state)
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



void Cudgel::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}