// ---------------------------------------------------------------------------------------
// �v���C���[���ړ����Ă��邩���`�F�b�N����N���X	: �`���[�g���A��
// ---------------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"
#include "Game/Scene/PlayScene.h"
#include "Game/EnemyManager/EnemyManager.h"

class TutorialClear : public IQuestChecker
{
public:

	// �e�N�X�`���p�X���擾����
	const wchar_t* GetTexturePath() override { return nullptr; }

	// �`���[�g���A���̃e�N�X�`���p�X���擾����
	const wchar_t* GetTutorialTexturePath() override { return L"Resources/Textures/Quest/TutorialEnd.png"; }

	// �`�F�b�J�[�����s����
	bool ExecuteChecker(PlayScene* playScene) override
	{
		// �G�}�l�[�W���[�̎擾
		auto enemyManager = playScene->GetEnemyManager();

		// �G���S�ł��Ă���Ȃ�true��Ԃ�
		return !enemyManager->IsEnemysAlive();
	};
};