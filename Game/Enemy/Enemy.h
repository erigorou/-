#pragma once	// ���d�ǂݍ��ݖh�~
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

#include "Game/Enemy/States/Header/EnemyIdling.h"		// �ҋ@�X�e�[�g
#include "Game/Enemy/States/Header/EnemyApproaching.h"	// �ǔ��X�e�[�g
#include "BehaviourTree/Header/BehaviorTree.h"	// �r�w�C�r�A�c���[


class Enemy
{
public:
	// �Œ�l
	static const float ENEMY_SPEED;
	static const float ENEMY_SCALE;


public:
	// �v���C�V�[�����擾����
	PlayScene* GetPlayScene() const { return m_playScene; }
	// �S�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// �S�̍��W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// �S�̉�]�p���擾����
	float GetAngle() const { return m_angle; }
	// �S�̉�]�p��ݒ肷��
	void SetAngle(const float angle){ m_angle = angle; }
	// �G�̃��[���h���W���擾����
	DirectX::SimpleMath::Matrix	GetWorldMatrix() const { return m_worldMatrix; }
	// �G�̃��[���h���W��ݒ肷��
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }


	// �G�̃A�C�h�����O���擾����
	EnemyIdling* GetEnemyIdling() const { return m_enemyIdling.get(); }
	// �G�̒ǔ���Ԃ��擾����
	EnemyApproaching* GetEnemyApproaching() const { return m_enemyApproaching.get(); }
	// ���݂̃X�e�[�g��Ԃ�
	IState* GetCurrentState() const { return m_currentState; }


public:
	// �R���X�g���N�^
	Enemy(PlayScene* playScene);
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
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::BoundingSphere boundingSphere);
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
	// �ҋ@�X�e�[�g
	std::unique_ptr<EnemyIdling> m_enemyIdling;
	// �ǔ��X�e�[�g
	std::unique_ptr<EnemyApproaching> m_enemyApproaching;

	// �r�w�C�r�A�c���[
	std::unique_ptr<BehaviorTree> m_pBT;


private:
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���C�V�[��(�����蔻��̏����Ɏg�p)
	PlayScene* m_playScene;
};
