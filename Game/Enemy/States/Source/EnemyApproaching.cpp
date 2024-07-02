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
	m_totalSeconds()
{
}


// デストラクタ
EnemyApproaching::~EnemyApproaching()
{
}


// 初期化処理
void EnemyApproaching::Initialize(DirectX::Model* model)
{
	// モデルの取得
	m_model = model;

	// 体の当たり判定の生成
	m_boundingSphereBody = DirectX::BoundingSphere();
	// 体の当たり判定のサイズや座標を設定
	m_boundingSphereBody.Radius = Enemy::ENEMY_SCALE * 12.f;

	// 振れ幅
	m_amplitude = 1.f;
	// 周波数
	m_frequency = 2.f;
}


// 事前更新処理
void EnemyApproaching::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.f;
}


// 更新処理
void EnemyApproaching::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	// elapsedTime使わないけどエラー出さないでねって文
	UNREFERENCED_PARAMETER(elapsedTime);
	
	m_totalSeconds += elapsedTime;

	// サイン波の計算(上下移動)
	parentPos.y = Math::CalculatingSinWave(m_totalSeconds, m_amplitude, m_frequency);

	// 敵に近づく
	using namespace DirectX::SimpleMath;
	Vector3 enemyPosition = m_enemy->GetPlayScene()->GetPlayer()->GetPosition();


	// 体の境界球の位置を更新
	m_boundingSphereBody.Center = parentPos;

	// 2秒経過で待機モーションに変更
	if (m_totalSeconds >= 2.f)
	{
		m_enemy->ChangeState(m_enemy->GetEnemyIdling());
	}
}


// プレイヤーの体との当たり判定を行う
void EnemyApproaching::CheckHitPlayerBody()
{
}


// 事後更新処理
void EnemyApproaching::PostUpdate()
{
	// 修正点があればここに記載
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