#pragma once
#include "Interface/IState.h"

#include "Game/Player/State/Header/PlayerIdling.h"		// �ҋ@���
#include "Game/Player/State/Header/PlayerDodging.h"		// ������
#include "Game/Player/State/Header/PlayerAttacking.h"	// �U�����

class Player
{
public:
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	static const float PLAYER_SPEED;

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
	PlayerAttacking* GetPlayerAttackingState() const { return m_playerAttacking.get(); }


public:
	// �R���X�g���N�^
	Player();
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
		const DirectX::SimpleMath::Matrix& projection);

	// �I������
	void Finalize();
	// ��]�p�̌v�Z
	void CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos);
	// Matrix�̌v�Z
	void CalculationMatrix();
	// �ړ��̊Ǘ�
	void MovePlayer();

private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;

	// �v���C���[�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// ���݂̃X�e�[�g
	IState* m_currentState;
	// �v���C���[�̃A�C�h�����O�X�e�[�g
	std::unique_ptr<PlayerIdling> m_playerIdling;
	// �v���C���[�̃_�b�W���O�X�e�[�g
	std::unique_ptr<PlayerDodging> m_playerDodging;
	// �v���C���[�̃A�^�b�L���O�X�e�[�g
	std::unique_ptr<PlayerAttacking> m_playerAttacking;

	// �o�E���f�B���O�X�t�B�A
	DirectX::BoundingSphere m_boundingSphereBody;

	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};