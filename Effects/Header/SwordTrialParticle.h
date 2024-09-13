// -------------------------------------------------------------------
//
// �������̂̌��ɕt���p�[�e�B�N���̓���
//
// -------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IParticle.h"


//SimpleMath::Vector2(0.0f, 0.0f)), // ����
//SimpleMath::Vector2(1.0f, 0.0f)), // �E��
//SimpleMath::Vector2(1.0f, 1.0f)), // �E��
//SimpleMath::Vector2(0.0f, 1.0f)), // ����


class SwordTrialParticle : public IParticle
{
public:
	// �Q�b�^�[************************************
	const float GetLife									() { return m_life; }			// �\������
	const float GetStartLife							() { return m_startLife; }		// �J�n����
	const DirectX::SimpleMath::Color GetNowColor		() { return m_nowColor; }		// ���݂̐F
	const DirectX::SimpleMath::Color GetStartColor		() { return m_startColor; }		// �J�n���F
	const DirectX::SimpleMath::Color GetEndColor		() { return m_endColor; }		// �I�����F
	// ********************************************

private:
	// �ϐ��錾
	// *********************************

	// �`��ɕK�v�ȂS���_�̍��W
	DirectX::VertexPositionTexture m_vertices[4];

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
	SwordTrialParticle(
		DirectX::VertexPositionTexture vertex[4],											// ���_���
		float life,																			// ��������
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor			// �����J���[�ƍŏI�J���[
	);
	// �f�X�g���N�^
	~SwordTrialParticle();
	// �X�V����
	bool Update(float elapsedTime);


	// �`��ɂ��Ăǂ��ōs���ׂ��Ȃ̂��B
};