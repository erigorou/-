#pragma once
#include "Interface/IAction.h"


class NormalAction final : public IAction
{
public:

	// ���s���e
	ActionParams Execute(ActionParams param, const float time) override
	{
		UNREFERENCED_PARAMETER(time);

		return param;
	}
};