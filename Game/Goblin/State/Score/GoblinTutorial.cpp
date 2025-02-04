#include "pch.h"
#include "../Header/GoblinTutorial.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"

// ---------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="goblin">�S�u�����̃|�C���^</param>
// ---------------------------------------------
GoblinTutorial::GoblinTutorial(Goblin* goblin)
	: 
	m_goblin{ goblin },
	m_TotalTime{}
{
}

// ---------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ---------------------------------------------
GoblinTutorial::~GoblinTutorial()
{
}

// ---------------------------------------------
/// <summary>
/// ������
/// </summary>
// ---------------------------------------------
void GoblinTutorial::Initialize()
{
	// �����_���ɃN�[���^�C����ݒ肷��
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}

// ---------------------------------------------
/// <summary>
/// ������(in)
/// </summary>
// ----------------------------------------------
void GoblinTutorial::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_TotalTime = 0.0f;

	// �����_���ɃN�[���^�C����ݒ肷��
	m_coolTime = Math::RandomFloat(MIN_COOLTIME, MAX_COOLTIME);
}

// ---------------------------------------------
/// <summary>
/// �X�V
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
// ---------------------------------------------
void GoblinTutorial::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

// ---------------------------------------------
/// <summary>
/// ������(out)
/// </summary>
// ---------------------------------------------
void GoblinTutorial::PostUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// �I������
/// </summary>
// ---------------------------------------------
void GoblinTutorial::Finalize()
{
}