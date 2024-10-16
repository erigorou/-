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


/// <summary>
/// �R���X�g���N�^
/// </summary>
BGM_Player::BGM_Player()
	: m_system(nullptr)
	, m_soundBGM(nullptr)
	, m_channelBGM(nullptr)
{
	InitializeFMOD(BGM_FILE_PATH);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
BGM_Player::~BGM_Player()
{
	FinalizeFMOD();
}


/// <summary>
/// FMOD�̃V�X�e�������������A�w�肳�ꂽ�t�@�C���p�X��BGM�����[�h����B
/// </summary>
/// <param name="filePath">���[�h����BGM�t�@�C���̃p�X</param>
void BGM_Player::InitializeFMOD(const char* filePath)
{
    // �V�X�e�����C���X�^���X������
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    assert(result == FMOD_OK);

    // �V�X�e��������������
    result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
    assert(result == FMOD_OK);

    // BGM�����[�h����
    result = m_system->createSound(filePath, FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
    assert(result == FMOD_OK);

}

/// <summary>
/// BGM�̍Đ����J�n���AFMOD�̃V�X�e�����X�V����B
/// </summary>
void BGM_Player::Update()
{
    FMOD_RESULT result;

    // ��d�Đ����Ȃ�
    if (m_channelBGM == nullptr)
    {
        result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
        assert(result == FMOD_OK);

        // ���ʒ������s��
        SetVolume(0.05f);
    }

    // FMOD�̃V�X�e�����X�V����
    result = m_system->update();
    assert(result == FMOD_OK);
}

/// <summary>
/// FMOD�̃V�X�e���ƃT�E���h�I�u�W�F�N�g���������B
/// </summary>
void BGM_Player::FinalizeFMOD()
{
    // Sound�p�̃I�u�W�F�N�g���������
    m_soundBGM->release();
    m_system->release();
}

/// <summary>
/// �Đ�����BGM�̉��ʂ�ݒ肷��B
/// </summary>
/// <param name="volume">�ݒ肷�鉹�ʁi0.0����1.0�͈̔́j</param>
void BGM_Player::SetVolume(float volume)
{
    if (m_channelBGM != nullptr)
    {
        m_channelBGM->setVolume(volume);
    }
}
