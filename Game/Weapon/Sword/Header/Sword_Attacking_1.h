#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"

class Sword;

/// <summary>
/// �����Y�������̍U��
/// </summary>
class Sword_Attacking_1 : public IWeapon
{
public:
	// �Œ�l
	static const float RADIAN_90;
	static const float ATTACK_TIME;


	// ���E�{�b�N�X�̓����蔻���n��
	DirectX::BoundingBox GetBoundingBox(){ return m_boundingBox; }


	// �R���X�g���N�^
	Sword_Attacking_1(Sword* sword);
	// �f�X�g���N�^
	~Sword_Attacking_1();

	// ����������
	void Initialize()override;
	// ���O����
	void PreUpdate()override;
	// �X�V����
	void Update(float elapsedTime)override;
	// ���㏈��
	void PostUpdate()override;
	// �`�揈��
	void Render(ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;
	// �I������
	void Finalize()override;

private:
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;
	// ���̉�](x��, y��, z��)
	DirectX::SimpleMath::Vector3 m_rot;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	DirectX::Model* m_model;

	// ���̃V�[���ɂȂ��Ă���̃X�e�[�g
	float m_totalSeconds;

	// ���̓����蔻��1(���ۂ̓����蔻��)�@
	DirectX::BoundingBox m_boundingBox;
	// �I���W�i���̓����蔻�� (�I���W�i���͐��������邾���̂���)
	DirectX::BoundingBox m_originalBox;


private:
	// �\�[�h�̌����擾
	Sword* m_sword;
};