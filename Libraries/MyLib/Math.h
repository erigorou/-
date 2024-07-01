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


	/// <summary>
	/// �v���C���[�Ɠ������Ă��鎞�ɉ����߂��̗ʂ��v������
	/// </summary>
	/// <param name="A">�����߂���鑤��Sphere</param>
	/// <param name="B">�Œ肳���ق���Sphere</param>
	/// <returns>�����߂��̒l�@[ A�̍��W�@+= return�l ]</returns>
	static Vector3 pushBack_BoundingSphere(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
	{
		// A��B���������Ă��Ȃ���Α������^�[��
		if (!A.Intersects(B))	return Vector3::Zero;

		// �Փ˔���@�v���C���[�������߂����--[====================>

		// �v���C���[�̒��S�ƓG�̒��S�Ƃ̍����x�N�g��
		Vector3 diffVec = A.Center - B.Center;
		// �v���C���[�̒��S�ƓG�̒��S�Ƃ̋������擾
		float diffLength = diffVec.Length();
		// �v���C���[�ƓG�̔��a�̍��v���擾
		float sumLength = A.Radius + B.Radius;
		// �v���C���[���߂荞�񂾋���
		diffLength = sumLength - diffLength;
		// �����x�N�g���𐳋K������
		diffVec.Normalize();
		// �����߂��x�N�g�����v�Z���A�Ԃ�
		return diffVec * diffLength;
	}
};