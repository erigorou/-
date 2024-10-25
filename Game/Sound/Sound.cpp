// ---------------------------
// *
// * FMOD���g�p����BGM�𗬂�
// *
// ---------------------------

#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Sound/Sound.h"
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"

std::unique_ptr<Sound> Sound::s_bgmPlayer = nullptr;


// ------------------------------------------------
// �C���X�^���X��Ԃ��֐�
// ------------------------------------------------
Sound* const Sound::GetInstance()
{
	// �C���X�^���X����������Ă��Ȃ��ꍇ
	if (s_bgmPlayer == nullptr)
	{
		// ��������
		s_bgmPlayer.reset(new Sound());
	}
	// �C���X�^���X��Ԃ�
	return s_bgmPlayer.get();
}


// ------------------------------------------------
// �R���X�g���N�^
// ------------------------------------------------
Sound::Sound()
    : m_system(nullptr)
    , m_soundBGM(nullptr)
    , m_channelBGM(nullptr)
    , m_channelSE(nullptr)
    , m_bgmList()
    , m_seList()
{
	InitializeFMOD();
}

// ------------------------------------------------
// �f�X�g���N�^
// ------------------------------------------------
Sound::~Sound()
{
	FinalizeFMOD();
}


// ------------------------------------------------
// �������֐�(�R���X�g���N�^�œǂ�ł�)
// ------------------------------------------------
/// <param name="filePath">���[�h����BGM�t�@�C���̃p�X</param>
void Sound::InitializeFMOD()
{
    // �V�X�e�����C���X�^���X������
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    assert(result == FMOD_OK);

    // �V�X�e��������������
    result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
    assert(result == FMOD_OK);

	// BGM���X�g�𐶐�����
	CreateBGMList();

	// SE���X�g�𐶐�����
    CreateSEList();
}

// ------------------------------------------------
// ���X�g�𐶐�����
// ------------------------------------------------
void Sound::CreateBGMList()
{
    LoadBGM(BGM_TYPE::TITLE,    TITLE_BGM_PATH  );
	LoadBGM(BGM_TYPE::PLAY,     PLAY_BGM_PATH   );
	LoadBGM(BGM_TYPE::WIN,      WIN_BGM_PATH    );
	LoadBGM(BGM_TYPE::LOSE,     LOSE_BGM_PATH   );

    // �ŏ���BGM���Z�b�g����
	m_soundBGM = m_bgmList[(size_t)BGM_TYPE::TITLE];
}


// ------------------------------------------------
// ���X�g�𐶐�����
// ------------------------------------------------
void Sound::CreateSEList()
{

}


// ------------------------------------------------
// BGM�t�@�C���̓ǂݍ���
// ------------------------------------------------
void Sound::LoadBGM(BGM_TYPE type, const char* filePath)
{
    // ���̃T�E���h���`������
    FMOD::Sound* sound;

    // ���T�E���h�ɓo�^������
	FMOD_RESULT result;
	result = m_system->createSound(filePath, FMOD_LOOP_NORMAL, nullptr, &sound);
	assert(result == FMOD_OK);

    if (m_bgmList.size() != (size_t)type)
        assert(! "BGM������ꏊ��enum�������܂���I�I�I�I�I");

    // ���X�g�ɓo�^����
	m_bgmList.push_back(sound);
}

// ------------------------------------------------
// BGM�t�@�C���̓ǂݍ���
// ------------------------------------------------



// ------------------------------------------------
// BGM�Đ�Update
// ------------------------------------------------
void Sound::Update()
{
    FMOD_RESULT result;

    // ��d�Đ����Ȃ�
    if (s_bgmPlayer->m_channelBGM == nullptr)
    {
        result = s_bgmPlayer->m_system->playSound(s_bgmPlayer->m_soundBGM, nullptr, false, &s_bgmPlayer->m_channelBGM);
        assert(result == FMOD_OK && "BGM �Đ����s�I");

        // ���ʒ������s��
        s_bgmPlayer->SetBGMVolume(0.2f);
    }

    // FMOD�̃V�X�e�����X�V����
    result = s_bgmPlayer->m_system->update();
    assert(result == FMOD_OK);
}


// ------------------------------------------------
// �I������
// ------------------------------------------------
void Sound::FinalizeFMOD()
{
    // Sound�p�̃I�u�W�F�N�g���������
    m_soundBGM->release();
    m_system->release();
}


// ------------------------------------------------
// BGM�̉��ʒ����@�\
// ------------------------------------------------
// ���ʁ@���@MAX 1 : MIN 0
// -----------------------------------------------
void Sound::SetBGMVolume(float volume)
{
    if (m_channelBGM != nullptr)
    {
        m_channelBGM->setVolume(volume);
    }
}


// ------------------------------------------------
// BGM�̉��ʒ����@�\
// ------------------------------------------------
// ���ʁ@���@MAX 1 : MIN 0
// -----------------------------------------------
void Sound::SetSEVolume(float volume)
{
    if (m_channelSE != nullptr)
    {
        m_channelSE->setVolume(volume);
    }
}
