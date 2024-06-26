#pragma once	// ���d�ǂݍ��ݖh�~
#include "Interface/IState.h"
#include "Game/Enemy/States/EnemyIdling.h"	// �ҋ@�X�e�[�g
#include "BehaviourTree/Header/BehaviorTree.h"	// �r�w�C�r�A�c���[


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
	void Initialize();
	// �X�e�[�g�̍쐬����
	void CreateState();
	// �V������ԂɑJ�ڂ���i�X�e�[�g�p�^�[���j
	void ChangeState(IState* newState);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
	void DrawBoundingSphere(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
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

	// �o�E���f�B���O�X�t�B�A
	DirectX::BoundingSphere m_boundingSphereBody;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
private:
	// �ҋ@�X�e�[�g
	std::unique_ptr<EnemyIdling> m_enemyIdling;
};
