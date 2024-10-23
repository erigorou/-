// �S�������_

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"
#include "Interface/IObject.h"

// ���_�̏�� ========================================================
#include "header/Cudgel_Idling.h"		// �ҋ@
#include "header/Cudgel_Attacking.h"	// �U��
#include "header/Cudgel_Sweeping.h"		// �ガ����

class Cudgel : public IObject
{
public:
	// �Œ�l
	static const float CUDGEL_SCALE;							// �傫��
	static const DirectX::SimpleMath::Vector3 DIRECTION_ENEMY;	// �G�Ƃ̗���Ă��鋗��
	static const DirectX::SimpleMath::Vector3 CUDGEL_LENGTH;	// ���_�̒���
	static const DirectX::SimpleMath::Vector3 CUDGEL_HADLE_POS;	// ���_�̎����̈ʒu


	// ===�擾�n====================================================================
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }	// �ʒu�̎擾


	DirectX::Model* GetModel()		const { return m_model.get();	}	// ���f���̎擾
	PlayScene*		GetPlayScene()	const { return m_playScene;		}	// �v���C�V�[���̎擾


	// ===��Ԃ̎擾================================================================
	Cudgel_Idling*		GetIdling()		const { return m_idling.get();		}	// �ҋ@
	Cudgel_Attacking*	GetAttacking()	const { return m_attacking.get();	}	// �U��
	Cudgel_Sweeping*	GetSweeping()	const { return m_sweeping.get();	}	// �ガ����	

	// ===�ݒ�n====================================================================
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }	// �����蔻��̈ʒu�̐ݒ�

public:
	// �R���X�g���N�^
	Cudgel(PlayScene* playScene);
	// �f�X�g���N�^
	~Cudgel();

	// ������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// ���E�{�b�N�X�̕`��
	void DrawBoundingBox(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// �I������
	void Finalize();
	// ���������Ƃ��̏���
	void HitAction(InterSectData) override;
	// �X�e�[�g���X�V����
	void ChangeState(IWeapon* state);

private:

	// ���f���̐���
	void CreateModel(ID3D11Device1* device);
	// �X�e�[�g�𐶐�
	void CreateState();
	// �����蔻��̐���
	void CreateCollision();



	DirectX::SimpleMath::Vector3 m_position;	// �ʒu
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	DirectX::SimpleMath::Vector3 m_angle;		// �p�x
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h�s��
	std::unique_ptr<DirectX::Model> m_model;	// ���f��

	// ���_�̓����蔻��1(���ۂ̓����蔻��)�@
	std::unique_ptr<DirectX::BoundingOrientedBox>	m_collision;
	// �I���W�i���̓����蔻�� (�I���W�i���͐��������邾���̂���)
	DirectX::BoundingOrientedBox m_originalBox;

private:
	// ���݂̃X�e�[�g
	IWeapon* m_currentState;

	std::unique_ptr<Cudgel_Idling>		m_idling;		// �ҋ@
	std::unique_ptr<Cudgel_Attacking>	m_attacking;	// ��������
	std::unique_ptr<Cudgel_Sweeping>	m_sweeping;		// �ガ����

private:
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���C�V�[���i�����蔻��̏����Ɏg�p�j
	PlayScene* m_playScene;
};