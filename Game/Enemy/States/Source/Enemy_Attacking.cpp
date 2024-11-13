#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Enemy/States/Header/Enemy_Attacking.h"


// コンストラクタ
Enemy_Attacking::Enemy_Attacking(Enemy* enemy)
	:m_model()
	,m_angle(0.f)
	,m_enemy(enemy)
	,m_totalSeconds()
{
}


// デストラクタ
Enemy_Attacking::~Enemy_Attacking()
{
}


// 初期化処理
void Enemy_Attacking::Initialize(DirectX::Model* model)
{
	m_model = model;										// モデルの取得
}


// 事前更新処理
void Enemy_Attacking::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 武器のステートを変更
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetAttacking());

	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceAttacking());
}


// 更新処理
void Enemy_Attacking::Update(const float& elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	// プレイヤーの座標を取得
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// 敵の座標を取得
	Vector3 parentPos = m_enemy->GetPosition();
	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);

	if (m_totalSeconds >= 2.5f)
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());	// 待機状態に遷移
}


// プレイヤーの体との当たり判定を行う
void Enemy_Attacking::CheckHitPlayerBody()
{

}


// 事後更新処理
void Enemy_Attacking::PostUpdate()
{
	// 武器のステートを変更する
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// 描画処理
void Enemy_Attacking::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);

#ifdef _DEBUG
#endif // _DEBUG
}


void Enemy_Attacking::Finalize()
{
}