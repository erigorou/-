/// ---------------------------
///
/// プレイヤーの剣の攻撃（円切り）
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"
#include "Libraries/MyLib/EasingFunctions.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"


// 固定値
const float Sword_Attacking_1::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_1::ATTACK_TIME  = 0.5f;


// コンストラクタ
Sword_Attacking_1::Sword_Attacking_1(Sword* sword)
	:
	m_sword(sword),
	m_position(5.0f, 0.0f, 5.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_totalSeconds(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
}

// デストラクタ
Sword_Attacking_1::~Sword_Attacking_1()
{
}


// 初期化処理
void Sword_Attacking_1::Initialize()
{
	using namespace DirectX::SimpleMath;
	// モデルを取得
	m_model = m_sword->GetModel();
	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;
}


// 事前処理
void Sword_Attacking_1::PreUpdate()
{
	// 経過時間のリセット
	m_totalSeconds = 0.0f;

	// 当たれるようにする
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true);
}

// 更新処理
void Sword_Attacking_1::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// 経過時間を計測
	m_totalSeconds += elapsedTime;

	// プレイヤーの座標を取得
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// プレイヤーの回転を取得
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();



	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		t = m_totalSeconds / ATTACK_TIME;
		m_rot.y = 250.0f * Easying::easeOutBack(t);
		m_rot.y = XMConvertToRadians(m_rot.y);
	}
	else
	{
		// 攻撃時間を過ぎたら当たり判定を無効にする
		m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
	}

	// ワールド行列を更新する
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);							// 剣のサイズの設定

	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationX(RADIAN_90)								// 剣を90度横に向ける
		*= SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))				// 少しだけずらす
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)								// プレイヤーの横に回転させる
		*= SimpleMath::Matrix::CreateRotationY(m_rot.y)									// 薙ぎ払いの回転を反映
		*= SimpleMath::Matrix::CreateTranslation(m_position);							// プレイヤーの位置に設定

	// 当たり判定の位置を設定
	m_sword->SetCollisionPosition(m_worldMatrix);

	// 攻撃が終わったらステートをIdlingStateに戻す
	if (m_totalSeconds >= 1.0f)
	{
		m_sword->ChangeState(m_sword->GetIdlingState());
	}
}



// 事後処理
void Sword_Attacking_1::PostUpdate()
{
}


// 描画処理
void Sword_Attacking_1::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);


#ifdef _DEBUG
	auto debugString = resources->GetDebugString();
	debugString->AddString("sword, %f : %f : %f", m_position.x, m_position.y, m_position.z);
#endif // _DEBUG
}


// 終了処理
void Sword_Attacking_1::Finalize()
{
}


void Sword_Attacking_1::HitAction(InterSectData data)
{
}
