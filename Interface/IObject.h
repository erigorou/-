#pragma once
#ifndef I_OBJECT
#define I_OBJECT

// �I�u�W�F�N�g�������̂͑S�ē����蔻������B�@My new gear...
#include "Libraries/MyLib/Collision/CollisionManager.h"

class IObject
{
public:
	// ���������Ƃ���true��ݒ肷��
	virtual void HitAction(InterSectData data) = 0;

	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;
};

#endif // !I_OBJECT