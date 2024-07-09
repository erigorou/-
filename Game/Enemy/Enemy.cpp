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

#include "Game/Enemy/Enemy.h"

#include "Interface/IState.h"

// 固定値
const float Enemy::ENEMY_SPEED = 0.2f;
const float Enemy::ENEMY_SCALE = 0.8f;

// --------------------------------
//  コンストラクタ
// --------------------------------
Enemy::Enemy(PlayScene* playScene)
	:
	m_playScene(playScene)
	,m_position{0.f, 0.f, 0.f}
	,m_angle{0.f}
	,m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
{
}

// --------------------------------
//  デストラクタ
// --------------------------------
Enemy::~Enemy()
{
}

// --------------------------------
//  イニシャライズ
// --------------------------------
void Enemy::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	UNREFERENCED_PARAMETER(states);

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む(仮でサイコロを読み込む)
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/oni.cmo", *fx);

	// HPを設定
	m_hp = std::make_unique<Enemy_HP>(5);
	m_hp->SetHP(5);

	// ビヘイビアツリーを取得
	m_pBT = std::make_unique<BehaviorTree>();

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
void Enemy::CreateState()
{
	// 待機状態を取得する
	m_enemyIdling = std::make_unique<EnemyIdling>(this);
	m_enemyIdling->Initialize(m_model.get());
	// 追尾状態を取得する
	m_enemyApproaching = std::make_unique<EnemyApproaching>(this);
	m_enemyApproaching->Initialize(m_model.get());
	// 初期のステートを待機状態に割り当てる
	m_currentState = m_enemyIdling.get();
}



// --------------------------------
//  更新処理
// --------------------------------
void Enemy::ChangeState(IState* newState)
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
void Enemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_worldMatrix = Matrix::Identity;

	// ステータスを更新しまーす
	m_currentState->Update(elapsedTime, m_position);

	// キー入力を受け付ける。
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1)
	{m_velocity += Vector3::Forward * Enemy::ENEMY_SPEED;}

	if (keyboardState.F2)
	{m_velocity += Vector3::Backward * Enemy::ENEMY_SPEED;}


	if (keyboardState.Enter)
	{
		// ビヘイビアツリーを実行。
		m_pBT->run();
	}

	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));	// 回転角の設定

	m_velocity *= Enemy::ENEMY_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ワールド行列の計算
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(ENEMY_SCALE)			// サイズ計算
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// 位置の設定
}


// --------------------------------
//  表示処理
// --------------------------------
void Enemy::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// 深度値を参照して書き込む
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// リソースを取得
	CommonResources* resources = CommonResources::GetInstance();

	// ステートを表示する
	m_currentState->Render(context,states,view,projection);

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// 境界球の描画
	DrawBoundingSphere(device, context, states, view, projection, m_currentState->GetBoundingSphereBody());

	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyPos : %f : %f : %f", m_position.x, m_position.y, m_position.z);

}


// --------------------------------
// 境界球を表示
// --------------------------------
void Enemy::DrawBoundingSphere(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const DirectX::BoundingSphere boundingSphere
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
		m_primitiveBatch.get(),	// プリミティブバッチ
		boundingSphere,			// 描画したい境界球
		Colors::DarkRed			// 色
	);
	m_primitiveBatch->End();
}


// --------------------------------
//  終了処理
// --------------------------------
void Enemy::Finalize()
{
}