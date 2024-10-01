#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Enemy/States/Header/Enemy_Sweeping.h"



// コンストラクタ
Enemy_Sweeping::Enemy_Sweeping(Enemy* enemy)
	:m_model()
	, m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
	m_easying = std::make_unique<EasyingFunctions>();
}


// デストラクタ
Enemy_Sweeping::~Enemy_Sweeping()
{
}


// 初期化処理
void Enemy_Sweeping::Initialize(DirectX::Model* model)
{
	// モデルの取得
	m_model = model;

	m_boundingSphereBody = DirectX::BoundingSphere();		// 境界球の生成と設定
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}



// 事前更新処理
void Enemy_Sweeping::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	// 当たり判定をプレイヤーの位置に修正
	m_boundingSphereBody.Center = m_enemy->GetPosition();
	m_angle = DirectX::XMConvertToDegrees(m_enemy->GetAngle());

	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetSweeping());			// 金棒のステートを変更
}


// 更新処理
void Enemy_Sweeping::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;

	// 経過時間の計算
	m_totalSeconds += elapsedTime;

	float t = 0.0f;  // 正規化された経過時間
	static float targetAngle = 0.0f;

	float fastTime = 1.0f;
	float secondTime = 1.3f;
	float thirdTime = 1.8f;

	// 最初の0.2秒で -20度から 0度に回転
	if (m_totalSeconds <= fastTime) {
		t = m_totalSeconds / fastTime;  // 0 ~ 1 に正規化
		targetAngle = m_angle - 20.f * m_easying->easeOutCirc(t);  // -20度から 0度に回転

		targetAngle -= 180;
	}
	else if (m_totalSeconds >= secondTime && m_totalSeconds <= thirdTime) {
		t = (m_totalSeconds - secondTime) / (thirdTime - secondTime);  // 0 ~ 1 に正規化
		targetAngle = m_angle - 20.f + 20.f * m_easying->easeOutBack(t);  // -20度から 0度に回転

		targetAngle -= 180;
	}

	if(m_totalSeconds > 2.5f)
	{
		// 回転が完了したら次のステートに移行
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}

	m_enemy->SetAngle(DirectX::XMConvertToRadians(targetAngle + 180));

	// 当たり判定の位置を調整する
	m_boundingSphereBody.Center = parentPos;
}


// 事後更新処理
void Enemy_Sweeping::PostUpdate()
{
}

// プレイヤーの体との当たり判定を行う
void Enemy_Sweeping::CheckHitPlayerBody()
{

}


// 描画処理
void Enemy_Sweeping::Render(
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
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyAngle : %f", m_angle);
#endif // _DEBUG
}


void Enemy_Sweeping::Finalize()
{
}
