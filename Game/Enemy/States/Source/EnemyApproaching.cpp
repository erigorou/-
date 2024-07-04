#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/States/Header/EnemyApproaching.h"


// コンストラクタ
EnemyApproaching::EnemyApproaching(Enemy* enemy)
	:
	m_enemy(enemy),
	m_position(0.f, 0.f, 0.f),
	m_velocity(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f)* Enemy::ENEMY_SPEED),
	m_angle(0.f),
	m_worldMat(DirectX::SimpleMath::Matrix::Identity),
	m_totalSeconds(0.f),
	m_amplitude(1.f),
	m_finishTime(1.f),
	m_frequency(1.0f)
{
}


// デストラクタ
EnemyApproaching::~EnemyApproaching()
{
}


// 初期化処理
void EnemyApproaching::Initialize(DirectX::Model* model)
{
	using namespace DirectX;
	// 速度を設定（前にしか動かない）
	m_velocity = SimpleMath::Vector3::Forward;
	// 体の当たり判定の生成
	m_boundingSphereBody = BoundingSphere();
	// 体の当たり判定のサイズや座標を設定
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;
}


// 事前更新処理
void EnemyApproaching::PreUpdate()
{
	using namespace DirectX::SimpleMath;

	// 経過時間を初期化
	m_totalSeconds = 0.f; 
	// 敵の座標を取得
	m_position = m_enemy->GetPosition();
	// 初期の回転角を設定
	m_angle = m_enemy->GetAngle();
	// ワールド行列の設定
	m_worldMat = m_enemy->GetWorldMatrix();
}


// 更新処理
void EnemyApproaching::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	//// elapsedTime使わないけどエラー出さないでねって文
	//UNREFERENCED_PARAMETER(elapsedTime);
	
	// 合計の時間を計算する
	m_totalSeconds += elapsedTime;

	// サイン波の計算(上下移動)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// 敵に近づく処理
	using namespace DirectX::SimpleMath;
	// プレイヤーの座標を取得
	Vector3 playerPos = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();


	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(m_position, playerPos);
	
	// 回転行列の作成
	Matrix angleMat  = Matrix::CreateScale(Enemy::ENEMY_SCALE)
					*= Matrix::CreateRotationY(-m_angle);
	// 前方に移動
	m_position += Vector3::Transform(m_velocity, angleMat);


	// 体の境界球の位置を更新
	m_boundingSphereBody.Center = m_position;
	m_boundingSphereBody.Center.y = 0;

	// 2秒経過で待機モーションに変更
	if (m_totalSeconds >= 2.f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}

	// 回転角を設定する
	m_enemy->SetAngle(m_angle);

	// 座標を設定する
	m_enemy->SetPosition(m_position);
}


// プレイヤーの体との当たり判定を行う
void EnemyApproaching::CheckHitPlayerBody()
{
}


// 事後更新処理
void EnemyApproaching::PostUpdate()
{
	// ワールド行列を全体に設定する
	m_enemy->SetWorldMatrix(m_worldMat);
	// 敵の位置を0で固定する
	m_position.y = 0.f;
	m_enemy->SetPosition(m_position);
}


// 描画処理
void EnemyApproaching::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context, states, view, projection, m_model);



	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
}


void EnemyApproaching::Finalize()
{
}