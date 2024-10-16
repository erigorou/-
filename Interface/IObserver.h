#pragma once
#ifndef OBSERVER
#define OBSERVER

#include "pch.h"

class IObserver
{
public:
	// ƒL[‚ª‰Ÿ‰º‚°‚ç‚ê‚½‚ç‚ÉŒÄ‚Ño‚³‚ê‚é
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
};

#endif // !OBSERVER
