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


const float Enemy_Sweeping::CHARGE_TIME = 1.0f;	// 振りかざし時間
const float Enemy_Sweeping::WINDUP_TIME = 1.3f;	// 待機
const float Enemy_Sweeping::ATTACK_TIME = 1.8f;	// 薙ぎ払い
const float Enemy_Sweeping::END_TIME	= 2.5f;	// 終了
const float Enemy_Sweeping::ROTATE_ANGLE = 20.0f;	// 振りかざし角度


// ----------------------------------
// コンストラクタ
// ----------------------------------
Enemy_Sweeping::Enemy_Sweeping(Enemy* enemy)
	:m_model()
	, m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
	m_easying = std::make_unique<EasyingFunctions>();
}


// ----------------------------------
// デストラクタ
// ----------------------------------
Enemy_Sweeping::~Enemy_Sweeping()
{
}


// ----------------------------------
// 初期化処理
// ----------------------------------
void Enemy_Sweeping::Initialize(DirectX::Model* model)
{
	// モデルの取得
	m_model = model;

	m_boundingSphereBody = DirectX::BoundingSphere();		// 境界球の生成と設定
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}



// ----------------------------------
// 事前更新処理
// ----------------------------------
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


// ----------------------------------
// 更新処理
// ----------------------------------
void Enemy_Sweeping::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX::SimpleMath;

	// 経過時間の計算
	m_totalSeconds += elapsedTime;

	float t = 0.0f;						// イージングに使用する変数
	static float targetAngle = 0.0f;	// 目標の角度

	if (m_totalSeconds <= CHARGE_TIME)	// 開始からためモーション中なら
	{
		t = m_totalSeconds / CHARGE_TIME;
		targetAngle = m_angle - ROTATE_ANGLE * m_easying->easeOutCirc(t);					// ためモーションを行う
	}

	else if (
		m_totalSeconds >= WINDUP_TIME &&	// 待機時間を過ぎた　かつ
		m_totalSeconds <= ATTACK_TIME)		// 攻撃時間中なら
	{
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		targetAngle = m_angle - ROTATE_ANGLE + ROTATE_ANGLE * m_easying->easeOutBack(t);	// 薙ぎ払いモーションを行う
	}


	if(m_totalSeconds > END_TIME)	m_enemy->ChangeState(m_enemy->GetEnemyIdling());		// 待機状態に遷移


	m_enemy->SetAngle(DirectX::XMConvertToRadians(targetAngle));							// 角度を設定

	// 当たり判定の位置を調整する
	m_boundingSphereBody.Center = parentPos;
}


// ----------------------------------
// 事後更新処理
// ----------------------------------
void Enemy_Sweeping::PostUpdate()
{
}


// ----------------------------------
// 描画処理
// ----------------------------------
void Enemy_Sweeping::CheckHitPlayerBody()
{

}


// ----------------------------------
// 描画処理
// ----------------------------------
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


// ----------------------------------
// 終了処理
// ----------------------------------
void Enemy_Sweeping::Finalize()
{
}
