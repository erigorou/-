// ----------------------------------------------
// �����蔻��Ɋւ��鏈�����L��
//
// �����f���̑傫���ʂ��BoundingBox��Ԃ��֐�
// ��
//
// ----------------------------------------------

#pragma once

#include "pch.h"							// �v���R���p�C���ς݃w�b�_�[
#include "Libraries/Microsoft/DebugDraw.h"	// �����蔻�����������̂Ɏg�p

class Collision
{
public:

    // -------------------------------------------------------------------------
    /// <summary>
    /// �w�肳�ꂽ���f����������t�����E�{�b�N�X�iOBB�j���擾
    /// </summary>
    /// <param name="model">���E�{�b�N�X���擾����Ώۂ̃��f��</param>
    /// <returns>�傫�����l������OBB</returns>
    // -------------------------------------------------------------------------
	static DirectX::BoundingOrientedBox Get_BoundingOrientedBox_FromMODEL(DirectX::Model* model)
	{
		// ���f����null�̎��ɒ��f������
		assert(model != nullptr && "model is null.");

		DirectX::BoundingBox initialAABB;
		DirectX::BoundingOrientedBox initialOBB;

        // ���f�����狫�E�{�b�N�X���擾����
        for (auto it = model->meshes.cbegin(); it != model->meshes.cend(); it++)
        {
            if (it == model->meshes.cbegin())
            {
                // �ŏ��̃��b�V���̋��E�{�b�N�X��ݒ肷��
                initialAABB = (*it)->boundingBox;
            }
            else
            {
                // ����ȍ~�̃��b�V���̋��E�{�b�N�X���}�[�W����
                DirectX::BoundingBox::CreateMerged(initialAABB, initialAABB, (*it)->boundingBox);
            }
        }

        // AABB����OBB�𐶐�����
        DirectX::BoundingOrientedBox::CreateFromBoundingBox(initialOBB, initialAABB);

		// obb��Ԃ�
		return initialOBB;
	}


};
