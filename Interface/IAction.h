#pragma once
#include "pch.h"
#include "Game/UI/UIAnchor.h"

struct ActionParams
{
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 scale;
	float alpha;
};

class IAction
{
public:

	virtual ActionParams Execute(ActionParams param, const float time) = 0;
};