#include "pch.h"
#include "../Header/GoblinAttacking.h"
#include "../../Goblin.h"
#include "Game/Player/Player.h"

#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/DebugString.h"

// コンストラクタ
GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}

// 終了処理
GoblinAttacking::~GoblinAttacking()
{
}


// 初期化処理
void GoblinAttacking::Initialize()
{
}


// 事前更新
void GoblinAttacking::PreUpdate()
{
	m_goblin->SetIsAttacking(true);
}


// 更新
void GoblinAttacking::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;

	SearchPlayer();
}



// 事後更新
void GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// 終了処理
void GoblinAttacking::Finalize()
{
}

// プレイヤーの探索を行う。
void GoblinAttacking::SearchPlayer()
{
	// プレイヤーと自身の位置を取得
	DirectX::SimpleMath::Vector3 playerPos = m_goblin->GetPlayScene()->GetPlayer()->GetPosition();
	DirectX::SimpleMath::Vector3 goblinPos = m_goblin->GetPosition();

	// プレイヤーの位置を探索
	m_angle = Math::CalculationAngle(playerPos, goblinPos);
	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
	m_goblin->SetAngle(-m_angle);

	auto resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("GoblinAngle, %f", m_angle);
}


