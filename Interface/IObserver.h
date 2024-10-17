#pragma once
#ifndef OBSERVER
#define OBSERVER

#include "pch.h"
#include "IObject.h"

class IObserver : public IObject
{
public:
	// キーが押下げられたら時に呼び出される
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;

	// キーが押下げられてたら呼び出される
	virtual void OnKeyDown(const DirectX::Keyboard::Keys& key) = 0;



	// 以下IObjectの関数
	virtual void HitAction(InterSectData data) = 0;

	// 座標の取得
	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;
};

#endif // !OBSERVER
