#pragma once
#include "pch.h"
#include "Interface/IQuestChecker.h"


#include "Game/Scene/PlayScene.h"
#include "Game/Player/Player.h"

class QuestPlayerMove : public IQuestChecker
{
public:
	// �`�F�b�J�[�����s����
	bool ExecuteChecker(PlayScene* playScene) override
	{
		// �v���C���[�̎擾
		auto player = playScene->GetPlayer();

		// �v���C���[���ړ����Ă��邩
		return (player->GetinputVector() != DirectX::SimpleMath::Vector2::Zero);
	};
};