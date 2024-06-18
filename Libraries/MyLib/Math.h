// ----------------------------------------------
//
// ���񏑂��̂��ʓ|�ȏ������N���X������ׂ̊֐�
//
// ----------------------------------------------

#pragma once
#include "pch.h"


class Math
{
	// �֐��̐錾
private:

	float truncate(const float num, const float prec)
	{
		// �w�肳�ꂽ���x�Ɋ�Â��ăX�P�[�����v�Z
		float scale = std::pow(10.f, prec);
		// �����_�ȉ��� "prec" �ʁ@������؂�̂Ă��A�Ԃ��B
		return std::trunc(num * scale) / scale;
	};














	// �V���O���g���������邽�߂̏���֐�
private:
	// Math
	static std::unique_ptr<Math> m_math;

public:
	// Math�N���X�̃C���X�^���X���擾����
	static Math* const GetInstance()
	{
		if (m_math == nullptr)
		{
			// Math�N���X�̃C���X�^���X�𐶐�����
			m_math.reset(new Math());
		}
		// Math�N���X�̃C���X�^���X��Ԃ�
		return m_math.get();
	}
};

// �N���X�O�ŏ�����
std::unique_ptr<Math> Math::m_math = nullptr;