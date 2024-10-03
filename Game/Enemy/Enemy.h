#pragma once	// ���d�ǂݍ��ݖh�~
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

#include "Game/Enemy/EnemyHP.h"	// HP

// ===== �G�̏�� =================================================================
#include "States/Header/EnemyIdling.h"		// �ҋ@���
#include "States/Header/Enemy_Attacking.h"	// ���������U��
#include "States/Header/Enemy_Sweeping.h"	// �ガ�����U��
#include "States/Header/EnemyApproaching.h"	// �ǔ����


#include "BehaviourTree/Header/BehaviorTree.h"			// �r�w�C�r�A�c���[


class Enemy
{
public:
	// �Œ�l
	static const float ENEMY_SPEED;
	static const float ENEMY_SCALE;


public:
	PlayScene* GetPlayScene()					const { return m_playScene;}		// PlayScene�̎擾 
	EnemyHP* GetEnemyHP()						const { return m_hp.get(); }		// HP�̎擾	
	
	DirectX::SimpleMath::Vector3 GetPosition()	const { return m_position; }		// �S�̍��W���擾����
	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }	// �S�̍��W��ݒ肷��
	
	float GetAngle() const { return m_angle; }										// �S�̉�]�p���擾����	
	void SetAngle(const float angle) { m_angle = angle; }							// �S�̉�]�p��ݒ肷��	

	DirectX::SimpleMath::Matrix	GetWorldMatrix() const { return m_worldMatrix; }	// �G�̃��[���h���W���擾����
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }	// �G�̃��[���h���W��ݒ肷��

	// ===== �G�̏�� =================================================================
	EnemyIdling* GetEnemyIdling() const { return m_idling.get(); }					// �ҋ@���
	Enemy_Attacking* GetEnemyAttacking() const { return m_attacking.get(); }		// �U�����
	Enemy_Sweeping* GetEnemySweeping() const { return m_sweeping.get(); }			// �ガ�������
	EnemyApproaching* GetEnemyApproaching() const { return m_approaching.get(); }	// �ǔ����


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
	// ���E���̕`�揈��
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


	// HP
	std::unique_ptr<EnemyHP> m_hp;

	// ==== �X�e�[�g�p�^�[���Ɏg�p =============================================�@
	IState* m_currentState;			// ���݂̃X�e�[�g�i�X�e�[�g�p�^�[���j
	std::unique_ptr<EnemyIdling> m_idling;					// �ҋ@���
	std::unique_ptr<Enemy_Attacking> m_attacking;			// �U�����
	std::unique_ptr<Enemy_Sweeping> m_sweeping;				// �ガ�������
	std::unique_ptr<EnemyApproaching> m_approaching;		// �ǔ����

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
