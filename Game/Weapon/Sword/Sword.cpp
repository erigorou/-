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

#include "Sword.h"
#include "Game/Player/Player.h"


// 固定値
const float Sword::SWORD_SCALE = Player::PLAYER_SCALE;

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
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(SWORD_SCALE);

}


// シーンを生成する
void Sword::CreateState()
{
	// 待機ステートを取得する か 初期化する
	m_swordIdling = std::make_unique<SwordIdling>(this);
	m_swordIdling->Initialize();

	// 現在のステートを設定
	m_currentState = m_swordIdling.get();
}

// 更新処理
void Sword::Update(float elapsedTime)
{
	// 現在のステートの更新処理
	m_currentState->Update(elapsedTime);
}


// 描画処理
void Sword::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources)
{
	// 現在のステートの描画処理
	m_currentState->Render(context,states,view,projection);
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