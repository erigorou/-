#include "pch.h"
#include "../Header/GoblinAttacking.h"

// コンストラクタ
Goblin::GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
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
	m_model = model;
}


// 事前更新
void Goblin::GoblinAttacking::PreUpdate()
{
}


// 更新
void Goblin::GoblinAttacking::Update(const float& elapsedTime)
{
	m_totalTime += elapsedTime;
}


// 事後更新
void Goblin::GoblinAttacking::PostUpdate()
{
}


// 描画
void Goblin::GoblinAttacking::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// モデルの描画
	m_model->Draw(context, *states, DirectX::SimpleMath::Matrix::Identity, view, projection);
}


// 終了処理
void Goblin::GoblinAttacking::Finalize()
{
}


