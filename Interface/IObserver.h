#pragma once
#ifndef OBSERVER
#define OBSERVER

#include "pch.h"
#include "IObject.h"

class IObserver
{
public:
	// キーが押下げられたら時に呼び出される
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;

	// キーが押下げられてたら呼び出される
	virtual void OnKeyDown(const DirectX::Keyboard::Keys& key) = 0;
};

#endif // !OBSERVER
