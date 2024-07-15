#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Cudgel;
class Enemy;

/// <summary>
/// �U�����
/// </summary>
class Cudgel_Attacking : public IWeapon
{
public:

	/// <summary>
	/// ���_�̓����蔻����擾����
	/// </summary>
	/// <returns>���̓����蔻��</returns>
	DirectX::BoundingBox GetBoundingBox() { return m_boundingBox; }

	// �R���X�g���N�^
	Cudgel_Attacking(Cudgel* cudgel);

	// �f�X�g���N�^
	~Cudgel_Attacking()override;
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
	float m_angleRL;
	// �㉺�̊p�x
	float m_angleUD;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	DirectX::Model* m_model;
	// ���v����
	float m_totalSeconds;
	// ���_�̓����蔻��1(���ۂ̓����蔻��)�@
	DirectX::BoundingBox m_boundingBox;

	// �I���W�i���̓����蔻�� (�I���W�i���͐��������邾���̂���)
	DirectX::BoundingBox m_originalBox;

private:
	// ���_�̌�
	Cudgel* m_cudgel;
};