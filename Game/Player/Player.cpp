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

#include "Game/Player/Player.h"


// ここで静的メンバー変数を定義する
const DirectX::SimpleMath::Vector3 Player::HOME_POSITION(0.0f);
const float Player::PLAYER_SPEED = 0.05f;


// --------------------------------
//  コンストラクタ
// --------------------------------
Player::Player()
	:
	m_model{},
	m_position{},
	m_angle{},
	m_cameraAngle{},
	m_worldMatrix{}
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
void Player::Initialize(
	 ID3D11Device* device,
	const ID3D11DeviceContext* context,
	const DirectX::CommonStates* states
	)
{
	using namespace DirectX;

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/momotaro.cmo", *fx);

	// 座標の初期設定
	m_position = SimpleMath::Vector3(4.0f, 0.0f, 2.0f);
	// 速度の初期化
	m_velocity = SimpleMath::Vector3::Zero;
	// 回転の初期値
	m_angle = 0.0f;
	// カメラの回転角の初期化
	m_cameraAngle = 0.0f;
}

// --------------------------------
//  更新処理
// --------------------------------
void Player::Update(
	DirectX::SimpleMath::Vector3 enemyPos,
	float cameraAngle
	)
{
	using namespace DirectX::SimpleMath;

	// このクラスでカメラのアングルを使用できるようにする
	m_cameraAngle = cameraAngle;

	m_angle = -cameraAngle;

	// プレイヤーの回転を加味した計算を行う
	Matrix matrix = Matrix::CreateRotationY(-cameraAngle);	// カメラの回転を与える

	// プレイヤーの移動
	MovePlayer();

	// 回転を加味して実際に移動する
	m_position += Vector3::Transform(m_velocity, matrix);

	// ワールドマトリクスの計算
	CalculationMatrix();
}



// --------------------------------
//  移動の管理
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;

	// キー入力を受け付ける。
	Keyboard::State keyboardState = Keyboard::Get().GetState();

	// 速度値をリセットする
	m_velocity = SimpleMath::Vector3::Zero;

	if (keyboardState.Up)	// 「↑」で前進
		m_velocity += SimpleMath::Vector3::Forward;
	if (keyboardState.Down)	// 「↓」で後退
		m_velocity += SimpleMath::Vector3::Backward;
	if (keyboardState.Left)	// 「←」で左移動
 		m_velocity += SimpleMath::Vector3::Left;
	if (keyboardState.Right)// 「→」で右移動
		m_velocity += SimpleMath::Vector3::Right;

	if (keyboardState.A)
		m_angle += 1.0f;
	if (keyboardState.D)
		m_angle -= 1.0f;

	(m_angle <= 360) ? m_angle : 0.0f;


	// 移動量を正規化する
	m_velocity.Normalize();

	// 移動量を補正する
	m_velocity *= PLAYER_SPEED;
}


// --------------------------------
//  ワールド行列の計算
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX;

	// 行列の計算を行う
	m_worldMatrix = SimpleMath::Matrix::Identity;
	m_worldMatrix
		*= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3::Zero)		// 初期地点に移動
		*= SimpleMath::Matrix::CreateRotationY(m_angle)							// 回転を適用
		*= SimpleMath::Matrix::CreateTranslation(m_position);					// 座標を移動させる
}


// --------------------------------
//  表示処理
// --------------------------------
void Player::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources
	)
{
	using namespace DirectX;

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("m_angle : %f", m_angle);
	debugString->AddString("m_position.x : %f", m_position.x);
	debugString->AddString("m_position.y : %f", m_position.y);
	debugString->AddString("m_position.z : %f", m_position.z);
}


// --------------------------------
//  終了処理
// --------------------------------
void Player::Finalize()
{
}