// �v���C���[�i�����Y�j������

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"
#include "Interface/IObject.h"

// ���̏�� ========================================================
#include "Game/Weapon/Sword/Header/Sword_Idling.h"			// �ҋ@���
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"		// �U����ԂP



class Sword : public IObject
{
public:
	// �Œ�l
	static const float SWORD_SCALE;	// ���̑傫��
	static const DirectX::SimpleMath::Vector3 SWORD_DIR_FOR_PLAYER;	// ���ƃv���C���̗���Ă鋗��


	DirectX::Model* GetModel()		const { return m_model.get();	}	// ���f���̃Q�b�^�[
	PlayScene*	GetPlayScene()		const { return m_playScene;		}	// �v���C�V�[���̃Q�b�^�[

	// ��Ԃ̃Q�b�^�[
	IWeapon* GetIdlingState()		const { return m_swordIdling.get();		}	// �ҋ@���
	IWeapon* GetAttacking_1State()	const { return m_swordAttacking_1.get();}	// �U����ԂP

	// �ʒu�̃Q�b�^�[
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }

	// �����蔻��̈ʒu�̐ݒ�
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }
public:
	// �R���X�g���N�^
	Sword(PlayScene* playScene);
	// �f�X�g���N�^
	~Sword();

	// ������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources
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

	// �X�e�[�g���X�V����
	void ChangeState(IWeapon* state);

	// ���������Ƃ��̏���
	void HitAction(InterSectData data)  override;

private:
	// �X�e�[�g�𐶐�
	void CreateState();
	void CreateCollision();


	DirectX::SimpleMath::Vector3 m_position;	// �ʒu
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	DirectX::SimpleMath::Vector3 m_angle;		// �p�x
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h�s��
	std::unique_ptr<DirectX::Model> m_model;	// ���f��

	// ���̓����蔻��P�i���ۂ̓����蔻��j
	std::unique_ptr<DirectX::BoundingOrientedBox> m_collision;
	// �I���W�i���̓����蔻��i�I���W�i���͐��������邾���̂��́j
	DirectX::BoundingOrientedBox m_originalBox;

private:
	// ���݂̃X�e�[�g
	IWeapon* m_currentState;
	// �ҋ@���[�V����
	std::unique_ptr<Sword_Idling> m_swordIdling;
	std::unique_ptr<Sword_Attacking_1> m_swordAttacking_1;

	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���C�V�[���i�����蔻��̏����Ɏg�p�j
	PlayScene* m_playScene;

};