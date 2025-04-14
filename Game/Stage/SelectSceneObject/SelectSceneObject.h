// ---------------------------------------------
// 
// ���O		:SelectSceneObject.h
// ���e		:�Z���N�g�V�[���ŕ\������I�u�W�F�N�g
// �쐬		:�r�c����
// 
// ---------------------------------------------

#pragma once
// �C���N���[�h
#include "pch.h"

class SelectSceneObject
{
	// -----------------------------
	// �萔
	// -----------------------------
public:
	// ���f���̉�]���x
	static constexpr float ROTATE_SPEED = 0.5f;

	// ���f���̈ʒu
	static constexpr DirectX::SimpleMath::Vector3 MODEL_POSITION = DirectX::SimpleMath::Vector3(
		20.0f,
		0.0f,
		70.0f
	);

	// -----------------------------
	// �����o�֐��i���J�j
	// -----------------------------
public:
	// �R���X�g���N�^
	SelectSceneObject();
	// �f�X�g���N�^
	~SelectSceneObject();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj);
	// �I������
	void Finalize();

	// -----------------------------
	// �����o�֐�(����J)
	// -----------------------------
private:
	// ���f���̎󂯎��
	void LoadModel();
	// ���[���h�s��̌v�Z
	void CalcWorldMatrix();

	// -----------------------------
	// �����o�ϐ�
	// -----------------------------
private:
	// ���f���̔z��
	std::vector<DirectX::Model*> m_models;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ��]
	DirectX::SimpleMath::Vector3 m_rotation;

	// ���̃X�e�[�W�ԍ�
	int m_stageNo;
};