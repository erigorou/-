#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"

class Sword;
class Easying;

/// <summary>
/// �����Y�������̍U��
/// </summary>
class Sword_Attacking_1 : public IWeapon
{
public:
	// �Œ�l
	static const float RADIAN_90;
	static const float ATTACK_TIME;

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

	void HitAction(InterSectData data)override;

private:
	// ���{�ƒ��_�̍��W���擾���� �� both ends = ���[
	void GetCudgelBothEnds(float _totalTime);

	void CreateSwordParticle();


	// �\�[�h�̌����擾
	Sword* m_sword;

	DirectX::SimpleMath::Vector3	m_position;		// ���W
	DirectX::SimpleMath::Vector3	m_velocity;		// ���x
	float							m_angle;		// ���̉�]
	DirectX::SimpleMath::Vector3	m_rot;			// 3�����̌��̉�]
	DirectX::SimpleMath::Matrix		m_worldMatrix;	// ���[���h�s��
	DirectX::Model*					m_model;		// ���f���̃|�C���^

	float m_totalSeconds;

	std::vector<DirectX::SimpleMath::Vector3>	m_rootPos;	// ���{�̍��W
	std::vector<DirectX::SimpleMath::Vector3>	m_tipPos;	// ��[�̍��W

};