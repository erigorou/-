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
#include "Game/Data/GameData.h"

std::unique_ptr<Sound> Sound::s_sound = nullptr;

// ------------------------------------------------
// �C���X�^���X��Ԃ��֐�
// ------------------------------------------------
Sound* const Sound::GetInstance()
{
	// �C���X�^���X����������Ă��Ȃ��ꍇ
	if (s_sound == nullptr)
	{
		// ��������
		s_sound.reset(new Sound());
	}
	// �C���X�^���X��Ԃ�
	return s_sound.get();
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
	, m_bgmVolume(1.0f)
	, m_seVolume(1.0f)
	, m_fadeValue(1.0f)
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
	LoadBGM(BGM_TYPE::TITLE, TITLE_BGM_PATH);
	LoadBGM(BGM_TYPE::PLAY, PLAY_BGM_PATH);
	LoadBGM(BGM_TYPE::WIN, WIN_BGM_PATH);
	LoadBGM(BGM_TYPE::LOSE, LOSE_BGM_PATH);

	// �ŏ���BGM���Z�b�g����
	m_soundBGM = m_bgmList[(size_t)BGM_TYPE::TITLE];
}

// ------------------------------------------------
// ���X�g�𐶐�����
// ------------------------------------------------
void Sound::CreateSEList()
{
	LoadSE(SE_TYPE::SYSTEM_OK, SYSTEM_OK_PATH);

	LoadSE(SE_TYPE::PLAYER_ATTACK, PLAYER_ATTACK_SE_PATH);
	LoadSE(SE_TYPE::PLAYER_ATTACK2, PLAYER_ATTACK2_SE_PATH);
	LoadSE(SE_TYPE::PLAYER_DAMAGED, PLAYER_DAMAGED_SE_PATH);

	LoadSE(SE_TYPE::BOSS_MOVE, ENEMY_MOVE_SE_PATH);
	LoadSE(SE_TYPE::BOSS_ATTACK, ENEMY_ATTACK_SE_PATH);
	LoadSE(SE_TYPE::BOSS_SWEEP, ENEMY_SWEEP_SE_PATH);
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
		assert(!"BGM������ꏊ��enum�������܂���I�I�I�I�I");

	// ���X�g�ɓo�^����
	m_bgmList.push_back(sound);
}

// ------------------------------------------------
// SE�t�@�C���̓ǂ݂���
// ------------------------------------------------
void Sound::LoadSE(SE_TYPE type, const char* filePath)
{
	// ���̃T�E���h���`������
	FMOD::Sound* sound;

	// ���T�E���h�ɓo�^������
	FMOD_RESULT result;
	result = m_system->createSound(filePath, FMOD_DEFAULT, nullptr, &sound);
	assert(result == FMOD_OK);

	if (m_seList.size() != (size_t)type)
		assert(!"SE������ꏊ��enum�������܂���I�I�I�I�I");

	// ���X�g�ɓo�^����
	m_seList.push_back(sound);
}

// ------------------------------------------------
// Sound�Đ�Update
// ------------------------------------------------
void Sound::Update()
{
	FMOD_RESULT result;

	// �t�F�[�h�̒l���擾
	s_sound->m_fadeValue = GameData::GetInstance()->GetFadeValue();

	// ��d�Đ����Ȃ�
	if (s_sound->m_channelBGM == nullptr)
	{
		result = s_sound->m_system->playSound(s_sound->m_soundBGM, nullptr, false, &s_sound->m_channelBGM);
		assert(result == FMOD_OK && "BGM �Đ����s�I");
	}

	s_sound->SetBGMVolume(s_sound->m_fadeValue);

	// FMOD�̃V�X�e�����X�V����
	result = s_sound->m_system->update();
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
// SE�̉��ʒ����@�\
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

// ------------------------------------------------
// BGM�̕ύX
// ------------------------------------------------
// �ǂ�BGM�𗬂���
// -----------------------------------------------
void Sound::ChangeBGM(Sound::BGM_TYPE type)
{
	// ���łɍĐ�����BGM���~����
	if (s_sound->m_channelBGM != nullptr)
		s_sound->m_channelBGM->stop();

	// BGM��ύX����
	s_sound->m_soundBGM = s_sound->m_bgmList[(size_t)type];

	// �V����BGM���Đ�����
	FMOD_RESULT result = s_sound->m_system->playSound(s_sound->m_soundBGM, nullptr, false, &s_sound->m_channelBGM);
	assert(result == FMOD_OK && "BGM �Đ����s�I");

	// ���ʒ������s��
	s_sound->SetBGMVolume(1.0f);
}

// ------------------------------------------------
// SE�̍Đ�
// ------------------------------------------------
// ����: type - �Đ�����SE�̎��
// -----------------------------------------------
void Sound::PlaySE(Sound::SE_TYPE type)
{
	// �Đ�����SE���擾
	FMOD::Sound* seSound = s_sound->m_seList[(size_t)type];

	// SE���Đ�
	FMOD_RESULT result = s_sound->m_system->playSound(seSound, nullptr, false, &s_sound->m_channelSE);
	assert(result == FMOD_OK && "SE �Đ����s�I");

	// ���ʒ������s���i�C�Ӂj
	s_sound->SetSEVolume(1.0f);  // �����ŔC�ӂ̉��ʂ�ݒ�ł��܂�
}