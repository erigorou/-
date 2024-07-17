// ----------------
//
// 桃太郎の挙動
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

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"


// ここで静的メンバー変数を定義する
const DirectX::SimpleMath::Vector3 Player::HOME_POSITION(0.0f);
const float Player::PLAYER_SPEED = 0.35f;
const float Player::PLAYER_SCALE = 0.1f;
const float Player::APPLIED_ATTACK_TIME = 1.0f;


// --------------------------------
//  コンストラクタ
// --------------------------------
Player::Player(PlayScene* playScene)
	:
	m_playScene(playScene),
	m_model{},
	m_position{0, 0, 40},
	m_angle{0.f},
	m_worldMatrix{},
	m_currentState{},
	m_keyboardState{},
	m_tracker{}
{
}


// --------------------------------
//  デストラクタ
// --------------------------------
Player::~Player()
{
}



// --------------------------------
//  イニシャライズ
// --------------------------------
void Player::Initialize()
{
	using namespace DirectX;
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// キー入力を生成（全体で使う）
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/momotaro.cmo", *fx);

	// ステートの作成
	CreateState();

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);
	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
}


// --------------------------------
//  ステートを作成する
// --------------------------------
void Player::CreateState()
{
	// アイドリングステートを取得
	m_playerIdling = std::make_unique<PlayerIdling>(this);
	// アイドリングステートの初期化
	m_playerIdling->Initialize(m_model.get());
	// ダッジングステートを取得
	m_playerDodging = std::make_unique<PlayerDodging>(this);
	// ダッジングステートを初期化
	m_playerDodging->Initialize(m_model.get());

	// アタッキングステート1を取得
	m_playerAttacking_1 = std::make_unique<PlayerAttacking_1>(this);
	m_playerAttacking_2 = std::make_unique<PlayerAttacking_2>(this);
	m_playerAttacking_3 = std::make_unique<PlayerAttacking_3>(this);
	m_playerAttacking_4 = std::make_unique<PlayerAttacking_4>(this);

	// アタッキングステート1を初期化
	m_playerAttacking_1->Initialize(m_model.get());
	m_playerAttacking_2->Initialize(m_model.get());
	m_playerAttacking_3->Initialize(m_model.get());
	m_playerAttacking_4->Initialize(m_model.get());


	// 最初のステートを設定
	m_currentState = m_playerIdling.get();
}


/// <summary>
/// ステートを変更する
/// </summary>
/// <param name="newState">変更したいステート</param>
void Player::ChangeState(IState* newState)
{
	// 同じステートで更新しようとすると早期リターン
	if (m_currentState == newState) return;
	// 事後更新処理を行う
	m_currentState->PostUpdate();
	// 現在のステートを変更する
	m_currentState = newState;
	// 新しいステートの事前更新を行う
	m_currentState->PreUpdate();
}


// 時間回帰処理
void Player::TimeComparison(float& nowTime, const float totalTime, IState* newState, const float elapsedTime)
{
	// 定められた時間になったら
	if (nowTime >= totalTime)
	{
		// シーンを変更
		ChangeState(newState);
		return;
	}
	// 時間の計測を行う
	nowTime += elapsedTime;
}

// ----------------------------------------------
/// <summary>
/// プレイヤーの更新処理
/// </summary>
/// <param name="enemyPos">敵の座標</param>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void Player::Update(
	const DirectX::SimpleMath::Vector3 enemyPos,
	const float elapsedTime
	)
{
	// ステートの更新
	m_currentState->Update(elapsedTime, m_position);

	// いずれここは1行のみにする　＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊

	// 敵の位置を比較して回転角を計算する
	m_angle = Math::CalculationAngle(m_position, enemyPos);
	// ワールド座標の更新
	CalculationMatrix();
}


