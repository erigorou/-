#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "../../States/Header/EnemyDashAttacking.h"


// コンストラクタ
EnemyDashAttacking::EnemyDashAttacking(Enemy* enemy)
	:m_model()
	, m_angle(0.f)
	, m_enemy(enemy)
	, m_totalSeconds()
{
}


// デストラクタ
EnemyDashAttacking::~EnemyDashAttacking()
{
}


// 初期化処理
void EnemyDashAttacking::Initialize(DirectX::Model* model)
{
	m_model = model;										// モデルの取得
}


// 事前更新処理
void EnemyDashAttacking::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;

	//// 武器のステートを変更
	//auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	//cudgel->ChangeState(cudgel->GetAttacking());

	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceAttacking());
}


// 更新処理
void EnemyDashAttacking::Update(const float& elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// 敵の挙動を更新する
	UpdateAction(elapsedTime);


	if (m_totalSeconds >= 4.3f)
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());	// 待機状態に遷移
}

// --------------------
// 敵の挙動更新処理
// --------------------
void EnemyDashAttacking::UpdateAction(float elapsedTime)
{

	// ためモーションの時
	if (m_totalSeconds <= CHARGE_TIME)		ChargeAction();	// 貯め
	else if (m_totalSeconds <= DASH_TIME)	DashAction(elapsedTime);	// ダッシュ
	else if (m_totalSeconds <= WAIT_TIME)	WaitAction();	// 待機
	else if (m_totalSeconds <= RETURN_TIME)	ReturnAction();	// 元に戻る
}


// --------------------
// 貯め
// --------------------
void EnemyDashAttacking::ChargeAction()
{
	// プレイヤーの座標を取得
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();
	// 敵の座標を取得
	Vector3 parentPos = m_enemy->GetPosition();
	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	// プレイヤーの方向を探索する
	m_enemy->SetAngle(m_angle);
}


// --------------------
// ダッシュ
// --------------------
void EnemyDashAttacking::DashAction(float elapsedTime)
{
	// 現在の時間に基づいてサイン波で加速度を計算
	float t = (m_totalSeconds - CHARGE_TIME) / (DASH_TIME - CHARGE_TIME);
	float accelerationFactor = sin(t * 2.0f); // サイン波で速度を変化
	
	// 敵の座標を取得
	Vector3 position = m_enemy->GetPosition();

	// 敵の向きに基づいて前方向を計算
	DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);

	// 速度を計算
	Vector3 velocity = Vector3(0, 0, -MAX_SPEED * accelerationFactor);

	position += Vector3::Transform(velocity, rotationMatrix) * elapsedTime;



	// 敵の座標を設定
	m_enemy->SetPosition(position);
}


// --------------------
// 待機
// --------------------
void EnemyDashAttacking::WaitAction()
{
}


// --------------------
// 元に戻る
// --------------------
void EnemyDashAttacking::ReturnAction()
{
}





// 事後更新処理
void EnemyDashAttacking::PostUpdate()
{
	// 武器のステートを変更する
	auto cudgel = m_enemy->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_enemy->SetFace(m_enemy->GetFaceIdling());
}


// 描画処理
void EnemyDashAttacking::Render(
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


void EnemyDashAttacking::Finalize()
{
}