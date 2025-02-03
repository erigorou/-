#pragma once
#include "pch.h"
#include "Interface/IFace.h"

class Boss;

class BossFaceIdling : public IFace
{
	// ---------------------------
	// �����o�֐��i���J�j
	// ---------------------------
public:
	// �R���X�g���N�^
	BossFaceIdling(Boss* Boss);
	// �f�X�g���N�^
	~BossFaceIdling() override;

	// ��̕`��
	void DrawFace(
		const DirectX::SimpleMath::Matrix mat,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	)	override;

	// ---------------------------
	// �����o�ϐ�
	// ---------------------------
private:
	// �G�̑匳�N���X
	Boss* m_Boss;
	// ���f��
	DirectX::Model* m_model;
};
