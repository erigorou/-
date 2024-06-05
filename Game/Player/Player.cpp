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
	m_position{0, 0, 5},
	m_angle{0.f},
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
}

// --------------------------------
//  更新処理
// --------------------------------
void Player::Update(
	DirectX::SimpleMath::Vector3 enemyPos
	)
{
	using namespace DirectX::SimpleMath;
	// 敵の位置を比較して回転角を計算する
	CaluclationAngle(enemyPos);
	// プレイヤーの移動
	MovePlayer();
	// ワールド座標の更新
	CaluclationMatrix();
}


// ----------------------------------------
//  敵の位置からプレイヤーの回転角を求める
// ----------------------------------------
void Player::CaluclationAngle(DirectX::SimpleMath::Vector3 enemyPos)
{
	using namespace DirectX::SimpleMath;

	Vector3 forward = m_position - enemyPos;	// 敵の方向をベクトルで取得
	forward.Normalize();						// 正規化

	Vector3 worldForward = Vector3::Forward;			// ワールド座標の前方ベクトルを作成
	float dotProduct = forward.Dot(worldForward);		// 内積を取得
	m_angle = acosf(dotProduct);						// 内積から角度を取得(弧度法)

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

	m_position += Vector3::Transform(-m_velocity, Matrix::CreateRotationY(-m_angle));	// 移動量を座標に反映

}


// --------------------------------
//  ワールド行列の計算
// --------------------------------
void Player::CaluclationMatrix()
{
	using namespace DirectX::SimpleMath;
	// 行列の計算を行う
	m_worldMatrix = Matrix::Identity;		// 更新ごとに初期化を行う
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3::Zero)					// 原点に移動
		*= Matrix::CreateRotationY	(-m_angle)						// 敵の方向を見るように設定する
		*= Matrix::CreateTranslation(m_position);					// 座標を移動させる
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

	m_model->Draw(context, *states, SimpleMath::Matrix::Identity, view, projection);

	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("m_angle : %f", XMConvertToDegrees( m_angle));
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