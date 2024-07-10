// ---------------------------
// *
// * FMOD���g�p����BGM�𗬂�
// *
// ---------------------------

#include "pch.h"
#include "Game/CommonResources.h"

#include "Game/Sound/BGM_Player.h"

#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"

void BGM_Player::InitializeFMOD(const char* filePath)
{
	// �V�X�e�����C���X�^���X������
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// �V�X�e��������������
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// BGM�����[�h����
	//result = m_system->createSound("Resources/Sounds/maou_bgm_8bit29.ogg", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	result = m_system->createSound(filePath, FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);
}

void BGM_Player::Update()
{
	FMOD_RESULT result;

	// ��d�Đ����Ȃ�
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
	}

	// FMOD�̃V�X�e�����X�V����
	result = m_system->update();
	assert(result == FMOD_OK);
}


void BGM_Player::FinalizeFMOD()
{
	// Sound�p�̃I�u�W�F�N�g���������
	m_soundBGM->release();
	m_system->release();
}