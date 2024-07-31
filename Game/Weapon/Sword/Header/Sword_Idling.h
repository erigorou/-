#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"
#include "Interface/IWeapon.h"

class Sword;

/// <summary>
/// �����Y�������̑ҋ@���
/// </summary>
class Sword_Idling : public IWeapon
{
public:
	// �Œ�l
	static const float ROTX;
	static const float ROTY;


	// ����̓����蔻����擾����֐�
	DirectX::BoundingOrientedBox GetBoundingBox(){ return m_boundingBox; }

	// �R���X�g���N�^
	Sword_Idling(Sword* sword);
	// �f�X�g���N�^
	~Sword_Idling();

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
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	DirectX::Model* m_model;

	// ���̓����蔻��
	DirectX::BoundingOrientedBox m_boundingBox;
	// �I���W�i���̓����蔻�� (�I���W�i���͐��������邾���̂���)
	DirectX::BoundingOrientedBox m_originalBox;

private:
	// �\�[�h�̌����擾
	Sword* m_sword;
};