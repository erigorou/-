#pragma once
#include "Interface/IAction.h"


class NormalAction final : public IAction
{
public:

	// ���s���e
	ActionParam Execute(ActionParam param, const float time) override
	{
		UNREFERENCED_PARAMETER(time);

		return param;
	}
};