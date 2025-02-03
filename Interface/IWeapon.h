#pragma once
#include "pch.h"
#include "Libraries/MyLib/Collision/CollisionManager.h"

class IWeapon
{
public:
	// デストラクタ
	virtual~IWeapon() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 事前処理
	virtual void PreUpdate() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 事後処理
	virtual void PostUpdate() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	virtual void HitAction(InterSectData data) = 0;
};