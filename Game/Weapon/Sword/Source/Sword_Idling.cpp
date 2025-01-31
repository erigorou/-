/// ---------------------------
///
/// プレイヤーの剣の待機状態
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Header/Sword_Idling.h"

// -----------------------
// コンストラクタ
// -----------------------
Sword_Idling::Sword_Idling(Sword* sword)
	:
	m_sword(sword),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}


// -----------------------
// デストラクタ
// -----------------------
Sword_Idling::~Sword_Idling()
{
}


// -----------------------
// 初期化処理
// -----------------------
void Sword_Idling::Initialize()
{
	// ワールド行列を初期化
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
}


// -----------------------
// 事前処理
// -----------------------
void Sword_Idling::PreUpdate()
{
	m_sword->SetAttackFlag(false);

	// 敵全体を衝突不可にする
	bool canHit = false;

	// 衝突可能フラグを敵全体に付与
	EventMessenger::Execute("EnemyCanHit", &canHit);
}


// -----------------------
// 更新処理
// -----------------------
void Sword_Idling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// プレイヤーを取得
	auto player = m_sword->GetPlayer();
	// プレイヤーの座標を取得
	m_position = player->GetPosition();
	// プレイヤーの回転を取得
	m_angle = player->GetAngle();

	// ワールド行列を更新する
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);								// サイズの設定
	
	m_worldMatrix
		// 傾ける
		*= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(-90.0f))// 回転
		*= DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90.0f))	// 回転
		*= DirectX::SimpleMath::Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER)	// プレイヤの位置に設定する
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)						// プレイヤの回転を付与
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);					// プレイヤの位置に設定する

	m_sword->SetWorldMatrix(m_worldMatrix);			// ワールド行列を設定
	m_sword->SetCollisionPosition(m_worldMatrix);	// 当たり判定の位置を設定
}


// -----------------------
// 事後処理
// -----------------------
void Sword_Idling::PostUpdate()
{
}


// -----------------------
// 終了処理
// -----------------------
void Sword_Idling::Finalize()
{
}


// -----------------------
// 当たり判定処理
// -----------------------
void Sword_Idling::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
