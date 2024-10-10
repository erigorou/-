// �X�e�[�W�̋��E��
#pragma once

#include "Interface/IObject.h"

class Wall : public IObject
{
public: 
	// �Œ�l
	static const float WALL_SCALE;					// �傫���i�Œ�l�j
	static constexpr float COLLISION_RADIUS = 1.0f;	// �����蔻��̔��a

	// �R���X�g���N�^
	Wall();
	// �f�X�g���N�^
	~Wall();
	// ������
	void Initialize();
	// �`�揈��
	void Render(
		 ID3D11DeviceContext* context
		,DirectX::CommonStates* states
		,const DirectX::SimpleMath::Matrix& view
		,const DirectX::SimpleMath::Matrix& projection
	);
	// �I������
	void Finalize();

	// ���[���h�s��X�V����
	void UpdateWorldMatrix();


	// �C���^�[�t�F�[�X�Ŏg�p���邩�炵��[�Ȃ��ł����Ă���B
	void HitAction(InterSectData data)override;
	DirectX::SimpleMath::Vector3 GetPosition()override;


private:

	void CreateCollision();	// �����蔻��̐���


	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h�s��
	std::unique_ptr<DirectX::Model> m_model;	// ���f��

	std::unique_ptr<DirectX::BoundingSphere> m_collision;	// �����蔻��



	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;										// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;	// �v���~�e�B�u�o�b�`
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;									// ���̓��C�A�E�g
};
