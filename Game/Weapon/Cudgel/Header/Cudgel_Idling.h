#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Cudgel;
class Enemy;

/// <summary>
/// �ҋ@���
/// </summary>
class Cudgel_Idling : public IWeapon
{
public:

	// �R���X�g���N�^
	Cudgel_Idling(Cudgel* cudgel);

	// �f�X�g���N�^
	~Cudgel_Idling()override;
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
	// ���_�̌�
	Cudgel* m_cudgel;
};