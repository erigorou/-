// �S�������_

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"

// ���_�̏�� ========================================================
#include "Cudgel_Idling.h"


class Cudgel
{
public:
	// �Œ�l
	static const float CUDGEL_SCALE;

public:
	// ���f����ݒ肷��
	DirectX::Model* GetModel()const { return m_model.get(); }
	// �v���C�V�[���̎擾
	PlayScene* GetPlayScene()const { return m_playScene; }




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
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources
	);

	// ���E�{�b�N�X�̕`��
	void DrawBoundingBox(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::BoundingBox boundingBox
	);

	// �I������
	void Finalize();

	// �X�e�[�g�𐶐�
	void CreateState();
	// �X�e�[�g���X�V����
	void ChangeState(IWeapon* state);

private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	DirectX::SimpleMath::Vector3 m_angle;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

private:
	// ���݂̃X�e�[�g
	IWeapon* m_currentState;
	// �ҋ@���
	std::unique_ptr<Cudgel_Idling> m_idling;


private:
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �v���C�V�[���i�����蔻��̏����Ɏg�p�j
	PlayScene* m_playScene;

};