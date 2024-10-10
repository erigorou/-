// -------------------------------------------------------
// ���S�̐e�N���X
// -------------------------------------------------------


#pragma once

#ifndef GOBLIN_OBJECT
#define GOBLIN_OBJECT

#include "pch.h"
#include "Interface/IObject.h"

class Player;
class PlayScene;

class Goblin : public IObject
{
public:
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	DirectX::SimpleMath::Vector3	GetPosition()	override{ return m_position;	}	// �S�̍��W���擾����
	DirectX::SimpleMath::Vector3	GetVelocity()	const	{ return m_velocity;	}	// ���x�̎擾
	DirectX::SimpleMath::Vector3	GetAngle()		const	{ return m_angle;		}	// ��]�p�̎擾
	DirectX::SimpleMath::Matrix		GetWorldMatrix()const	{ return m_worldMatrix; }	// ���[���h���W�̎擾

	// �X�e�[�g�p�^�[��


	PlayScene* GetPlayScen() const { return m_playScene; }	// PlayScene�̎擾

	Goblin(PlayScene* playScene);
	~Goblin();

	void Initialize();									// �������֐�
	void Update(const float elapsedTime);				// �X�V����
	
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);	// �`��֐�
	
	void Finalize();									// �I���֐�
	void HitAction(InterSectData data) override;		// ���������Ƃ��̏���

private:

	void CreateState();		// �X�e�[�g�̍쐬
	void CreateCollision();	// �����蔻��̐���
	
	DirectX::SimpleMath::Vector3 m_position;	// ���W
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	DirectX::SimpleMath::Vector3 m_angle;		// ��]�p
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h���W

	std::unique_ptr<DirectX::Model> m_model;	// ���f��


	// �X�e�[�g�p�^�[��



	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;	// �G�t�F�N�g
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���C�V�[��(�����蔻��̏����Ɏg�p)
	PlayScene* m_playScene;

	// �̂̓����蔻��
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
};


#endif // !GOBLIN_OBJECT