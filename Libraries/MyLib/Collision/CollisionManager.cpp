// �����蔻������

#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"

// -------------------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// -------------------------------------------------------
CollisionManager::CollisionManager()
{}



// -------------------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// -------------------------------------------------------
CollisionManager::~CollisionManager()
{
	// m_obbs�̊e�I�u�W�F�N�g�����
	for (auto& obbCollision : m_obbs)
		delete obbCollision.m_obb;  // OBB�����

	// m_spheres�̊e�I�u�W�F�N�g�����
	for (auto& sphereCollision : m_spheres)
		delete sphereCollision.m_sphere;  // Sphere�����
}


// -------------------------------------------------------
/// <summary>
/// �X�V�֐�
/// </summary>
// -------------------------------------------------------
void CollisionManager::Update()
{
	 //////////////////OBB�Ƌ��̓����蔻��/////////////////////
    for (size_t i = 0; i < m_obbs.size() - 1; i++)
    {
        for (size_t j = 0; j < m_spheres.size(); j++)
        {
			/////////////////�Փ˂����ꍇ�̓����蔻����r����/////////////////////
            if (m_obbs[i].m_obb->Intersects(*m_spheres[j].m_sphere))
            {
				InterSectData obbData	= { m_obbs[i]	.m_type,	m_obbs[i].m_object		};
				InterSectData sphereData= { m_spheres[j].m_type,	m_spheres[j].m_object	};

				m_obbs[i]	.m_object->HitAction(sphereData);
				m_spheres[j].m_object->HitAction(obbData);
            }
        }
    }

	/////////////////�����m�̓����蔻��////////////////////////
	for (size_t i = 0; i < m_spheres.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_spheres.size(); j++)
		{
			/////////////////�Փ˂����ꍇ�̓����蔻����r����/////////////////////
			if (m_spheres[i].m_sphere->Intersects(*m_spheres[j].m_sphere))
			{
				InterSectData sphereData1 = { m_spheres[i].m_type, m_spheres[i].m_object };
				InterSectData sphereData2 = { m_spheres[j].m_type, m_spheres[j].m_object };

				m_spheres[i].m_object->HitAction(sphereData2);
				m_spheres[j].m_object->HitAction(sphereData1);
			}
		}
	}

}



// -------------------------------------------------------
/// <summary>
/// �����蔻����N���A
/// </summary>
// -------------------------------------------------------
void CollisionManager::Clear()
{
	m_obbs.clear();
	m_spheres.clear();
}



// -------------------------------------------------------
/// <summary>
/// �����蔻��ꗗ����w�肵���I�u�W�F�N�g�̓����蔻����폜
/// </summary>
/// <param name="object">�I�u�W�F�N�g�̃|�C���^��ID�Ƃ��Ďg�p</param>
// -------------------------------------------------------
void CollisionManager::DeleteOBBCollision(IObject* object)
{
	// m_obbs����object���폜
	m_obbs.erase(std::remove_if(m_obbs.begin(), m_obbs.end(),
		[object](const OBBCollision& obbCollision) { return obbCollision.m_object == object; }),
		m_obbs.end());
}



// -------------------------------------------------------
/// <summary>
/// �����蔻��ꗗ����w�肵���I�u�W�F�N�g�̓����蔻����폜
/// </summary>
/// <param name="object">�I�u�W�F�N�g�̃|�C���^��ID�Ƃ��Ďg�p</param>
// -------------------------------------------------------
void CollisionManager::DeleteSphereCollision(IObject* object)
{
	// m_spheres����object���폜
	m_spheres.erase(std::remove_if(m_spheres.begin(), m_spheres.end(),
		[object](const SphereCollision& sphereCollision) { return sphereCollision.m_object == object; }),
		m_spheres.end());
}