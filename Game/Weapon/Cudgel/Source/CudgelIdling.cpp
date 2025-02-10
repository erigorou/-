// ----------------------------------------------------------
// 名前:	CudgelAttacking
// 
// 内容:	ボス鬼の武器の金棒　待機攻撃のステート
// 			ステートマシーンを使用
//
// 制作:	池田桜輔
// ----------------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Header/CudgelIdling.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// -----------------------------------------------
// コンストラクタ
// -----------------------------------------------
CudgelIdling::CudgelIdling(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position{},
	m_velocity{},
	m_angle{},
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
{
}

// -----------------------------------------------
// デストラクタ
// -----------------------------------------------
CudgelIdling::~CudgelIdling()
{
}

// -----------------------------------------------
// 初期化処理
// -----------------------------------------------
void CudgelIdling::Initialize()
{
	// ワールド行列の初期化
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
}

// -----------------------------------------------
// 事前処理
// -----------------------------------------------
void CudgelIdling::PreUpdate()
{
}

// -----------------------------------------------
// 更新処理
// -----------------------------------------------
void CudgelIdling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// 敵の座標を取得
	m_position = m_cudgel->GetBoss()->GetPosition();
	// 敵の回転を取得
	m_angle = m_cudgel->GetBoss()->GetAngle();

	// 大きさの設定　＆　リセット
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(Cudgel::CUDGEL_SCALE)
		// 原点の位置からすこしずらす
		*= DirectX::SimpleMath::Matrix::CreateTranslation(Cudgel::DIRECTION_ENEMY)
		// 剣全体の回転を行う
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)
		// 鬼の座標に移動する
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// ワールド行列を設定する
	m_cudgel->SetWorldMatrix(m_worldMatrix);
	// 当たり判定の位置を設定する
	m_cudgel->SetCollisionPosition(m_worldMatrix);
}

// -----------------------------------------------
// 事後処理
// -----------------------------------------------
void CudgelIdling::PostUpdate()
{
}

// -----------------------------------------------
// 終了処理
// -----------------------------------------------
void CudgelIdling::Finalize()
{
}

// -----------------------------------------------
// 衝突判定
// -----------------------------------------------
void CudgelIdling::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}