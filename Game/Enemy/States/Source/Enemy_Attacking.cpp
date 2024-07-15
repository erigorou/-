#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"
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
	// モデルの取得
	m_model = model;

	// 体の当たり判定の生成
	m_boundingSphereBody = DirectX::BoundingSphere();
	// 体の当たり判定のサイズや座標を設定
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}


// 事前更新処理
void Enemy_Attacking::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 武器のステートを変更
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

}


// 更新処理
void Enemy_Attacking::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;

	// 経過時間の計算
	m_totalSeconds += elapsedTime;


	if (m_totalSeconds >= 2.5f)
	{
		// 2.5秒で待機状態に変更
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}
}


// プレイヤーの体との当たり判定を行う
void Enemy_Attacking::CheckHitPlayerBody()
{

}


// 事後更新処理
void Enemy_Attacking::PostUpdate()
{
	// 修正点があればここに記載
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

	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();

	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyAngle : %f", m_angle);
}


void Enemy_Attacking::Finalize()
{
}