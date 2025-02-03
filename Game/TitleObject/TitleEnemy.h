#pragma once	// ���d�ǂݍ��ݖh�~
#include "pch.h"
#include "Interface/IState.h"
#include "Game/Scene/TitleScene.h"

// ===== �G�̏�� =================================================================
class EnemyTitleMoving;
class EnemyTitleIdling;

class TitleEnemy
{
public:
	// �Œ�l
	static const float TITLE_ENEMY_SPEED;
	static const float TITLE_ENEMY_SCALE;

	static const float COOL_TIME;

public:

	// /////////////////�G�̊�b����n���֐�/////////////////////////////////////////////////////////////////////
	DirectX::SimpleMath::Vector3	GetPosition()	const { return m_position; }		// �S�̍��W���擾����
	float							GetAngle()		const { return m_angle; }			// �S�̉�]�p���擾����
	DirectX::SimpleMath::Matrix		GetWorldMatrix()const { return m_worldMatrix; }	// �G�̃��[���h���W���擾����

	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }		// �S�̍��W��ݒ肷��
	void SetAngle(const float angle) { m_angle = angle; }		// �S�̉�]�p��ݒ肷��
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }	// �G�̃��[���h���W��ݒ肷��

	// /////////////////�G�̃X�e�[�g��n���֐�/////////////////////////////////////////////////////////////////////
	EnemyTitleMoving* GetTitleEnemyTitleMoving() const { return m_titleMoving.get(); }	// �^�C�g����ʂ̓G�̈ړ����
	EnemyTitleIdling* GetTitleEnemyIdling() const { return m_titleIdling.get(); }	// �^�C�g����ʂ̓G�̑ҋ@���

	// ���݂̃X�e�[�g��Ԃ�
	IState* GetCurrentState() const { return m_currentState; }

public:
	// �R���X�g���N�^
	TitleEnemy();
	// �f�X�g���N�^
	~TitleEnemy();
	// ����������
	void Initialize();
	// �V������ԂɑJ�ڂ���i�X�e�[�g�p�^�[���j
	void ChangeState(IState* newState);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// �I������
	void Finalize();

private:
	// �X�e�[�g�̍쐬����
	void CreateState();

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

	// ==== �X�e�[�g�p�^�[���Ɏg�p =============================================�@
	IState* m_currentState;			// ���݂̃X�e�[�g�i�X�e�[�g�p�^�[���j
	std::unique_ptr<EnemyTitleMoving> m_titleMoving;		// �^�C�g����ʂ̓G�̈ړ����
	std::unique_ptr<EnemyTitleIdling> m_titleIdling;		// �^�C�g����ʂ̓G�̑ҋ@���

private:
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
