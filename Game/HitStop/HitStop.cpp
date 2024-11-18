// --------------------------------
// �q�b�g�X�g�b�v���v�Z����N���X
// --------------------------------

#include "pch.h"
#include "HitStop.h"
#include "Libraries/MyLib/EasingFunctions.h"

std::unique_ptr<HitStop> HitStop::s_hitStop = nullptr;

// ----------------------------
// �C���X�^���X�̎擾
// ----------------------------
HitStop* const HitStop::GetInstance()
{
	// �C���X�^���X����������Ă��Ȃ��ꍇ
	if (s_hitStop == nullptr)
	{
		//�@��������
		s_hitStop.reset(new HitStop);
	}
	// �C���X�^���X��Ԃ�
	return s_hitStop.get();
}


// ----------------------------
// �R���X�g���N�^
// ----------------------------
HitStop::HitStop()
	: m_isActive(false)
	, m_smoothDeltaTime(0.f)
	, m_totalSeconds(0.f)
{
}


// ----------------------------
// �f�X�g���N�^
// ----------------------------
HitStop::~HitStop()
{
}


// ----------------------------
// �X�V����
// ----------------------------
void HitStop::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ�o�^
	m_smoothDeltaTime = elapsedTime;
	// �����Ȃ��ꍇ�I��
	if ( ! m_isActive) return;
	// �q�b�g�X�g�b�v���̌v�Z���s��
	CalculateSmoothDeltaTime(elapsedTime);
}

// -----------------------------
// �q�b�g�X�g�b�v�̌v�Z�i�c�莞�ԁj
// -----------------------------
void HitStop::CalculateSmoothDeltaTime(float elapsedTime)
{
	// �o�ߎ��Ԃ̋L�^
	m_totalSeconds += elapsedTime;
	// �q�b�g�X�g�b�v�����̔���
	m_isActive = (m_totalSeconds < HITSTOP_TIME);

	if (m_totalSeconds < DELAY) return;

	// �C�[�W���O�p�̎��Ԃ��v�Z
	float t = Easing::easeOutBack(m_totalSeconds - DELAY) / (HITSTOP_TIME - DELAY);

	// �q�b�g�X�g�b�v�̏�Ԃɉ����Čo�ߎ��Ԃ�ݒ�@TRUE : FALSE
	m_smoothDeltaTime = m_isActive ? elapsedTime / t / 10.0f : (m_totalSeconds = 0.0f);
}
