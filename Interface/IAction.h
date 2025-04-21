#pragma once
#include "pch.h"
#include "Game/UI/UIAnchor.h"

/// <summary>
/// アクションのパラメーター
/// </summary>
struct ActionParams
{
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 scale;
	float alpha = 1.0f;
	DirectX::SimpleMath::Vector3 color = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
};

class IAction
{
public:

	virtual ActionParams Execute(ActionParams param, const float time) = 0;
};