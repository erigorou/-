// ----------------------------------------------
//
// ���񏑂��̂��ʓ|�ȏ������N���X������ׂ̊֐�
//
// ----------------------------------------------

#pragma once
#include "pch.h"

using namespace DirectX::SimpleMath;

class Math
{
	// �֐��̐錾
public:

	/// <summary>
	/// �����_��Z�Z�ʂ�艺��؂�̂Ă鏈��
	/// </summary>
	/// <param name="num">�؂�̂Ă鐔</param>
	/// <param name="prec">�����_��Z�Z��</param>
	/// <returns>�؂�̂Ă��l</returns>
	static float truncate_float(const float num, const int prec)
	{
		// �w�肳�ꂽ���x�Ɋ�Â��ăX�P�[�����v�Z
		float scale = std::pow(10.f, prec);
		// �����_�ȉ��� "prec" �ʁ@������؂�̂Ă��A�Ԃ��B
		return std::trunc(num * scale) / scale;
	};


	/// <summary>
	/// �����_��Z�Z�ʂ�艺��؂�̂Ă鏈��
	/// </summary>
	/// <param name="num">�؂�̂Ă鐔</param>
	/// <param name="prec">�����_��Z�Z��</param>
	/// <returns>�؂�̂Ă��l</returns>
	static Vector3 truncate_vector(Vector3 num, const int prec)
	{
		// x,y,z�����ꂼ��؂�̂Ă���
		num.x = truncate_float(num.x, prec);
		num.y = truncate_float(num.y, prec);
		num.z = truncate_float(num.z, prec);
		// �v�Z���ʂ�Ԃ�
		return num;
	}
};