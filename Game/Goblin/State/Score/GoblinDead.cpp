#include "pch.h"
#include "../Header/GoblinDead.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Scene/PlayScene.h"
#include "Game/EnemyManager/EnemyManager.h"


// -------------------
// �R���X�g���N�^
// -------------------
GoblinDead::GoblinDead(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
	, m_scale(1.0f)
{
}


// -------------------
// �f�X�g���N�^
// -------------------
GoblinDead::~GoblinDead()
{
}


// -------------------
// ����������
// -------------------
void GoblinDead::Initialize()
{
}



// -------------------
// ������(in)
// -------------------
void GoblinDead::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_totalTime = 0.0f;
	m_scale = 1.0f;

	// �S�u�����̈ʒu���擾
	m_startPos = m_goblin->GetPosition();
	m_startPos_y = m_startPos.y;
}



// -------------------
// �X�V����
// -------------------
void GoblinDead::Update(const float& elapsedTime)
{
	// ���Ԃ����Z����
	m_totalTime += elapsedTime;

	// �A�j���[�V�������X�V����
	UpdateAnimation();

	// ��莞�Ԍo�߂�����
	if (m_totalTime > DEAD_TIME)	m_goblin->DeleteGoblin();
}


// -------------------
// �A�j���[�V�����X�V����
// -------------------
void GoblinDead::UpdateAnimation()
{
	// �C�[�W���O�A�j���[�V�������g���ăX�P�[����ύX����
	m_scale = 1 - Easing::easeInBack(m_totalTime / DEAD_TIME);

	// �C�[�W���O�A�j���[�V�������g���Ĉʒu��ύX����
	m_startPos.y = m_startPos_y - m_startPos_y * Easing::easeOutBounce(m_totalTime / DEAD_TIME);

	// �S�u�����Ɍv�Z��������^����
	m_goblin->SetScale(DirectX::SimpleMath::Vector3(m_scale, m_scale, m_scale));
	m_goblin->SetPosition(m_startPos);
}


// -------------------
// ������(out)
// -------------------
void GoblinDead::PostUpdate()
{
}


// -------------------
// �㏈�����s��
// -------------------
void GoblinDead::Finalize()
{
}