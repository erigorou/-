/// ---------------------------
///
/// 鬼の金棒の攻撃状態
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


// 固定値 ==================================
const float Cudgel_Attacking::CHARGE_TIME = 1.0f;	// 振りかざし時間
const float Cudgel_Attacking::WINDUP_TIME = 1.4f;	// 待機
const float Cudgel_Attacking::ATTACK_TIME = 1.7f;	// 降り下ろし



// コンストラクタ
Cudgel_Attacking::Cudgel_Attacking(Cudgel* cudgel)
	:
	m_cudgel(cudgel)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angleRL(0.0f)
	, m_angleUD(0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
	, m_model(nullptr)
{
}

// デストラクタ
Cudgel_Attacking::~Cudgel_Attacking()
{
}


// 初期化処理
void Cudgel_Attacking::Initialize()
{
	using namespace DirectX::SimpleMath;
	// モデルを取得
	m_model = m_cudgel->GetModel();

	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;

	// モデルの大きさに合わせてOBBを設定する
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);
}


// 事前処理
void Cudgel_Attacking::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	m_angleUD = 0.0f;
}

// 更新処理
void Cudgel_Attacking::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	// 合計時間を計測
	m_totalSeconds += elapsedTime;

	// 敵を取得
	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	// 敵の座標を取得
	m_position = enemy->GetPosition();
	// 敵の回転角を取得
	m_angleRL = enemy->GetAngle();


	// 1秒上にあげて、
	if (m_totalSeconds < CHARGE_TIME)
	{
		m_angleUD = XMConvertToRadians(-20.0f * (m_totalSeconds / 0.5f)); // 上げる角度を20度に設定
	}

	// 0.,4秒間の遅延
	else if (m_totalSeconds < WINDUP_TIME){}

	// 0.3秒振り下ろす。
	else if (m_totalSeconds < ATTACK_TIME)
	{
		// 次の0.2秒間で振り下ろす
		m_angleUD = XMConvertToRadians(-30.0f + 85.0f * ((m_totalSeconds - 1.4f) / 0.3f)); // 85度振り下ろす
	}


	// ワールド行列を更新する
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);	// サイズの設定 & 初期化
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3(0.0f, 1.0f, 0.0f))			// 原点からすこしずらす
		*= Matrix::CreateRotationX(-m_angleUD)							// 縦回転を行う
		*= Matrix::CreateTranslation(Cudgel::DIRECTION_ENEMY)			// プレイヤーの位置にずらす
		*= Matrix::CreateRotationY(-m_angleRL)							// 横回転を行う
		*= Matrix::CreateTranslation(m_position);						// プレイヤの位置に設定する

	// 当たり判定を反映させる
	m_originalBox.Transform(m_boundingBox, m_worldMatrix);
}

// 事後処理
void Cudgel_Attacking::PostUpdate()
{
}


// 描画処理
void Cudgel_Attacking::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);


#ifdef _DEBUG
	auto debugString = resources->GetDebugString();
	debugString->AddString("Cudgel, %f : %f : %f", m_position.x, m_position.y, m_position.z);
	debugString->AddString("angleUD, %f", m_angleUD);
#endif // _DEBUG
}


// 終了処理
void Cudgel_Attacking::Finalize()
{
}