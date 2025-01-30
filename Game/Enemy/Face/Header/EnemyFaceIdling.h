#pragma once
#include "pch.h"
#include "Interface/IFace.h"

class Enemy;

class EnemyFaceIdling : public IFace
{
// ---------------------------
// �����o�֐��i���J�j
// ---------------------------
public:
	// �R���X�g���N�^
	EnemyFaceIdling(Enemy* enemy);
	// �f�X�g���N�^
	~EnemyFaceIdling() override;

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
	Enemy* m_enemy;
	// ���f��
	DirectX::Model* m_model;
};
