#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"

class Sword;
class Easying;
class Particle;

/// <summary>
/// �����Y�������̍U��
/// </summary>
class Sword_Attacking_2 : public IWeapon
{
// �Œ�l **
public:
	// 90�x�̃��W�A��
	static const float RADIAN_90;
	// �U���̎���
	static constexpr float ATTACK_TIME = 0.5f;
	// �ʒu�����p
	static constexpr DirectX::SimpleMath::Vector3 MATRIX_DIRECTION = DirectX::SimpleMath::Vector3(1.0f, 2.0f, 0.0f);

	// ���̒��ߊp�x
	static constexpr float MAX_SIDE_ANGLE = 250.0f;
	// �����p�x
	static constexpr float INIT_ANGLE = 10.0f;
	// �c�̍ő�
	static constexpr float MAX_VERTICAL_ANGLE = 30.0f;

// ���J�֐� **
public:
	// �R���X�g���N�^
	Sword_Attacking_2(Sword* sword);
	// �f�X�g���N�^
	~Sword_Attacking_2();

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
	// �Փˏ���
	void HitAction(InterSectData data)override;


// ---------------------
// ����J�֐�
// ---------------------
private:
	// �A�j���[�V�����X�V����
	void UpdateAnimation();
	// ���[���h�s��̍X�V
	void UpdateWorldMatrix();
	// ���{�ƒ��_�̍��W���擾���� �� both ends = ���[
	void GetCudgelBothEnds();

	// �\�[�h�̃p�[�e�B�N���𐶐�
	void CreateSwordParticle();


	// �\�[�h�̌����擾
	Sword* m_sword;

	DirectX::SimpleMath::Vector3	m_position;		// ���W
	DirectX::SimpleMath::Vector3	m_velocity;		// ���x
	float							m_angle;		// ���̉�]
	DirectX::SimpleMath::Vector3	m_rot;			// 3�����̌��̉�]
	DirectX::SimpleMath::Matrix		m_worldMatrix;	// ���[���h�s��
	DirectX::Model* m_model;		// ���f���̃|�C���^
	Particle* m_particles;							// �p�[�e�B�N��
	float m_totalSeconds;

	std::vector<DirectX::SimpleMath::Vector3>	m_rootPos;	// ���{�̍��W
	std::vector<DirectX::SimpleMath::Vector3>	m_tipPos;	// ��[�̍��W

};