// ----------------------------------------
//  敵の位置からプレイヤーの回転角を求める
// ----------------------------------------
void Player::CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos)
{
	using namespace DirectX::SimpleMath;

	Vector3 forward = m_position - enemyPos;		// 敵の方向をベクトルで取得
	forward.Normalize();										// 正規化

	Vector3 worldForward = Vector3::Forward;			// ワールド座標の前方ベクトルを作成
	float dotProduct = forward.Dot(worldForward);	// 内積を取得
	m_angle = acosf(dotProduct);								// 内積から角度を取得(弧度法)

	Vector3 crossProduct = forward.Cross(worldForward);	// カメラの前方向ベクトルが右方向に向いているかどうかで符号を決定
	m_angle = (crossProduct.y < 0)? -m_angle: m_angle;	// -180 ~ 180に収める。
}



// --------------------------------
//  移動の管理
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// キー入力を受け付ける。
	Keyboard::State keyboardState = Keyboard::Get().GetState();

	m_velocity = SimpleMath::Vector3::Zero;	// 速度値をリセットする

	if (keyboardState.Up)
		m_velocity += Vector3::Forward;	// 「↑」で前進
	if (keyboardState.Down)
		m_velocity += Vector3::Backward;// 「↓」で後退
	if (keyboardState.Left)
 		m_velocity += Vector3::Left;	// 「←」で左移動
	if (keyboardState.Right)
		m_velocity += Vector3::Right;	// 「→」で右移動

	if (m_velocity.LengthSquared() > 0.0f)		// 移動量がある場合：
		m_velocity.Normalize();					// 移動量を正規化する

	m_velocity *= PLAYER_SPEED;					// 移動量を補正する
	m_velocity = Math::truncate_vector(m_velocity, 2);

	m_position += Vector3::Transform(m_velocity, Matrix::CreateRotationY(-m_angle));	// 移動量を座標に反映
}


// --------------------------------
//  ワールド行列の計算
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	// 行列の計算を行う
	m_worldMatrix = Matrix::Identity;		// 更新ごとに初期化を行う
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3::Zero)										// 原点に移動
		*= Matrix::CreateScale		(PLAYER_SCALE)										// プレイヤーのサイズ変更
		*= Matrix::CreateRotationY	(-m_angle + XMConvertToRadians(180.f))	// 敵の方向を見るように設定する
		*= Matrix::CreateTranslation(m_position);											// 座標を移動させる
}


// --------------------------------
//  表示処理
// --------------------------------
void Player::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources
	)
{
	using namespace DirectX;

	context->OMSetDepthStencilState(states->DepthDefault(), 0);	// 深度値を参照して書き込む

	// モデルのエフェクト情報を更新する
	m_model->UpdateEffects([](DirectX::IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// ライトを有効化する
				basicEffect->SetLightingEnabled(true);
				// アンビエントライトの設定
				basicEffect->SetAmbientLightColor(Colors::Gray); // 適切なアンビエント色を設定
				// 必要に応じてライトの設定を行う
				basicEffect->SetLightEnabled(0, true);
				basicEffect->SetLightDiffuseColor(0, Colors::White);  // ディフューズ色を設定
				basicEffect->SetLightSpecularColor(0, Colors::White); // スペキュラー色を設定

				basicEffect->SetLightEnabled(1, false); // 追加のライトが不要なら無効化
				basicEffect->SetLightEnabled(2, false); // 追加のライトが不要なら無効化
				// モデルの自発光色を黒に設定して無効化
				basicEffect->SetEmissiveColor(Colors::Black);
			}
		}
	);

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// ステートで描画する
	m_currentState->Render(
		context,
		states,
		view,
		projection);

	//// 体の境界球の描画
	//DrawBoundingSphere(device, context, states, view, projection,m_currentState->GetBoundingSphereBody());
}


// --------------------------------
// 境界球を表示
// --------------------------------
void Player::DrawBoundingSphere(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const DirectX::BoundingSphere boundingSphere)
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
		boundingSphere,			// 境界球
		Colors::White			// 色
	);
	m_primitiveBatch->End();
}


// --------------------------------
//  終了処理
// --------------------------------
void Player::Finalize()
{
}

