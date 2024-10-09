#pragma once
#ifndef I_OBJECT
#define I_OBJECT

// オブジェクトを持つものは全て当たり判定を持つ。　My new gear...
#include "Libraries/MyLib/Collision/CollisionManager.h"

class IObject
{
public:
	// 当たったときにtrueを設定する
	virtual void HitAction(InterSectData data) = 0;

	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;
};

#endif // !I_OBJECT