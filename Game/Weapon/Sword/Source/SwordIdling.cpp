/// ---------------------------
///
/// プレイヤーの剣の待機状態
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
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Header/SwordIdling.h"

// -----------------------
// コンストラクタ
// -----------------------
SwordIdling::SwordIdling(Sword* sword)
	:
	m_sword(sword),
	m_position{},
	m_velocity{},
	m_angle{},
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}

// -----------------------
// デストラクタ
// -----------------------
SwordIdling::~SwordIdling()
{
}

// -----------------------
// 初期化処理
// -----------------------
void SwordIdling::Initialize()
{
	// ワールド行列を初期化
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
}

// -----------------------
// 事前処理
// -----------------------
void SwordIdling::PreUpdate()
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
void SwordIdling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// プレイヤーを取得
	auto player = m_sword->GetPlayer();
	// プレイヤーの座標を取得
	m_position = player->GetPosition();
	// プレイヤーの回転を取得
	m_angle = player->GetAngle();

	// クオータニオンによる回転計算
	DirectX::SimpleMath::Quaternion baseRotation =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(-90.0f)) *
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitX, DirectX::XMConvertToRadians(90.0f));

	DirectX::SimpleMath::Quaternion playerRotation =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(m_angle));

	// 回転行列に変換
	DirectX::SimpleMath::Matrix baseRotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(baseRotation);

	DirectX::SimpleMath::Matrix playerRotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(playerRotation);

	// ワールド行列を更新する
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(Sword::SWORD_SCALE)
		* baseRotationMatrix // まずXY軸の回転を適用
		* DirectX::SimpleMath::Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER) // 持ち手位置を適用
		* playerRotationMatrix // 最後にm_angleを適用
		* DirectX::SimpleMath::Matrix::CreateTranslation(m_position); // 最後にプレイヤーの位置を適用

	m_sword->SetWorldMatrix(m_worldMatrix); // ワールド行列を設定
	m_sword->SetCollisionPosition(m_worldMatrix); // 当たり判定の位置を設定
}

// -----------------------
// 事後処理
// -----------------------
void SwordIdling::PostUpdate()
{
}

// -----------------------
// 終了処理
// -----------------------
void SwordIdling::Finalize()
{
}

// -----------------------
// 当たり判定処理
// -----------------------
void SwordIdling::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
