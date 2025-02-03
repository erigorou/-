#pragma once
#include "pch.h"
#include "Interface/IAction.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"

// �^�C�g�����S�̃A�N�V����
class TitleLogoAction final : public IAction
{
	// ------------------------------
	// �Œ�l
	// ------------------------------
private:
	// �x��
	static constexpr float DELAY = 1.5f;
	// �ړ�����
	static constexpr float MOVE_TIME = 1.5f;
	// �������W
	static constexpr float INIT_POSITION_Y = 200.0f;
	// �ړ�����
	static constexpr float MOVE_DISTANCE = 200.0f;

	// ------------------------------
	// ���J�֐�
	// ------------------------------
public:
	// ���s���e
	ActionParams Execute(ActionParams param, const float time) override
	{
		ActionParams result = param;

		// �x�����Ȃ�Όv�Z����
		if (time < DELAY) return result;

		// �b���𐳋K������
		float t = Math::Clamp(time - DELAY, 0.0f, MOVE_TIME) / MOVE_TIME;
		// �㉺�ɃA�j���[�V����������
		result.position.y = MOVE_DISTANCE * Easing::easeOutElastic(t) - INIT_POSITION_Y;

		// ���ʂ�Ԃ�
		return result;
	}
};