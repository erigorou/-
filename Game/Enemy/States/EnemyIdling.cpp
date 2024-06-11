#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/States/EnemyIdling.h"


// コンストラクタ
EnemyIdling::EnemyIdling(Enemy* enemy)
	:
	m_enemy(enemy),
	m_totalSeconds()
{
}


// デストラクタ
EnemyIdling::~EnemyIdling()
{
}


// 初期化処理
void EnemyIdling::Initialize()
{
}


// 事前更新処理
void EnemyIdling::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0;
}


// 更新処理
void EnemyIdling::Update(const float& elapsedTime)
{
	// このステートになってからの時間を計測
	m_totalSeconds += elapsedTime;



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
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();

	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("State:EnemyIdling");
}


void EnemyIdling::Finalize()
{
}