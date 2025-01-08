// �X�e�[�W�̋��E��
#pragma once

#include "Interface/IObject.h"

class PlayScene;

class Wall : public IObject
{
public: 
	// ------------------------
	// �Œ�l
	// ------------------------
	static const float WALL_SCALE;						// �傫���i�Œ�l�j
	
	static constexpr float COLLISION_RADIUS = 160.0f;	// �����蔻��̔��a
	static constexpr float COLLISION_SCALE = 10.0f;		// �����蔻��̑傫��

	static constexpr DirectX::SimpleMath::Vector3 WALL_POS = { 0.0f, -60.0f, 0.0f };	// ���f�����ړ�������ʒu


	// ------------------------
	// ���J�֐�
	// ------------------------
	// �ǂ̏Փ˔�����擾
	DirectX::BoundingSphere* GetCollision() { return m_collision.get();	}
	// �ǂ̍��W�̎擾
	DirectX::SimpleMath::Vector3 GetPosition() override	{ return DirectX::SimpleMath::Vector3::Zero; }

	// �R���X�g���N�^
	Wall(PlayScene* playScene);
	// �f�X�g���N�^
	~Wall();
	// ������
	void Initialize();
	// �`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);
	// �I������
	void Finalize();

	// �Փˏ���
	void HitAction(InterSectData data)override;


private:

	// �Փ˔���̐���
	void CreateCollision();

	// ���[���h�s��X�V����
	void UpdateWorldMatrix();

	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h�s��
	std::unique_ptr<DirectX::Model> m_model;	// ���f��

	std::unique_ptr<DirectX::BoundingSphere> m_collision;	// �����蔻��
	std::unique_ptr<DirectX::BoundingSphere> m_overCollision; // �O�̓����蔻��Ɍ����Ă�����

	PlayScene* m_playScene;	// �v���C�V�[��

	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;										// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;	// �v���~�e�B�u�o�b�`
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;									// ���̓��C�A�E�g
};
