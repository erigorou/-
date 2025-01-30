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
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"


// --------------------------------
//  固定値
// --------------------------------
const float Sword_Attacking_1::RADIAN_90 = DirectX::XMConvertToRadians(90);


// --------------------------------
//  コンストラクタ
// --------------------------------
Sword_Attacking_1::Sword_Attacking_1(Sword* sword)
	: m_sword(sword)
	, m_position(5.0f, 0.0f, 5.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angle(0.0f)
	, m_rot(0.0f, 0.0f, 0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
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
	m_particles = m_sword->GetPlayScene()->GetParticle();	// パーティクルの取得
}


// --------------------------------
//  状態開始処理
// --------------------------------
void Sword_Attacking_1::PreUpdate()
{
	// 攻撃フラグを立てる
	m_sword->SetAttackFlag(true);

	m_totalSeconds = 0.0f;	// 時間経過のリセット

	m_rootPos.clear();	// 根本の座標をクリア
	m_tipPos.clear();	// 先端の座標をクリア

	bool canHit = true;

	// 衝突可能フラグを敵全体に付与
	EventMessenger::Execute("EnemyCanHit",&canHit);
}


// --------------------------------
//  更新処理
// --------------------------------
void Sword_Attacking_1::Update(float elapsedTime)
{
	// 経過時間を計測
	m_totalSeconds += elapsedTime;

	m_position	= m_sword->GetPlayScene()->GetPlayer()->GetPosition();	// プレイヤーの座標を取得
	m_angle		= m_sword->GetPlayScene()->GetPlayer()->GetAngle();		// プレイヤーの回転を取得

	// アニメーションの更新
	UpdateAnimation();
	// ワールド行列の更新
	UpdateWorldMatrix();
	// 当たり判定の位置を設定
	m_sword->SetCollisionPosition(m_worldMatrix);
	// エフェクト描画用の根本と頂点を描画
	GetCudgelBothEnds();
}


// --------------------------------
//  アニメーション更新処理
// --------------------------------
void Sword_Attacking_1::UpdateAnimation()
{
	// イージング関数を使って回転を計算
	float t = 0.0f;

	if (m_totalSeconds <= ATTACK_TIME)
	{
		// 時間の正規化
		t = m_totalSeconds / ATTACK_TIME;

		m_rot.y = MAX_SIDE_ANGLE					* Easing::easeOutBack(t);
		m_rot.x = INIT_ANGLE - MAX_VERTICAL_ANGLE	* Easing::easeOutBack(t);

		if (m_rot.y < MAX_SIDE_ANGLE)
		{
			// ボスが存在する場合
			if (m_sword->GetPlayScene()->GetBoss())
			{
				//// 当たり判定を無効にする
				//m_sword->GetPlayScene()->GetEnemy()->CanHit(false);
			}
			m_sword->SetAttackFlag(false);
		}

		// 設定
		m_rot.x = DirectX::XMConvertToRadians(m_rot.x);
		m_rot.y = DirectX::XMConvertToRadians(m_rot.y);
	}
}


// --------------------------------
// ワールド行列の更新処理
// --------------------------------
void Sword_Attacking_1::UpdateWorldMatrix()
{	// ワールド行列を更新する
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);	// 剣のサイズの設定

	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180))	// 反転
		*= DirectX::SimpleMath::Matrix::CreateRotationX(RADIAN_90)							// 剣を90度横に向ける
		*= DirectX::SimpleMath::Matrix::CreateTranslation(Vector3(1.0f, 2.0f, 0.0f))		// 少しだけずらす
		*= DirectX::SimpleMath::Matrix::CreateRotationX(m_rot.x)							// 薙ぎ払いの回転を反映
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)							// プレイヤーの横に回転させる
		*= DirectX::SimpleMath::Matrix::CreateRotationY(m_rot.y)							// 薙ぎ払いの回転を反映
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);						// プレイヤーの位置に設定

	m_sword->SetWorldMatrix(m_worldMatrix);	// ワールド行列を設定
}



// --------------------------------
//  量頂点の取得
// --------------------------------
void Sword_Attacking_1::GetCudgelBothEnds()
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
		EventMessenger::Execute("CreateSwordTrail", &ver);	// パーティクルの生成
	}
}


// --------------------------------
//  状態終了処理
// --------------------------------
void Sword_Attacking_1::PostUpdate()
{
	bool canHit = false;

	// 衝突可能フラグを敵全体に付与
	EventMessenger::Execute("EnemyCanHit", &canHit);
}



// --------------------------------
//  終了処理
// --------------------------------
void Sword_Attacking_1::Finalize()
{
}


// --------------------------------
//  衝突処理イベント
// --------------------------------
void Sword_Attacking_1::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
