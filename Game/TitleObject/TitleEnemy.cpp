// ----------------
//
// 鬼の挙動
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/Math.h"

#include "TitleEnemy.h"
#include "EnemyTitleMoving.h"
#include "EnemyTitleIdling.h"

#include "Interface/IState.h"

// 固定値
const float TitleEnemy::TITLE_ENEMY_SPEED	= 0.1f;
const float TitleEnemy::TITLE_ENEMY_SCALE	= 1.0f;
const float TitleEnemy::COOL_TIME	= 0.4f;

// --------------------------------
//  コンストラクタ
// --------------------------------
TitleEnemy::TitleEnemy(TitleScene* titleScene)
	: m_TitleScene(titleScene)
	, m_currentState()
	, m_position{0.f, 0.f, 0.f}
	, m_angle{0.f}
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
{
}

// --------------------------------
//  デストラクタ
// --------------------------------
TitleEnemy::~TitleEnemy()
{
}

// --------------------------------
//  イニシャライズ
 // --------------------------------
void TitleEnemy::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	UNREFERENCED_PARAMETER(states);

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/oni.cmo", *fx);


	// ステートの作成
	CreateState();


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
}



// --------------------------------
//  更新処理
// --------------------------------
void TitleEnemy::CreateState()
{
	// === 状態の生成 ====
	m_titleIdling = std::make_unique<EnemyTitleIdling>(this);		// タイトル画面の敵の待機
	m_titleMoving = std::make_unique<EnemyTitleMoving>(this);	// タイトル画面の敵の移動


	// === 状態の初期化 ===
	m_titleIdling->Initialize(m_model.get());	// タイトル画面の敵の待機
	m_titleMoving->Initialize(m_model.get());	// タイトル画面の敵の移動

	// 初期のステートを待機状態に割り当てる
	m_currentState = m_titleIdling.get();
}


// --------------------------------
//  状態の生成処理
// --------------------------------
void TitleEnemy::ChangeState(IState* newState)
{
	// おんなじステートを更新しようとしたら戻る
	if (m_currentState == newState) return;
	// 新規の状態遷移前に事後更新を行う
	m_currentState->PostUpdate();
	// 新規の状態を現在の状態に設定する
	m_currentState = newState;
	// 新規の状態遷移後に事前更新を行う
	m_currentState->PreUpdate();
}


// --------------------------------
//  更新処理
// --------------------------------
void TitleEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_worldMatrix = Matrix::Identity;

	// ステータスを更新しまーす
	m_currentState->Update(elapsedTime);


	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));	// 回転角の設定

	m_velocity *= TitleEnemy::TITLE_ENEMY_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ワールド行列の計算@;;;;;;;;;;;;;;;;
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(TITLE_ENEMY_SCALE)		// サイズ計算
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// 位置の設定
}


// --------------------------------
//  表示処理
// --------------------------------
void TitleEnemy::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{


	// 深度値を参照して書き込む
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	m_currentState->Render(context,states,view,projection);				// ステート側の描画
	m_model->Draw(context, *states, m_worldMatrix, view, projection);	// モデルの描画

#ifdef _DEBUG
#endif // _DEBUG
}



// --------------------------------
//  終了処理
// --------------------------------
void TitleEnemy::Finalize()
{
}