// �v���C���[�i�����Y�j������

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"
#include "Interface/IObject.h"

// ���̏�� ===============================================================
#include "Game/Weapon/Sword/Header/Sword_Idling.h"			// �ҋ@���
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"		// �U����ԂP
#include "Game/Weapon/Sword/Header/Sword_Attacking_2.h"		// �U����ԂQ
#include "Game/Weapon/Sword/Header/Sword_Attacking_3.h"		// �U����ԂR
#include "Game/Weapon/Sword/Header/Sword_Attacking_4.h"		// �U����ԂS



class Sword : public IObject
{
// �Œ�l**
public:
	static const float SWORD_SCALE;

	static constexpr DirectX::SimpleMath::Vector3 SWORD_DIR_FOR_PLAYER = { -1.5f, 1.0f, -2.0f };	// �v���C���[�̓��̈ʒu

	static constexpr float MODEL_TOP_HEIGHT  = 55.0f;
	static constexpr float MODEL_ROOT_HEIGHT = 50.0f;

// ���J�֐�**
	PlayScene*	GetPlayScene()		const { return m_playScene;		}	// �v���C�V�[���̃Q�b�^�[
	DirectX::BoundingOrientedBox GetCollision() const { return *m_collision.get(); }	// �����蔻��̎擾

	// ��Ԃ̃Q�b�^�[
	IWeapon* GetIdlingState()		const { return m_swordIdling.get();		}	// �ҋ@���
	IWeapon* GetAttacking_1State()	const { return m_swordAttacking_1.get();}	// �U����ԂP
	IWeapon* GetAttacking_2State()	const { return m_swordAttacking_2.get();}	// �U����ԂQ
	IWeapon* GetAttacking_3State()	const { return m_swordAttacking_3.get();}	// �U����ԂR
	IWeapon* GetAttacking_4State()	const { return m_swordAttacking_4.get();}	// �U����ԂS
	IWeapon* GetCurrentState()		const { return m_currentState;			}	// ���݂̃X�e�[�g�̎擾

	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }	// ���[���h�s��̐ݒ�

	void SetAttackFlag	(bool flag) { m_canAttack = flag; }	// �U���\���ǂ����̐ݒ�
	bool GetAttackFlag	()			{ return m_canAttack; }	// �U���\���ǂ���

	// �ʒu�̃Q�b�^�[
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }

	// �����蔻��̈ʒu�̐ݒ�
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }

	// �R���X�g���N�^
	Sword(PlayScene* playScene);
	// �f�X�g���N�^
	~Sword();
	// ������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
	// �I������
	void Finalize();

	// �X�e�[�g���X�V����
	void ChangeState(IWeapon* state);
	// ���������Ƃ��̏���
	void HitAction(InterSectData data)  override;

// �����֐�**
private:
	// �X�e�[�g�𐶐�
	void CreateState();
	void CreateCollision();


	DirectX::SimpleMath::Vector3 m_position;	// �ʒu
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	DirectX::SimpleMath::Vector3 m_angle;		// �p�x
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h�s��
	DirectX::Model*				m_model;		// ���f��

	// ���̓����蔻��P�i���ۂ̓����蔻��j
	std::unique_ptr<DirectX::BoundingOrientedBox> m_collision;
	// �I���W�i���̓����蔻��i�I���W�i���͐��������邾���̂��́j
	DirectX::BoundingOrientedBox m_originalBox;
	// �U���ł��邩�ǂ���
	bool m_canAttack;

	// ���݂̃X�e�[�g
	IWeapon* m_currentState;
	// �ҋ@���[�V����
	std::unique_ptr<Sword_Idling> m_swordIdling;
	std::unique_ptr<Sword_Attacking_1> m_swordAttacking_1;
	std::unique_ptr<Sword_Attacking_2> m_swordAttacking_2;
	std::unique_ptr<Sword_Attacking_3> m_swordAttacking_3;
	std::unique_ptr<Sword_Attacking_4> m_swordAttacking_4;


	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���C�V�[���i�����蔻��̏����Ɏg�p�j
	PlayScene* m_playScene;

};