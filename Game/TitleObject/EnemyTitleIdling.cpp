#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "TitleEnemy.h"
#include "EnemyTitleIdling.h"
#include "EnemyTitleMoving.h"

// コンストラクタ
EnemyTitleIdling::EnemyTitleIdling(TitleEnemy* enemy)
	: m_enemy(enemy)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)* TitleEnemy::TITLE_ENEMY_SPEED)
	, m_model(nullptr)
	, m_angle(0.0f)
	, m_worldMat(DirectX::SimpleMath::Matrix::Identity)
	, m_totalSeconds(0.0f)
	, m_amplitude(1.0f)
	, m_finishTime(1.0f)
	, m_frequency(1.0f)
	, m_isJump(false)
	, m_canShake(false)
{
}


// デストラクタ
EnemyTitleIdling::~EnemyTitleIdling()
{
}


// 初期化処理
void EnemyTitleIdling::Initialize(DirectX::Model* model)
{
	using namespace DirectX;
	// モデルを取得
	m_model = model;
	// 速度を設定（前にしか動かない）
	m_velocity = SimpleMath::Vector3::Forward;
}


// 事前更新処理
void EnemyTitleIdling::PreUpdate()
{
	using namespace DirectX::SimpleMath;

	// 経過時間を初期化
	m_totalSeconds = 0.0f; 

	m_position = Vector3::Zero;
}


// 更新処理
void EnemyTitleIdling::Update(const float& elapsedTime)
{

	using namespace DirectX::SimpleMath;
	
	// 合計の時間を計算する
	m_totalSeconds += elapsedTime;


	if (m_totalSeconds > TitleEnemy::COOL_TIME)
	{
		m_enemy->ChangeState(m_enemy->GetTitleEnemyTitleMoving());
	}


	// 回転行列の作成
	Matrix angleMat  = Matrix::CreateScale(TitleEnemy::TITLE_ENEMY_SCALE)
					*= Matrix::CreateRotationY(-m_angle);

	// 回転角を設定する
	m_enemy->SetAngle(m_angle);

	// 座標を設定する
	m_enemy->SetPosition(m_position);
}




// 事後更新処理
void EnemyTitleIdling::PostUpdate()
{
	// ワールド行列を全体に設定する
	m_enemy->SetWorldMatrix(m_worldMat);
	// 敵の位置を0で固定する
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);
}


// 描画処理
void EnemyTitleIdling::Render(
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
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();

	UNREFERENCED_PARAMETER(debugString);
#endif // _DEBUG
}


void EnemyTitleIdling::Finalize()
{
}