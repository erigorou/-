#pragma once

#include "pch.h"

class ICameraState
{
	virtual		~ICameraState	() = 0;		// �f�X�g���N�^
	virtual void PreUpdate		() = 0;		// �X�e�[�g�ύX�iin�j
	virtual void Update			() = 0;		// �X�V
	virtual void PostUpdate		() = 0;		// �X�e�[�g�ύX�iout)
};
