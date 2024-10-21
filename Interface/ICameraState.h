#pragma once

#include "pch.h"

class ICameraState
{
	virtual		~ICameraState	() = 0;		// デストラクタ
	virtual void PreUpdate		() = 0;		// ステート変更（in）
	virtual void Update			() = 0;		// 更新
	virtual void PostUpdate		() = 0;		// ステート変更（out)
};
