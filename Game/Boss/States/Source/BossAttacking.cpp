#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/Boss/States/Header/BossAttacking.h"
#include "Game/Weapon/WeaponState.h"

// --------------------------------------
// �R���X�g���N�^
// --------------------------------------
BossAttacking::BossAttacking(Boss* boss)
	:
	m_angle{},
	m_boss(boss),
	m_totalSeconds{}
{
}

// --------------------------------------
// �f�X�g���N�^
// --------------------------------------
BossAttacking::~BossAttacking()
{
}


// --------------------------------------
// ����������
// --------------------------------------
void BossAttacking::Initialize()
{
	// �v���C���[�̎擾
	void* object = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = object ? static_cast<IObject*>(object) : nullptr;
}


// --------------------------------------
// �X�e�[�g�X�V�����iin�j
// --------------------------------------
void BossAttacking::PreUpdate()
{
	// �o�ߎ��Ԃ�������
	m_totalSeconds = 0.0f;
	// ����̃X�e�[�g��ύX
	CudgelState state = CudgelState::Attack;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);

	// ��̃X�e�[�g��ύX
	FaceState face = FaceState::Attacking;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}


// --------------------------------------
// �X�V����
// --------------------------------------
void BossAttacking::Update(const float& elapsedTime)
{
	// �o�ߎ��Ԃ����Z
	m_totalSeconds += elapsedTime;

	// �v���C���[�̍��W���擾
	DirectX::SimpleMath::Vector3 playerPos = m_player->GetPosition();
	// �G�̍��W���擾
	DirectX::SimpleMath::Vector3 parentPos = m_boss->GetPosition();
	// �G���猩���v���C���[�̈ʒu���v�Z����
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	// �U�����܂ł̓v���C���[��ǔ�����悤�ɂ���
	if (Math::InTime(0.0f, m_totalSeconds, CHASE_TIME))
	{
		// �v���C���[��ǔ�
		m_boss->SetAngle(m_angle);
	}

	// ��ԑJ��
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// �X�e�[�g��ύX�i�ҋ@��ԁj
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);
	}
}




// --------------------------------------
// �X�e�[�g�ύX����(out)
// --------------------------------------
void BossAttacking::PostUpdate()
{
	// ����̃X�e�[�g��ύX
	CudgelState state = CudgelState::Idle;
	EventMessenger::Execute(EventList::ChangeCudgelState, &state);
	// ��̃X�e�[�g��ύX
	FaceState face = FaceState::Idling;
	EventMessenger::Execute(EventList::ChangeBossFace, &face);
}


// --------------------------------------
// �I������
// --------------------------------------
void BossAttacking::Finalize()
{
}