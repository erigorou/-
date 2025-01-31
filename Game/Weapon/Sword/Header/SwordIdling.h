#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Sword;

/// <summary>
/// �����Y�������̑ҋ@���
/// </summary>
class SwordIdling : public IWeapon
{
public:
	// �Œ�l
	static const float ROTX;
	static const float ROTY;

	// �R���X�g���N�^
	SwordIdling(Sword* sword);
	// �f�X�g���N�^
	~SwordIdling();

	// ����������
	void Initialize()override;
	// ���O����
	void PreUpdate()override;
	// �X�V����
	void Update(float elapsedTime)override;
	// ���㏈��
	void PostUpdate()override;
	// �I������
	void Finalize()override;

	void HitAction(InterSectData data)override;


private:
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;

private:
	// �\�[�h�̌����擾
	Sword* m_sword;
};