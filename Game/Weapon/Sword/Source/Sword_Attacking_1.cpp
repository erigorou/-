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
#include "GeometricPrimitive.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"


// --------------------------------
//  固定値
// --------------------------------
const float Sword_Attacking_1::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_1::ATTACK_TIME  = 0.5f;


// --------------------------------
//  コンストラクタ
// --------------------------------
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
	m_particles = m_sword->GetPlayScene()->GetParticle();
}

// --------------------------------
//  デストラクタ
// --------------------------------
Sword_Attacking_1::~Sword_Attacking_1()
{
}


// --------------------------------
//  初期化処理
// --------------------------------
void Sword_Attacking_1::Initialize()
{
	using namespace DirectX::SimpleMath;
	// モデルを取得
	m_model = m_sword->GetModel();
	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;
}


// --------------------------------
//  状態開始処理
// --------------------------------
void Sword_Attacking_1::PreUpdate()
{
	m_totalSeconds = 0.0f;									// 経過時間の初期化
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true);		// 衝突可能にする

	// パーティクルの初期化
	m_rootPos.clear();
	m_tipPos.clear();
}


// --------------------------------
//  更新処理
// --------------------------------
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
		// 0-1の間の値を取得
		t = m_totalSeconds / ATTACK_TIME;
		m_rot.y = 250.0f * Easying::easeOutBack(t);
		m_rot.x = 10 + 30.0f * Easying::easeOutBack(t);


		m_rot.x = XMConvertToRadians(m_rot.x);
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
		*= SimpleMath::Matrix::CreateRotationX(RADIAN_90 / 2)							// 剣を90度横に向ける
		*= SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))				// 少しだけずらす
		*= SimpleMath::Matrix::CreateRotationX(m_rot.x)									// 薙ぎ払いの回転を反映

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

	// エフェクト描画用の根本と頂点を描画
	GetCudgelBothEnds();
}


// --------------------------------
//  両端座標の取得処理
// --------------------------------
void Sword_Attacking_1::GetCudgelBothEnds()
{
	// 根本と頂点のワールド座標をそれぞれ取得
	m_rootPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT	, 0.0f), m_worldMatrix));
	m_tipPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_TOP_HEIGHT	, 0.0f), m_worldMatrix));

	// パーティクルを生成
	CreateSwordParticle();
}


// --------------------------------
//  斬撃エフェクト生成処理
// --------------------------------
void Sword_Attacking_1::CreateSwordParticle()
{
	int max = static_cast<int>(m_rootPos.size()) - 1;
	if (max > 1)
	{
		DirectX::VertexPositionTexture ver[4] =
		{
			DirectX::VertexPositionTexture(m_tipPos[max]		,Vector2(0, 0)),	// 左上
			DirectX::VertexPositionTexture(m_tipPos[max - 1]	,Vector2(1, 0)),	// 右上
			DirectX::VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// 右下
			DirectX::VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// 左下
		};
		m_particles->CreateSwordTrail(ver);
	}
}


// --------------------------------
//  状態終了処理
// --------------------------------
void Sword_Attacking_1::PostUpdate()
{
}


// --------------------------------
//  描画処理
// --------------------------------
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
	debugString->AddString("");
#endif // _DEBUG
}


// --------------------------------
//  終了処理
// --------------------------------
void Sword_Attacking_1::Finalize()
{
}


// --------------------------------
//  衝突時イベント
// --------------------------------
void Sword_Attacking_1::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
