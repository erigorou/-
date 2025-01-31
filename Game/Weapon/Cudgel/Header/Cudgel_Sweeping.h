#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Cudgel;
class Boss;
class Easying;


/// <summary>
/// �U�����
/// </summary>
class Cudgel_Sweeping : public IWeapon
{
public:
	// �Œ�l

	// �U���̒��߃��[�V�����̎���
	static constexpr float CHARGE_TIME = 0.8f;
	// �U������O�̎���
	static constexpr float WINDUP_TIME = 0.9f;
	// �U�����鎞��
	static constexpr float ATTACK_TIME = 1.5f;
	// �U���i�S�́j���I�����鎞��
	static constexpr float END_TIME = 2.3f;

	// ������]�p
	static constexpr float INITIAL_ROTATE_UD = 93.0f;
	// �U���̒��߃��[�V�����̊p�x
	static constexpr float CHARGE_ROTATE_ANGLE = 30.0f;
	// �U����̊p�x
	static constexpr float WINDUP_ROTATE_ANGLE = 240.0f;

	// �r�̒���
	static constexpr DirectX::SimpleMath::Vector3 ARM_LENGTH = {0.0f, 5.0f, 0.0f};
	// ���_����ړ�����ʒu
	static constexpr DirectX::SimpleMath::Vector3 ZERO_DIREC = {6.0f, 1.0f, 0.0f};


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
	// �`���[�W�A�j���[�V����
	void ChargeAnimation();
	// �U�����[�V����
	void AttackAnimation();
	// �I�����[�V����
	void EndAnimation();

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

	Cudgel* m_cudgel; // �X�e�[�g�����L����e
	Boss* m_boss;	// �{�X

	// ���̋O�Ղ̃G�t�F�N�g
	// ���_��ۑ�����p�̉ϒ��z��
	std::vector<DirectX::SimpleMath::Vector3> m_rootPos;	// ���{
	std::vector<DirectX::SimpleMath::Vector3> m_tipPos;		// ��[
	// ���ʉ����Đ��������̃t���O
	bool m_playSound;

	// �v���C���[���q�b�g�ł��邩�̃t���O
	bool m_playerCanHit;
};