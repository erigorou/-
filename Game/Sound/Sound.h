#pragma once

// FMOD�p�̑O���錾
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class Sound
{
public:
	// �񋓌^-------------------
	enum class BGM_TYPE : unsigned int
	{
		TITLE,
		PLAY,
		WIN,
		LOSE
	};

	enum class SE_TYPE : unsigned int
	{
		SYSTEM_OK,
		PLAYER_ATTACK,
		PLAYER_ATTACK2,
		PLAYER_DAMAGED,
		BOSS_MOVE,
		BOSS_ATTACK,
		BOSS_SWEEP
	};

	// �Œ�l
	static constexpr const char* TITLE_BGM_PATH = "Resources/Sounds/BGM/Title.mp3";
	static constexpr const char* PLAY_BGM_PATH = "Resources/Sounds/BGM/Play.mp3";
	static constexpr const char* WIN_BGM_PATH = "Resources/Sounds/BGM/Win.mp3";
	static constexpr const char* LOSE_BGM_PATH = "Resources/Sounds/BGM/Lose.mp3";

	static constexpr const char* SYSTEM_OK_PATH = "Resources/Sounds/SE/System/OK.mp3";
	static constexpr const char* PLAYER_ATTACK_SE_PATH = "Resources/Sounds/SE/Player/Attack.mp3";
	static constexpr const char* PLAYER_ATTACK2_SE_PATH = "Resources/Sounds/SE/Player/Attack2.mp3";
	static constexpr const char* PLAYER_DAMAGED_SE_PATH = "Resources/Sounds/SE/Player/DamagedSE.mp3";
	static constexpr const char* ENEMY_MOVE_SE_PATH = "Resources/Sounds/SE/Enemy/Move.mp3";
	static constexpr const char* ENEMY_ATTACK_SE_PATH = "Resources/Sounds/SE/Enemy/Attack.mp3";
	static constexpr const char* ENEMY_SWEEP_SE_PATH = "Resources/Sounds/SE/Enemy/Sweep.mp3";
	// �A�N�Z�T�֐�
public:

	// BGM�Đ��N���X�̃C���X�^���X���擾����
	static Sound* const GetInstance();

	// ���ʂ𒲐�����֐�
	void SetBGMVolume(float volume);
	void SetSEVolume(float volume);

	static void ChangeBGM(Sound::BGM_TYPE type);
	static void PlaySE(Sound::SE_TYPE type);

	// �f�X�g���N�^
	~Sound();

	// �X�V����
	static void Update();

	// SOUND�Ŏg�p�����I�u�W�F�N�g�����
	void FinalizeFMOD();

private:
	// �R���X�g���N�^
	Sound();
	// FMOD�̃V�X�e���̏�����
	void InitializeFMOD();
	// BGM���X�g�̐���
	void CreateBGMList();
	// SE���X�g�̐���
	void CreateSEList();

	//�@BGM�ǂݍ���
	void LoadBGM(BGM_TYPE	type, const char* filePath);
	void LoadSE(SE_TYPE	type, const char* filePath);

private:
	static std::unique_ptr<Sound> s_sound;	// �C���X�^���X

	FMOD::System* m_system;		// FMOD�̃V�X�e��
	FMOD::Sound* m_soundBGM;		// BGM�p�̉����f�[�^
	FMOD::Channel* m_channelBGM;	// BGM���Đ�����`�����l��
	FMOD::Channel* m_channelSE;	// SE���Đ�����`�����l��

	std::vector<FMOD::Sound*> m_bgmList;	// BGM�p�̉����f�[�^���X�g
	std::vector<FMOD::Sound*> m_seList;		// SE�p�̉����f�[�^���X�g

	float m_bgmVolume;	// BGM�̉���
	float m_seVolume;	// SE�̉���

	float m_fadeValue;	// ��ʂ̃t�F�[�h��(�t�F�[�h�ɉ�����BGM���s���B)
};