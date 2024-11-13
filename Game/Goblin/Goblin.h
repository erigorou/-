// -------------------------------------------------------
// ���S�̐e�N���X
// -------------------------------------------------------


#pragma once

#ifndef GOBLIN_OBJECT
#define GOBLIN_OBJECT

#include "pch.h"
#include "Interface/IObject.h"
#include "Interface/IState.h"

class Player;
class PlayScene;
class HPSystem;

class Goblin : public IObject
{
public:
	class GoblinIdling;
	class GoblinAttacking;

	// �Œ�l
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	static constexpr float GOBLIN_HP = 3.0f;

	// ���n�֐�
	DirectX::SimpleMath::Vector3	GetPosition()	override{ return m_position;	}	// �S�̍��W���擾����
	DirectX::SimpleMath::Vector3	GetVelocity()	const	{ return m_velocity;	}	// ���x�̎擾
	DirectX::SimpleMath::Vector3	GetAngle()		const	{ return m_angle;		}	// ��]�p�̎擾
	DirectX::SimpleMath::Matrix		GetWorldMatrix()const	{ return m_worldMatrix; }	// ���[���h���W�̎擾

	// �X�e�[�g�p�^�[��
	GoblinIdling*		GetIdling()		const { return m_idling.get		(); }	// �ҋ@�X�e�[�g�̎擾
	GoblinAttacking*	GetAttacking()	const { return m_attacking.get	();	}	// �U���X�e�[�g�̎擾


	PlayScene* GetPlayScen() const { return m_playScene; }	// PlayScene�̎擾

	Goblin(PlayScene* playScene);
	~Goblin();

	void Initialize();									// �������֐�
	void CreateCollision();								// �����蔻��̐���

	void Update(const float elapsedTime);				// �X�V����
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);	// �`��֐�
	
	void Finalize();									// �I���֐�
	void HitAction(InterSectData data) override;		// ���������Ƃ��̏���

	void ChangeState(IState* state);	// �X�e�[�g�̕ύX
private:

	void CreateState();		// �X�e�[�g�̍쐬
	
	DirectX::SimpleMath::Vector3 m_position;	// ���W
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	DirectX::SimpleMath::Vector3 m_angle;		// ��]�p
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h���W

	std::unique_ptr<DirectX::Model> m_model;	// ���f��


	// ���݂̃X�e�[�g
	IState* m_currentState;										// ���݂̃X�e�[�g
	std::unique_ptr<Goblin::GoblinIdling	> m_idling		;	// �ҋ@
	std::unique_ptr<Goblin::GoblinAttacking	> m_attacking	;	// �U��
 
	// HP
	std::unique_ptr<HPSystem> m_hp;	// HP

	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;										// �G�t�F�N�g
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;	// �v���~�e�B�u�o�b�`

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;	// ���̓��C�A�E�g
	
	// �v���C�V�[��(�����蔻��̏����Ɏg�p)
	PlayScene* m_playScene;

	// �̂̓����蔻��
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
};


#endif // !GOBLIN_OBJECT