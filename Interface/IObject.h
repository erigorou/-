#pragma once
#ifndef I_OBJECT
#define I_OBJECT

#include "Libraries/MyLib/Collision/CollisionManager.h"

class IObject
{
public:
	// “–‚½‚Á‚½‚Æ‚«‚Étrue‚ğİ’è‚·‚é
	virtual void HitAction(InterSectData data) = 0;

	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;
};

#endif // !I_OBJECT