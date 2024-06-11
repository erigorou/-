#pragma once	// ���d�ǂݍ��ݖh�~

#include "Interface/IState.h"
// �ȉ��X�e�[�g
#include "Game/Enemy/States/EnemyIdling.h"

// �r�w�C�r�A�c���[
#include "BehaviourTree/Header/BehaviorTree.h"


class Enemy
{
	// �Œ�l
	static const float ENEMY_SPEED;

public:

public:
	// �S�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// �S�̉�]�p���擾����
	float GetAngle() const { return m_angle; }
	// �G�̃A�C�h�����O���擾����
	EnemyIdling* GetEnemyIdling() const { return m_enemyIdling.get(); }


public:
	// �R���X�g���N�^
	Enemy();
	// �f�X�g���N�^
	~Enemy();
	// ����������
	void Initialize(ID3D11Device* device, const ID3D11DeviceContext* context, const DirectX::CommonStates* states );
	// �V������ԂɑJ�ڂ���i�X�e�[�g�p�^�[���j
	void ChangeState(IState* newState);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources);
	// �I������
	void Finalize();


private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;
	// �G�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// ���݂̃X�e�[�g�i�X�e�[�g�p�^�[���j
	IState* m_currentState;

	// �r�w�C�r�A�c���[
	std::unique_ptr<BehaviorTree> m_pBT;

private:
	std::unique_ptr<EnemyIdling> m_enemyIdling;
};
