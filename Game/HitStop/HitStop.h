// ---------===========------------
// �q�b�g�X�g�b�v�֘A�N���X
// ---------===========------------

#ifndef HITSTOP_H
#define HITSTOP_H

#pragma once
#include "pch.h"

class HitStop
{
// �Œ�l ================
public:

	static constexpr float DELAY = 0.05f;					// �q�b�g�X�g�b�v�̒x������
	static constexpr float HITSTOP_TIME = DELAY + 0.1f;	// �q�b�g�X�g�b�v����


// �ÓI�֐��@=============
	// �C���X�^���X�̎擾
	static HitStop* const GetInstance();

// �A�N�Z�T�֐� ==========
public:
	// �q�b�g�X�g�b�v���L����
	bool IsActive() const { return m_isActive; }
	// �q�b�g�X�g�b�v��L���ɂ���
	void SetActive() { m_isActive = true; }


	inline float GetSmoothDeltaTime() const { return m_smoothDeltaTime; }

// ���J�֐� ============
public:
	// �R���X�g���N�^
	HitStop();
	// �f�X�g���N�^
	~HitStop();
	// �X�V����
	void Update(float elapsedTime);


// ����J�֐� ==========
private:
	void CalculateSmoothDeltaTime(float elapsedTime);


// �����ϐ� ==========
private:
	// �C���X�^���X
	static std::unique_ptr<HitStop> s_hitStop;

	// �q�b�g�X�g�b�v���L����
	bool m_isActive;
	// �q�b�g�X�g�b�v�֘A�̎���
	float m_smoothDeltaTime;
	// �o�ߎ���
	float m_totalSeconds;


};

#endif // !HITSTOP_H