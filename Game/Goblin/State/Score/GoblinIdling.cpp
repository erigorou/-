#include "pch.h"
#include "../Header/GoblinIdling.h"
#include "../../Goblin.h"


GoblinIdling::GoblinIdling(Goblin* goblin)
	: m_goblin(goblin)
	, m_TotalTime(0.0f)
{
}


GoblinIdling::~GoblinIdling()
{
}



void GoblinIdling::Initialize()
{
}


void GoblinIdling::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;
}



void GoblinIdling::Update(const float& elapsedTime)
{
	// ŽžŠÔ‚ð‰ÁŽZ‚·‚é
	m_TotalTime += elapsedTime;

	// UŒ‚ƒXƒe[ƒg‚É‘JˆÚ‚·‚é
	if (m_TotalTime > 3.0f)
	{
		m_goblin->ChangeState(m_goblin->GetAttacking());
	}
}



void GoblinIdling::PostUpdate()
{
}


void GoblinIdling::Finalize()
{
}