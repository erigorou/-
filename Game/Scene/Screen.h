// ---------------------------------------------
//
// ���O�FScreen.h
// ���e�F��ʂ̃T�C�Y���`����N���X
// �쐬�F�r�c����
//
// ---------------------------------------------

#pragma once
#include "pch.h"

class Screen
{
	// ------------------------------
	// �Œ�l
	// ------------------------------
public:
	// ��ʂ̍�
	static constexpr float LEFT = 0.0f;
	// ��ʂ̉E
	static constexpr float RIGHT = 1280.0f;
	// ��ʂ̏�
	static constexpr float TOP = 0.0f;
	// ��ʂ̉�
	static constexpr float BOTTOM = 720.0f;
	// ��ʂ̒��S
	static constexpr DirectX::SimpleMath::Vector2 CENTER = DirectX::SimpleMath::Vector2(640.0f, 360.0f);

	// ��ʂ̃T�C�Y(��)
	static constexpr float WIDTH = RIGHT - LEFT;
	// ��ʂ̃T�C�Y(�c)
	static constexpr float HEIGHT = BOTTOM - TOP;
};