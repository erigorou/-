#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Sword;

/// <summary>
/// �����Y�������̑ҋ@���
/// </summary>
class SwordIdling : public IWeapon
{
// -----------------------
// �Œ�l
// -----------------------
public:



// -----------------------
// �����o�֐�(���J)
// -----------------------
public:
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
	// �Փˏ���
	void HitAction(InterSectData data)override;


// -----------------------
// �����o�ϐ�
// -----------------------
private:
	// �\�[�h�̌����擾
	Sword* m_sword;
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
};