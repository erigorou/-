/// ---------------------------
///
/// プレイヤーの剣の攻撃（円切り）(反転)
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
#include "Game/Weapon/Sword/Header/Sword_Attacking_2.h"


// --------------------------------
//  固定値
// --------------------------------
const float Sword_Attacking_2::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_2::ATTACK_TIME  = 0.5f;


// --------------------------------
//  コンストラクタ
// --------------------------------
Sword_Attacking_2::Sword_Attacking_2(Sword* sword)
	: m_sword		(sword)
	, m_position	(5.0f, 0.0f, 5.0f)
	, m_velocity	(0.0f, 0.0f, 0.0f)
	, m_angle		(0.0f)
	, m_rot			(0.0f, 0.0f, 0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix	(DirectX::SimpleMath::Matrix::Identity)
	, m_model		(nullptr)
{
}


// --------------------------------
//  デストラクタ
// --------------------------------
Sword_Attacking_2::~Sword_Attacking_2()
{
}


// --------------------------------
//  初期化処理
// --------------------------------
void Sword_Attacking_2::Initialize()
{
	m_model		= m_sword->GetModel();						// モデルの取得
	m_particles = m_sword->GetPlayScene()->GetParticle();	// パーティクルの取得
}


// --------------------------------
//  状態開始処理
// --------------------------------
void Sword_Attacking_2::PreUpdate()
{
	m_totalSeconds = 0.0f;								// 時間経過のリセット
	m_sword->GetPlayScene()->GetEnemy()->CanHit(true);	// 衝突可能フラグを有効に

	m_rootPos	.clear();	// 根本の座標をクリア
	m_tipPos	.clear();	// 先端の座標をクリア
}


// --------------------------------
//  更新処理
// --------------------------------
void Sword_Attacking_2::Update(float elapsedTime)
{
	using namespace DirectX;

	// 経過時間を計測
	m_totalSeconds += elapsedTime;

	// プレイヤーの座標を取得
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// プレイヤーの回転を取得
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();

	// イージング関数を使って回転を計算
	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		t = m_totalSeconds / ATTACK_TIME;
		m_rot.y = -250.0f * Easing::easeOutBack(t);
		m_rot.x = 10 - 40 * Easing::easeOutBack(t);

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
		*= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180))					// 反転
		*= SimpleMath::Matrix::CreateRotationX(RADIAN_90)								// 剣を90度横に向ける
		*= SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))				// 少しだけずらす
		*= SimpleMath::Matrix::CreateRotationX(m_rot.x)									// 薙ぎ払いの回転を反映
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)								// プレイヤーの横に回転させる
		*= SimpleMath::Matrix::CreateRotationY(m_rot.y)									// 薙ぎ払いの回転を反映
		*= SimpleMath::Matrix::CreateTranslation(m_position);							// プレイヤーの位置に設定

	// 当たり判定の位置を設定
	m_sword->SetCollisionPosition(m_worldMatrix);

	// エフェクト描画用の根本と頂点を描画
	GetCudgelBothEnds();
}


// --------------------------------
//  量頂点の取得
// --------------------------------
void Sword_Attacking_2::GetCudgelBothEnds()
{
	// 根本と頂点のワールド座標をそれぞれ取得
	m_rootPos.push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT, 0.0f), m_worldMatrix));
	m_tipPos.push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_TOP_HEIGHT, 0.0f), m_worldMatrix));

	// パーティクルを生成
	CreateSwordParticle();
}


// --------------------------------
//  斬撃エフェクトの生成処理
// --------------------------------
void Sword_Attacking_2::CreateSwordParticle()
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
void Sword_Attacking_2::PostUpdate()
{
}


// --------------------------------
//  描画処理
// --------------------------------
void Sword_Attacking_2::Render(ID3D11DeviceContext* context,
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
void Sword_Attacking_2::Finalize()
{
}


// --------------------------------
//  衝突処理イベント
// --------------------------------
void Sword_Attacking_2::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
