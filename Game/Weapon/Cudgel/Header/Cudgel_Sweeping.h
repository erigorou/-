#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Cudgel;
class Enemy;
class Particle;
class Easying;


/// <summary>
/// �U�����
/// </summary>
class Cudgel_Sweeping : public IWeapon
{
public:
	// �Œ�l
	static const float CHARGE_TIME;	// �U���̒��߃��[�V�����̎���
	static const float WINDUP_TIME;	// �U������O�̎���
	static const float ATTACK_TIME;	// �U�����鎞��
	static const float END_TIME;	// �U���i�S�́j���I�����鎞��

	static const float CHARGE_ROTATE_ANGLE;	// �U���̒��߃��[�V�����̊p�x
	static const float WINDUP_ROTATE_ANGLE;	// �U����̊p�x

	static const DirectX::SimpleMath::Vector3 ARM_LENGTH;		// �ˋ�̘r�̒���
	static const DirectX::SimpleMath::Vector3 ZERO_DIREC;		// ���_����ړ�����n�_


	// �R���X�g���N�^
	Cudgel_Sweeping(Cudgel* cudgel);

	// �f�X�g���N�^
	~Cudgel_Sweeping()override;
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
	// Cudgel�̉�]���v�Z����֐�
	void UpdateCudgelRotation();
	// �����l�Ƃ��Ďg�p���郏�[���h�s����v�Z����֐�
	void CalculateModelMatrix();
	// Cudgel�̍U�����[�V�����̉�]���v�Z����֐�
	DirectX::SimpleMath::Matrix CalculateAttackMatrix();
	// ���{�ƒ��_�̍��W���擾���� �� both ends = ���[
	void GetCudgelBothEnds();

private:
	DirectX::SimpleMath::Vector3 m_position;	// ���W
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	float m_angleRL;							// ���E�p�x
	float m_angleUD;							// �㉺�p�x
	float m_parentAngleRL;						// �e�̍��E�p�x

	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_collMatrix;	// �����蔻��p�̍s��
	float m_totalSeconds;						// �X�e�[�g�̌o�ߎ���

	Cudgel*		m_cudgel;						// �X�e�[�g�����L����e
	Particle*	m_particles;					// �p�[�e�B�N��

	// ���̋O�Ղ̃G�t�F�N�g
	// ���_��ۑ�����p�̉ϒ��z��
	std::vector<DirectX::SimpleMath::Vector3> m_rootPos;	// ���{
	std::vector<DirectX::SimpleMath::Vector3> m_tipPos;		// ��[

	// �p�[�e�B�N���𐶐��������̃t���O
	bool m_canGenerateSlamParticles;

	// ���ʉ����Đ��������̃t���O
	bool m_playSound;
};