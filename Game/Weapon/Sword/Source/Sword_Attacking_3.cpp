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
#include "Game/Weapon/Sword/Header/Sword_Attacking_3.h"


// --------------------------------
//  固定値
// --------------------------------
const float Sword_Attacking_3::RADIAN_90 = DirectX::XMConvertToRadians(90);
const float Sword_Attacking_3::ATTACK_TIME  = 0.7f;


// --------------------------------
//  コンストラクタ
// --------------------------------
Sword_Attacking_3::Sword_Attacking_3(Sword* sword)
	: m_sword		(sword)
	, m_position	(5.0f, 0.0f, 5.0f)
	, m_velocity	(0.0f, 0.0f, 0.0f)
	, m_angle		(0.0f)
	, m_rot			(0.0f, 0.0f, 0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix	(DirectX::SimpleMath::Matrix::Identity)
{
}

// --------------------------------
//	デストラクタ
// --------------------------------
Sword_Attacking_3::~Sword_Attacking_3()
{
}


// --------------------------------
//  初期化処理
// --------------------------------
void Sword_Attacking_3::Initialize()
{
	m_particles = m_sword->GetPlayScene()->GetParticle();	// パーティクルpの取得
}


// --------------------------------
//  状態開始処理
// --------------------------------
void Sword_Attacking_3::PreUpdate()
{
	// 攻撃中のフラグを立てる
	m_sword->SetAttackFlag(true);

	m_totalSeconds = 0.0f;								// 経過時間のリセット
	//m_sword->GetPlayScene()->GetEnemy()->CanHit(true);	// 衝突可能フラグを有効に

	m_rootPos.clear();	// 根本の座標をクリア
	m_tipPos.clear();	// 先端の座標をクリア
}


// --------------------------------
//  更新処理
// --------------------------------
void Sword_Attacking_3::Update(float elapsedTime)
{
	using namespace DirectX;

	// 経過時間を計測
	m_totalSeconds += elapsedTime;

	// プレイヤーの座標を取得
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// プレイヤーの回転を取得
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();

	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		t = m_totalSeconds / ATTACK_TIME;  // 進行度を0から1へ

		// 上方向に切り上げるように、X軸回転を調整
		m_rot.x = 120.0f - 100.0f * Easing::easeOutCirc(t);  // 切り上げ角度を大きめに設定

		m_rot.x = XMConvertToRadians(m_rot.x);  // ラジアンに変換
	}
	else
	{
		//// 攻撃時間を過ぎたら当たり判定を無効にする
		//m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
	}

	// ワールド行列を更新する
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE); // 剣のサイズの設定
	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationZ(m_rot.x) // X軸回転を適用
		*= SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(-90.0f))
		*= SimpleMath::Matrix::CreateTranslation(Vector3(2.0f, 2.0f, -2.0f));

	// 2. プレイヤーの位置と回転を適用
	m_worldMatrix
		*= SimpleMath::Matrix::CreateRotationY(-m_angle) // プレイヤーの方向に回転
		*= SimpleMath::Matrix::CreateTranslation(m_position); // プレイヤーの位置に設定

	// 当たり判定の位置を設定
	m_sword->SetCollisionPosition(m_worldMatrix);

	// エフェクト描画用の根本と頂点を描画
	GetCudgelBothEnds();
}


// --------------------------------
//  量頂点の座標取得処理
// --------------------------------
void Sword_Attacking_3::GetCudgelBothEnds()
{
	// 根本と頂点のワールド座標をそれぞれ取得
	m_rootPos	.push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT, 0.0f), m_worldMatrix));
	m_tipPos	.push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_TOP_HEIGHT	, 0.0f), m_worldMatrix));

	// パーティクルを生成
	CreateSwordParticle();
}


// --------------------------------
//  斬撃エフェクトの生成処理
// --------------------------------
void Sword_Attacking_3::CreateSwordParticle()
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
void Sword_Attacking_3::PostUpdate()
{
}


// --------------------------------
//  終了処理
// --------------------------------
void Sword_Attacking_3::Finalize()
{
}


// --------------------------------
//  衝突処理イベント
// --------------------------------
void Sword_Attacking_3::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
