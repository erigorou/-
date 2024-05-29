#pragma once
#include <random>
#include "SimpleMath.h"

// 0��1�̊Ԃ̃����_����double��Ԃ�
inline float GetRandomFloat()
{
	// �񌈒�I�ȗ���������
	std::random_device rand;
	// �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h
	std::mt19937 mt(rand());
	// [low, high] �͈͂̈�l����
	std::uniform_real_distribution<> rand100(0.0f, 1.0f);
	return (float)rand100(mt);
}

// 2�̐��l�̊Ԃ̃����_����float��Ԃ�
inline float GetRandomFloat(const float& a, const float& b)
{
	// �񌈒�I�ȗ���������
	std::random_device rand;
	// �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h
	std::mt19937 mt(rand());
	// [low, high] �͈͂̈�l����
	std::uniform_real_distribution<> rand100(a, b);
	return (float)rand100(mt);
}

// 2�����̓��ς��v�Z����
inline float Dot2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.x + b.y * a.y;
}

// 2�����̊O�ς��v�Z����
inline float Cross2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}

// �x�N�g���𐳋K������
inline DirectX::SimpleMath::Vector2 Normalize(const DirectX::SimpleMath::Vector2& v1)
{
	// �x�N�g���𐳋K������
	return v1 * (1.0f / v1.Length());
}

