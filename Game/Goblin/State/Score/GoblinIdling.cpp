#include "pch.h"
#include "../Header/GoblinIdling.h"
#include "../../Goblin.h"


Goblin::GoblinIdling::GoblinIdling(Goblin* goblin)
	: m_goblin(goblin)
{
}


Goblin::GoblinIdling::~GoblinIdling()
{
}



void Goblin::GoblinIdling::Initialize(DirectX::Model* model)
{
}


void Goblin::GoblinIdling::PreUpdate()
{
}



void Goblin::GoblinIdling::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
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
}


void Goblin::GoblinIdling::Finalize()
{
}