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



void Goblin::GoblinIdling::Initialize()
{
}


void Goblin::GoblinIdling::PreUpdate()
{
}



void Goblin::GoblinIdling::Update(const float& elapsedTime)
{
	// ŽžŠÔ‚ð‰ÁŽZ‚·‚é
	m_TotalTime += elapsedTime;
}



void Goblin::GoblinIdling::PostUpdate()
{
}


void Goblin::GoblinIdling::Finalize()
{
}