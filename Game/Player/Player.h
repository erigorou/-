#pragma once
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

// �v���C���[�ɕt�^�������� ===========================================
#include "Game/Weapon/Sword/Sword.h"	// ����

// �v���C���[�̏�� =====================================================
#include "Game/Player/State/Header/Player_Idling.h"			// �ҋ@���
#include "Game/Player/State/Header/Player_Dodging.h"		// ������
#include "Game/Player/State/Header/Player_Attacking_1.h"	// �U����ԂP
#include "Game/Player/State/Header/Player_Attacking_2.h"	// �U����ԂQ
#include "Game/Player/State/Header/Player_Attacking_3.h"	// �U����ԂR
#include "Game/Player/State/Header/Player_Attacking_4.h"	// �U����ԂS


class Player
{
public:
	// �������W
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	// ����
	static const float PLAYER_SPEED;
	// �傫��
	static const float PLAYER_SCALE;
	// �U�����K�p����鎞��
	static const float APPLIED_ATTACK_TIME;

public:
	// �v���C���[�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// �v���C���[�̑��x���擾����
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// �v���C���[�̉�]�p���擾����
	float GetAngle() const { return m_angle; }


	// �v���C���[�̃A�C�h�����O�X�e�[�g���擾
	PlayerIdling* GetPlayerIdlingState() const { return m_playerIdling.get(); }
	// �v���C���[�̃_�b�W���O�X�e�[�g���擾
	PlayerDodging* GetPlayerDodgingState() const { return m_playerDodging.get(); }

	// �v���C���[�̃A�^�b�L���O�X�e�[�g���擾
	PlayerAttacking_1* GetPlayerAttackingState1() const { return m_playerAttacking_1.get(); }
	PlayerAttacking_2* GetPlayerAttackingState2() const { return m_playerAttacking_2.get(); }
	PlayerAttacking_3* GetPlayerAttackingState3() const { return m_playerAttacking_3.get(); }
	PlayerAttacking_4* GetPlayerAttackingState4() const { return m_playerAttacking_4.get(); }

	// �v���C�V�[���̏����擾����
	PlayScene* GetPlayScene()const { return m_playScene; }
	// �L�[�{�[�h�̏����擾����(�������̂ݑΉ�)
	DirectX::Keyboard::State GetKeyboardState() const { return m_keyboardState; }
	// �L�[�{�[�h�̏����擾����i�P�����̂ݑΉ��j
	DirectX::Keyboard::KeyboardStateTracker GetKeyboardTracker() const { return m_tracker; }

public:
	// �R���X�g���N�^
	Player(PlayScene* playScene);
	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize();
	// �X�e�[�g�̍쐬�֐�
	void CreateState();
	// �V������ԂɑJ�ڂ���
	void ChangeState(IState* newState);
	// ���Ԍv�����s��
	void TimeComparison(float& nowTime, const float totalTime, IState* newState, const float elapsedTime);
	// �X�V����
	void Update(
		const DirectX::SimpleMath::Vector3 enemyPos,
		const float elapsedTime
	);
	// �`�揈��
	void Render(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources
	);
	// ���E���̕`��
	void DrawBoundingSphere(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::BoundingSphere boundingSphere);

	// �I������
	void Finalize();
	// ��]�p�̌v�Z
	void CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos);
	// Matrix�̌v�Z
	void CalculationMatrix();
	// �ړ��̊Ǘ�
	void MovePlayer();

private:
	// �̗�
	int m_HP;
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �����x
	DirectX::SimpleMath::Vector3 m_acceleration;
	// �p�x
	float m_angle;


	// �v���C���[�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �X�e�[�g�֘A =================================================
	IState* m_currentState;										// ���݂̃X�e�[�g
	std::unique_ptr<PlayerIdling> m_playerIdling;				// �ҋ@���
	std::unique_ptr<PlayerDodging> m_playerDodging;				// ������

	std::unique_ptr<PlayerAttacking_1> m_playerAttacking_1;		// �U����ԂP
	std::unique_ptr<PlayerAttacking_2> m_playerAttacking_2;		// �U����ԂQ
	std::unique_ptr<PlayerAttacking_3> m_playerAttacking_3;		// �U����ԂR
	std::unique_ptr<PlayerAttacking_4> m_playerAttacking_4;		// �U����ԂS


	// �v���C���[�ɕt�^�������� ============
	std::unique_ptr<Sword> m_sword;


	// �V�F�[�_�[�Ɏg�p������� ==============
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	float m_particleTime;
	float m_elapsedTime;

private:
	// �v���C�V�[��(���̃I�u�W�F�N�g�̏��̎擾�ȂǂɎg��)
	PlayScene* m_playScene;
	// �L�[�{�[�h�̓���
	DirectX::Keyboard::State m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_tracker;
};
