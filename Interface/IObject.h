#pragma once
#ifndef I_OBJECT
#define I_OBJECT

class IObject
{
	// ���������Ƃ���true��ݒ肷��
	virtual bool HitAction() = 0;
};

#endif // !I_OBJECT