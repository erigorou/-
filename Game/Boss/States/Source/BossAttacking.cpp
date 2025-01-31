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
	void* object = EventMessenger::ExecuteGetter("GetPlayerObject");
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
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetAttacking());
	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceAttacking());
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
		m_boss->ChangeState(m_boss->GetBossIdling());
	}
}




// --------------------------------------
// �X�e�[�g�ύX����(out)
// --------------------------------------
void BossAttacking::PostUpdate()
{
	// ����̃X�e�[�g��ύX����
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());
	// ��̃X�e�[�g��ύX
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------------------------
// �I������
// --------------------------------------
void BossAttacking::Finalize()
{
}