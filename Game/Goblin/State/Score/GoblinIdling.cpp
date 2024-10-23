#include "pch.h"
#include "../Header/GoblinIdling.h"
#include "../../Goblin.h"


Goblin::GoblinIdling::GoblinIdling(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}


Goblin::GoblinIdling::~GoblinIdling()
{
}



void Goblin::GoblinIdling::Initialize(DirectX::Model* model)
{
	m_model = model;
}


void Goblin::GoblinIdling::PreUpdate()
{
}



void Goblin::GoblinIdling::Update(const float& elapsedTime)
{
	m_totalTime += elapsedTime;
}



void Goblin::GoblinIdling::PostUpdate()
{
}


void Goblin::GoblinIdling::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// ƒ‚ƒfƒ‹‚Ì•`‰æ
	m_model->Draw(context, *states, DirectX::SimpleMath::Matrix::Identity, view, projection);
}


void Goblin::GoblinIdling::Finalize()
{
}