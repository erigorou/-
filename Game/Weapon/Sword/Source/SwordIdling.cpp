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
	// 攻撃フラグをリセット
	m_sword->SetAttackFlag(false);

	// 敵全体を衝突不可にする
	bool canHit = false;

	// 衝突可能フラグを敵全体に付与
	EventMessenger::Execute(EventList::EnemyCanHit, &canHit);
}

// -----------------------
// 更新処理
// -----------------------
void SwordIdling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// ワールド行列の更新処理
	UpdateWorldMatrix();
}

// -----------------------
// ワールド行列の更新処理
// -----------------------
void SwordIdling::UpdateWorldMatrix()
{
	using namespace DirectX::SimpleMath;

	// プレイヤーを取得
	auto player = m_sword->GetPlayer();
	// プレイヤーの向きを取得
	float rotation = player->GetAngle();

	// 剣の基礎回転（初期角度）
	Quaternion baseRotation =
		Quaternion::CreateFromAxisAngle(Vector3::UnitY, INIT_Y_ROT) *
		Quaternion::CreateFromAxisAngle(Vector3::UnitX, INIT_Z_ROT);

	// プレイヤーの回転を適用
	Quaternion playerRotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -rotation);

	// ワールド行列を更新
	m_worldMatrix =
		// 剣のスケールを適用
		Matrix::CreateScale(Sword::SWORD_SCALE) *
		// 基本回転を適用
		Matrix::CreateFromQuaternion(baseRotation) *
		// 持ち手位置を適用
		Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER) *
		// プレイヤーの向きを適用
		Matrix::CreateFromQuaternion(playerRotation) *
		// プレイヤーの位置を適用
		Matrix::CreateTranslation(player->GetPosition());

	// 剣にワールド行列を設定
	m_sword->SetWorldMatrix(m_worldMatrix);
	// 剣の当たり判定位置を設定
	m_sword->SetCollisionPosition(m_worldMatrix);
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