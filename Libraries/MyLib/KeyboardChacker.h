#pragma once
#include "pch.h"

class KeyboardChacker
{
public:

	static bool IsInputArrowKey(DirectX::Keyboard::Keys key)
	{
		return
			key == DirectX::Keyboard::Keys::Up ||
			key == DirectX::Keyboard::Keys::Down ||
			key == DirectX::Keyboard::Keys::Left ||
			key == DirectX::Keyboard::Keys::Right;
	}
};