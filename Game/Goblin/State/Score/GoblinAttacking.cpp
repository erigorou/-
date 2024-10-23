#include "pch.h"
#include "../Header/GoblinAttacking.h"

// コンストラクタ
Goblin::GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_model(nullptr)
	, m_totalTime(0.0f)
{
}

// 終了処理
Goblin::GoblinAttacking::~GoblinAttacking()
{
}


// 初期化処理
void Goblin::GoblinAttacking::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
}


// 事前更新
void Goblin::GoblinAttacking::PreUpdate()
{
}


// 更新
void Goblin::GoblinAttacking::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;
}


// 事後更新
void Goblin::GoblinAttacking::PostUpdate()
{
}


// 描画
void Goblin::GoblinAttacking::Render
(
	ID3D11DeviceContext*				context		,
	DirectX::CommonStates*				states		,
	const DirectX::SimpleMath::Matrix&	view		,
	const DirectX::SimpleMath::Matrix&	projection
)
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	m_model->Draw(context, *states, world, view, projection);
}


// 終了処理
void Goblin::GoblinAttacking::Finalize()
{
}


