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
}


// -------------------------------------------------------
/// <summary>
/// �X�V�֐�
/// </summary>
// -------------------------------------------------------
void CollisionManager::Update()
{
	 //////////////////OBB�Ƌ��̓����蔻��/////////////////////
    for (size_t i = 0; i + 1< m_obbs.size(); i++)
    {
        for (size_t j = 0; j < m_spheres.size(); j++)
        {
			/////////////////�Փ˂����ꍇ�̓����蔻����r����/////////////////////
            if (m_obbs[i].obb->Intersects(*m_spheres[j].sphere))
            {
				InterSectData obbData	= { m_obbs[i]	.objType, m_obbs[i].colType,	m_obbs[i].object	};
				InterSectData sphereData= { m_spheres[j].objType, m_spheres[i].colType,	m_spheres[j].object	};

				m_obbs[i]	.object->HitAction(sphereData);
				m_spheres[j].object->HitAction(obbData);
            }
        }
    }


	/////////////////�����m�̓����蔻��////////////////////////
	for (size_t i = 0; i + 1 < m_spheres.size(); i++)
	{
		for (size_t j = i + 1; j < m_spheres.size(); j++)
		{
			/////////////////�Փ˂����ꍇ�̓����蔻����r����/////////////////////
			if (m_spheres[i].sphere->Intersects(*m_spheres[j].sphere))
			{
				InterSectData sphereData1 = { m_spheres[i].objType, m_spheres[i].colType, m_spheres[i].object};
				InterSectData sphereData2 = { m_spheres[j].objType, m_spheres[j].colType, m_spheres[j].object};

				m_spheres[i].object->HitAction(sphereData2);
				m_spheres[j].object->HitAction(sphereData1);
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
		[object](const OBBCollision& obbCollision) { return obbCollision.object == object; }),
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
		[object](const SphereCollision& sphereCollision) { return sphereCollision.object == object; }),
		m_spheres.end());
}