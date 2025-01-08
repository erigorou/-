#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"


#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

class QuestPlayerAvoid : public IQuestChecker
{
public:
	// �`�F�b�J�[�����s����
	bool ExecuteChecker(PlayScene* playScene) override
	{
		// �v���C���[�̎擾
		auto player = playScene->GetPlayer();

		// ���݂̃X�e�[�g�ƃR���{�̃X�e�[�g���擾
		auto currentState = player->GetCurrentState();
		auto attackState = player->GetPlayerDodgingState();

		// �v���C���[�̃X�e�[�g���U�������ǂ���
		return (currentState == attackState);
	};
};