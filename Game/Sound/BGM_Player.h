#pragma once

// FMOD�p�̑O���錾
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class BGM_Player
{
public:
	// FMOD�̃V�X�e���̏������Ɖ����f�[�^�̃��[�h
	void InitializeFMOD(const char* filePath);

	// �X�V����
	void Update();

	// SOUND�Ŏg�p�����I�u�W�F�N�g�����
	void FinalizeFMOD();
private:
	FMOD::System* m_system;	// FMOD�̃V�X�e��
	FMOD::Sound* m_soundBGM;	// BGM�p�̉����f�[�^
	FMOD::Channel* m_channelBGM;// BGM���Đ�����`�����l��
};