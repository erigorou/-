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
	// �Œ�l
	static const float CHARGE_TIME;	// �U���̒��߃��[�V�����̎���
	static const float WINDUP_TIME;	// �U������O�̎���
	static const float ATTACK_TIME;	// �U�����鎞��
	static const float END_TIME;	// �U���i�S�́j���I�����鎞��

	static const DirectX::SimpleMath::Vector3 ARM_LENGTH;		// �ˋ�̘r�̒���
	static const DirectX::SimpleMath::Vector3 ZERO_DIREC;		// ���_����ړ�����n�_


	/// <summary>
	/// ���_�̓����蔻����擾����
	/// </summary>
	/// <returns>���̓����蔻��</returns>
	DirectX::BoundingOrientedBox GetBoundingBox()override { return m_boundingBox; }

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
	
	// Cudgel�̉�]���v�Z����֐�
	void UpdateCudgelRotation();
	// �����l�Ƃ��Ďg�p���郏�[���h�s����v�Z����֐�
	void CalculateModelMatrix();
	// Cudgel�̍U�����[�V�����̉�]���v�Z����֐�
	DirectX::SimpleMath::Matrix CalculateAttackMatrix();
	// ���{�ƒ��_�̍��W���擾���� �� both ends = ���[
	void GetCudgelBothEnds(float _totalTime);


private:
	DirectX::SimpleMath::Vector3 m_position;	// ���W
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	float m_angleRL;							// ���E�p�x
	float m_angleUD;							// �㉺�p�x

	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h�s��
	DirectX::Model* m_model;					// ���f���̃|�C���^
	float m_totalSeconds;						// �X�e�[�g�̌o�ߎ���
	float m_recordPointTimer;					// ���W���L�^����C���^�[�o���̌v���p�ϐ� 

	DirectX::BoundingOrientedBox m_boundingBox;	// ���_�̓����蔻��
	DirectX::BoundingOrientedBox m_originalBox;	// ���_�̑匳�ƂȂ铖���蔻��i�����l�����L�^����Ă���j

	Cudgel* m_cudgel;							// �X�e�[�g�����L����e

	// ���̋O�Ղ̃G�t�F�N�g
	// ���_��ۑ�����p�̑o�ɔz��
	std::list<DirectX::SimpleMath::Vector3> m_rootPos;	// ���{
	std::list<DirectX::SimpleMath::Vector3> m_tipPos;	// ��[




	// ���{�̈ʒu�Ɛ�[�̈ʒu�������ƕۑ��ł��Ă���̂������
	// ���̖����������邽�߂�BoundingSphere�𐶐����āA��̈ʒu�ɒu���B
	DirectX::SimpleMath::Vector3 m_rootDeb;
	DirectX::SimpleMath::Vector3 m_tipDeb;

	std::vector<DirectX::GeometricPrimitive> m_sphere;
};