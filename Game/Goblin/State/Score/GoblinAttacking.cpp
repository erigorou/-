#include "pch.h"
#include "../Header/GoblinAttacking.h"
#include "../../Goblin.h"
#include "Game/Player/Player.h"

#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/DebugString.h"

// �R���X�g���N�^
GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}

// �I������
GoblinAttacking::~GoblinAttacking()
{
}


// ����������
void GoblinAttacking::Initialize()
{
}


// ���O�X�V
void GoblinAttacking::PreUpdate()
{
	m_goblin->SetIsAttacking(true);
}


// �X�V
void GoblinAttacking::Update(const float& elapsedTime)
{
	// ���Ԃ����Z����
	m_totalTime += elapsedTime;

	SearchPlayer();
}



// ����X�V
void GoblinAttacking::PostUpdate()
{
	m_goblin->SetIsAttacking(false);
}

// �I������
void GoblinAttacking::Finalize()
{
}

// �v���C���[�̒T�����s���B
void GoblinAttacking::SearchPlayer()
{
	// �v���C���[�Ǝ��g�̈ʒu���擾
	DirectX::SimpleMath::Vector3 playerPos = m_goblin->GetPlayScene()->GetPlayer()->GetPosition();
	DirectX::SimpleMath::Vector3 goblinPos = m_goblin->GetPosition();

	// �v���C���[�̈ʒu��T��
	m_angle = Math::CalculationAngle(playerPos, goblinPos);
	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
	m_goblin->SetAngle(-m_angle);

	auto resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("GoblinAngle, %f", m_angle);
}


