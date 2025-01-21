#pragma once
#include "pch.h"
#include "Interface/IFace.h"

class Enemy;

class EnemyFaceIdling : public IFace
{

// �Œ�l
public:
	const wchar_t* MODEL_PATH = L"Resources/Models/Oni/Face/IdlingFace.cmo";

// ���J�֐�
public:
	EnemyFaceIdling(Enemy* enemy);	// �R���X�g���N�^
	~EnemyFaceIdling() override;	// �f�X�g���N�^


	void DrawFace(								// �����̕`��
		const DirectX::SimpleMath::Matrix mat,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	)	override;


// �����֐�
private:
	void CreateModel();	// ���f���̐���


// �����ϐ�
private:
	Enemy* m_enemy;				// �G�̑匳�N���X
	DirectX::Model* m_model;	// ���f��
};
