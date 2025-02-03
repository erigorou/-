/// ---------------------------
///
/// プレイヤーの剣の攻撃（円切り）
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/Weapon/Sword/Sword.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Sword/Header/SwordAttacking2.h"


// --------------------------------
//  固定値
// --------------------------------
// 
const float SwordAttacking2::RADIAN_90 = DirectX::XMConvertToRadians(90);


// --------------------------------
//  コンストラクタ
// --------------------------------
SwordAttacking2::SwordAttacking2(Sword* sword)
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
SwordAttacking2::~SwordAttacking2()
{
}


// --------------------------------
//  初期化処理
// --------------------------------
void SwordAttacking2::Initialize()
{
}


// --------------------------------
//  状態開始処理
// --------------------------------
void SwordAttacking2::PreUpdate()
{
	m_sword->SetAttackFlag(true); // 攻撃フラグを立てる

	m_totalSeconds = 0.0f;	// 経過時間の初期化
	m_rootPos	.clear();	// 根本の座標配列をクリア
	m_tipPos	.clear();	// 先端の座標配列をクリア

	bool canHit = true;

	// 衝突可能フラグを敵全体に付与
	EventMessenger::Execute(EventList::EnemyCanHit, &canHit);
}


// --------------------------------
//  更新処理
// --------------------------------
void SwordAttacking2::Update(float elapsedTime)
{	
	// 経過時間を計測
	m_totalSeconds += elapsedTime;

	// プレイヤーを取得
	auto player = m_sword->GetPlayer();
	// プレイヤーの位置を取得
	m_position = player->GetPosition();
	// プレイヤーの角度を取得
	m_angle = player->GetAngle();

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
// アニメーション更新処理
// --------------------------------
void SwordAttacking2::UpdateAnimation()
{
	// 攻撃中の回転処理
	if (m_totalSeconds <= ATTACK_TIME)
	{
		// 秒数を正規化する
		float t = m_totalSeconds / ATTACK_TIME;

		// イージング関数を使って回転を計算
		m_rot.y = MAX_SIDE_ANGLE					* Easing::easeOutBack(t);
		m_rot.x = INIT_ANGLE + MAX_VERTICAL_ANGLE	* Easing::easeOutBack(t);

		if (m_rot.y > MAX_SIDE_ANGLE)
		{
			m_sword->SetAttackFlag(false);
		}

		// ラジアンに変換
		m_rot.x = DirectX::XMConvertToRadians(m_rot.x);
		m_rot.y = DirectX::XMConvertToRadians(m_rot.y);
	}
}



// --------------------------------
// ワールド行列の更新処理
// --------------------------------
void SwordAttacking2::UpdateWorldMatrix()
{
	// ワールド行列を更新する
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);

	// 剣を90度横に向ける
	Quaternion initialRotation = Quaternion::CreateFromAxisAngle(Vector3::UnitX, RADIAN_90 / 2);

	// 薙ぎ払いの回転
	Quaternion attackRotation = Quaternion::CreateFromYawPitchRoll(
		m_rot.y,
		m_rot.x,
		0.0f
	);

	// プレイヤーの回転
	Quaternion playerRotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -m_angle);

	// 回転の合成
	Quaternion totalRotation = initialRotation * attackRotation * playerRotation;

	// ワールド行列に回転と平行移動を適用
	m_worldMatrix *= Matrix::CreateTranslation(MATRIX_DIRECTION); // 少しだけずらす
	m_worldMatrix *= Matrix::CreateFromQuaternion(totalRotation); // 合成した回転を反映
	m_worldMatrix *= Matrix::CreateTranslation(m_position); // プレイヤーの位置に移動

	// ワールド行列を設定
	m_sword->SetWorldMatrix(m_worldMatrix);
}



// --------------------------------
//  両端座標の取得処理
// --------------------------------
void SwordAttacking2::GetCudgelBothEnds()
{
	// 根本と頂点のワールド座標をそれぞれ取得
	m_rootPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_ROOT_HEIGHT, 0.0f)	, m_worldMatrix));
	m_tipPos.	push_back(Vector3::Transform(Vector3(0.0f, Sword::MODEL_TOP_HEIGHT, 0.0f)	, m_worldMatrix));

	// パーティクルを生成
	CreateSwordParticle();
}


// --------------------------------
//  斬撃エフェクト生成処理
// --------------------------------
void SwordAttacking2::CreateSwordParticle()
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
		EventMessenger::Execute(EventList::CreateWeaponTrail, &ver);	// パーティクルの生成
	}
}


// --------------------------------
//  状態終了処理
// --------------------------------
void SwordAttacking2::PostUpdate()
{
	bool canHit = false;

	// 衝突可能フラグを敵全体に付与
	EventMessenger::Execute(EventList::EnemyCanHit, &canHit);
}


// --------------------------------
//  終了処理
// --------------------------------
void SwordAttacking2::Finalize()
{
}


// --------------------------------
//  衝突時イベント
// --------------------------------
void SwordAttacking2::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
