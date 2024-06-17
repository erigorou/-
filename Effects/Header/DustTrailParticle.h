// -------------------------------------------------------------------
//
// �������̂̌��ɕt���p�[�e�B�N���̓���
//
// -------------------------------------------------------------------

#pragma once
#include "pch.h"

class DustTrialParticle
{
public:
	// �Q�b�^�[************************************
	const DirectX::SimpleMath::Vector3 GetPosition		() { return m_position; }		// ���W
	const DirectX::SimpleMath::Vector3 GetVelocity		() { return m_velocity; }		// ���x
	const DirectX::SimpleMath::Vector3 GetAccelerator	() { return m_accelerator; }	// �����x
	const DirectX::SimpleMath::Vector3 GetNowScale	() { return m_nowScale; }		// ���݂̃T�C�Y
	const DirectX::SimpleMath::Vector3 GetStartScale() { return m_startScale; }		// �J�n���T�C�Y
	const DirectX::SimpleMath::Vector3 GetEndScale	() { return m_endScale; }		// �I�����T�C�Y
	const float GetLife		() { return m_life; }			// �\������
	const float GetStartLife() { return m_startLife; }		// �J�n����
	const DirectX::SimpleMath::Color GetNowColor	() { return m_nowColor; }		// ���݂̐F
	const DirectX::SimpleMath::Color GetStartColor	() { return m_startColor; }		// �J�n���F
	const DirectX::SimpleMath::Color GetEndColor	() { return m_endColor; }		// �I�����F
	// ********************************************

private:
	// �ϐ��錾
	// �y���̏�� *********************************
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �����x�x�N�g��
	DirectX::SimpleMath::Vector3 m_accelerator;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;
	//	��������
	float m_life;
	float m_startLife;
	//	�J���[
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;
	// *******************************************
public:

	// �֐��錾
	// �R���X�g���N�^
	DustTrialParticle(
		float life,
		DirectX::SimpleMath::Vector3 pos,													// �������W
		DirectX::SimpleMath::Vector3 velocity,												// �������x
		DirectX::SimpleMath::Vector3 accelerator,												// �����x
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,		// �����T�C�Y�ƍŏI�T�C�Y
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor			// �����J���[�ƍŏI�J���[
	);
	// �f�X�g���N�^
	~DustTrialParticle();
	// �X�V����
	bool Update(float elapsedTime);
};