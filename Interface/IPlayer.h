#pragma once
#ifndef IPLAYER
#define IPLAYER

#include "pch.h"
#include "Interface/IState.h"

class IPlayer : public IState
{
public:
	// デストラクタ
	virtual ~IPlayer() = default;
	// 初期化する
	virtual void Initialize() = 0;
	// 事前更新する
	virtual void PreUpdate() = 0;
	// 更新する
	virtual void Update(const float& elapsedTime) = 0;

	// キー入力があった瞬間に呼び出される
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
	// キーが押下げられてたら呼び出される
	virtual void OnKeyDown(const DirectX::Keyboard::Keys& key) = 0;
	// 事後更新する
	virtual void PostUpdate() = 0;
	// 後処理を行う
	virtual void Finalize() = 0;
};

#endif // !1
