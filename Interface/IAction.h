#pragma once
#include "pch.h"
#include "Game/UI/UIAnchor.h"

struct ActionParam
{
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 scale;
};

class IAction
{
public:

	virtual ActionParam Execute(ActionParam param, const float time) = 0;
};