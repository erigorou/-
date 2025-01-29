// -------------------------------------------------------------------
//
// �������̂̌��ɕt���p�[�e�B�N���̓���
//
// -------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IParticle.h"

class DustTrailParticle : public IParticle
{
// ----------------------------
// �A�N�Z�T
// ----------------------------
public:
	DirectX::SimpleMath::Vector3 GetPosition () { return m_position; } // ���W
	DirectX::SimpleMath::Vector3 GetVelocity () { return m_velocity; } // ���x
	DirectX::SimpleMath::Vector3 GetAccelerator () { return m_accelerator; } // �����x
	DirectX::SimpleMath::Vector3 GetNowScale () { return m_nowScale; } // ���݂̃T�C�Y
	DirectX::SimpleMath::Vector3 GetStartScale () { return m_startScale; } // �J�n���T�C�Y
	DirectX::SimpleMath::Vector3 GetEndScale () { return m_endScale; } // �I�����T�C�Y
	float GetLife () { return m_life; } // �\������
	float GetStartLife () { return m_startLife; } // �J�n����
	DirectX::SimpleMath::Color GetNowColor () { return m_nowColor; } // ���݂̐F
	DirectX::SimpleMath::Color GetStartColor () { return m_startColor; } // �J�n���F
	DirectX::SimpleMath::Color GetEndColor () { return m_endColor; } // �I�����F

// ----------------------------
// �����o�֐��i���J�j
// ----------------------------
public:

	// �R���X�g���N�^
	DustTrailParticle(
		float life,
		DirectX::SimpleMath::Vector3 pos,													// �������W
		DirectX::SimpleMath::Vector3 velocity,												// �������x
		DirectX::SimpleMath::Vector3 accelerator,											// �����x
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,		// �����T�C�Y�ƍŏI�T�C�Y
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor			// �����J���[�ƍŏI�J���[
	);
	// �f�X�g���N�^
	~DustTrailParticle();
	// �X�V����
	bool Update(float elapsedTime);

// ----------------------------
// �����o�ϐ�
// ----------------------------
private:
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
};