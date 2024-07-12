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

#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


// 固定値									0.7f
const float Cudgel::CUDGEL_SCALE = Enemy::ENEMY_SCALE * 2.0f;

// コンストラクタ
Cudgel::Cudgel(PlayScene* playScene)
	:
	m_playScene(playScene)
{
}

// デストラクタ
Cudgel::~Cudgel()
{
}

// 初期化処理
void Cudgel::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();


	// モデルを生成
	m_model = std::make_unique<DirectX::Model>();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/nihontou.cmo", *fx);


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

	// ワールド座標の初期化
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(CUDGEL_SCALE);

}


// シーンを生成する
void Cudgel::CreateState()
{

}

// 更新処理
void Cudgel::Update(float elapsedTime)
{
	//// 現在のステートの更新処理
	//m_currentState->Update(elapsedTime);
}


// 描画処理
void Cudgel::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources)
{
	//// 現在のステートの描画処理
	//m_currentState->Render(context, states, view, projection);

	// 境界ボックスの描画
	DrawBoundingBox(device, context, states, view, projection, m_currentState->GetBoundingBox());
}

// --------------------------------
// 境界ボックスを表示
// --------------------------------
void Cudgel::DrawBoundingBox(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const DirectX::BoundingBox boundingBox)
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
		m_primitiveBatch.get(),	// プリミティブバッチ
		boundingBox,			// 境界ボックス
		Colors::Green			// 色
	);
	m_primitiveBatch->End();
}



// 終了処理
void Cudgel::Finalize()
{
}


// シーン変更処理
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