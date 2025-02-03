#pragma once
#include "pch.h"
#include "Interface/IAction.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Data/GameData.h"

// �`���[�g���A���{�^���̃A�N�V����
class TutorialButtonAction final : public IAction
{
	// ------------------------------
	// �Œ�l
	// ------------------------------
private:
	// �x��
	static constexpr float DELAY = 2.0f;
	// �ړ�����
	static constexpr float MOVE_TIME = 1.5f;
	// �������W
	static constexpr float INIT_POSITION_Y = 2080.0f;
	// �ړ�����
	static constexpr float MOVE_DISTANCE = 800.0f;


	// ------------------------------
	// ���J�֐�
	// ------------------------------
public:
	// ���s���e
	ActionParams Execute(ActionParams param, const float time) override
	{
		ActionParams result = param;

		// �`���[�g���A���{�^���̓����x��ݒ�
		if (GameData::GetInstance()->GetSelectStage() != 0)
			result.alpha = 0.5f;

		// �x�����Ȃ�Όv�Z����
		if (time < DELAY) return result;

		// �b���𐳋K������
		float t = Math::Clamp(time - DELAY, 0.0f, MOVE_TIME) / MOVE_TIME;
		// �㉺�ɃA�j���[�V����������
		result.position.x = INIT_POSITION_Y - MOVE_DISTANCE * Easing::easeOutBack(t);

		// ���ʂ�Ԃ�
		return result;
	}
};