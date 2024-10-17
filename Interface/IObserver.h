#pragma once
#ifndef OBSERVER
#define OBSERVER

#include "pch.h"
#include "IObject.h"

class IObserver : public IObject
{
public:
	// �L�[����������ꂽ�玞�ɌĂяo�����
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;

	// �L�[�����������Ă���Ăяo�����
	virtual void OnKeyDown(const DirectX::Keyboard::Keys& key) = 0;



	// �ȉ�IObject�̊֐�
	virtual void HitAction(InterSectData data) = 0;

	// ���W�̎擾
	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;
};

#endif // !OBSERVER
