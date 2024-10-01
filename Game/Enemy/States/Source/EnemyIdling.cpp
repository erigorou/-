#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

// 関数を使用するのに必要な引数
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"

// ヘッダーファイル
#include "Game/Enemy/States/Header/EnemyIdling.h"


// コンストラクタ
EnemyIdling::EnemyIdling(Enemy* enemy)
	:
	m_angle(0.f),
	m_enemy(enemy),
	m_totalSeconds()
{
}


// デストラクタ
EnemyIdling::~EnemyIdling()
{
}


// 初期化処理
void EnemyIdling::Initialize(DirectX::Model* model)
{
	m_model = model;
	m_boundingSphereBody = DirectX::BoundingSphere();			// モデルを取得し、
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;	// 
}


// 事前更新処理
void EnemyIdling::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0;
	// 武器のステートを変更
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());
}


// 更新処理
void EnemyIdling::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;
	m_totalSeconds += elapsedTime;

	// プレイヤーの座標を取得
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);

	// 体の境界球の位置を更新
	m_boundingSphereBody.Center = parentPos;

	// 回転角を設定する
	m_enemy->SetAngle(m_angle);

	if (m_totalSeconds >= 2.f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemySweeping());	// 薙ぎ払い

		// 近い距離だと追従を行わない
		// プレイヤーとの距離を計算
		float distance = Vector3::Distance(parentPos, playerPos);

		//if (distance > 25.0f)
		//	m_enemy->ChangeState(m_enemy->GetEnemyApproaching());	// 追従
		//else
		//	m_enemy->ChangeState(m_enemy->GetEnemyAttacking());		// 待機
	}
}


// プレイヤーの体との当たり判定を行う
void EnemyIdling::CheckHitPlayerBody()
{

}


// 事後更新処理
void EnemyIdling::PostUpdate()
{
	// 修正点があればここに記載
}


// 描画処理
void EnemyIdling::Render(
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


#ifdef _DEBUG
	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyAngle : %f", m_angle);
#endif // _DEBUG
}


void EnemyIdling::Finalize()
{
}