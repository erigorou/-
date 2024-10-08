// 当たり判定やるやつ

#include "pch.h"
#include "CollisionManager.h"
#include "Interface/IObject.h"

// -------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// -------------------------------------------------------
CollisionManager::CollisionManager()
{}



// -------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// -------------------------------------------------------
CollisionManager::~CollisionManager()
{
}


// -------------------------------------------------------
/// <summary>
/// 更新関数
/// </summary>
// -------------------------------------------------------
void CollisionManager::Update()
{
	 //////////////////OBBと球の当たり判定/////////////////////
    for (size_t i = 0; i + 1< m_obbs.size(); i++)
    {
        for (size_t j = 0; j < m_spheres.size(); j++)
        {
			/////////////////衝突した場合の当たり判定を比較する/////////////////////
            if (m_obbs[i].obb->Intersects(*m_spheres[j].sphere))
            {
				InterSectData obbData	= { m_obbs[i]	.objType, m_obbs[i].colType,	m_obbs[i].object	};
				InterSectData sphereData= { m_spheres[j].objType, m_spheres[i].colType,	m_spheres[j].object	};

				m_obbs[i]	.object->HitAction(sphereData);
				m_spheres[j].object->HitAction(obbData);
            }
        }
    }


	/////////////////球同士の当たり判定////////////////////////
	for (size_t i = 0; i + 1 < m_spheres.size(); i++)
	{
		for (size_t j = i + 1; j < m_spheres.size(); j++)
		{
			/////////////////衝突した場合の当たり判定を比較する/////////////////////
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
/// 当たり判定をクリア
/// </summary>
// -------------------------------------------------------
void CollisionManager::Clear()
{
	m_obbs.clear();
	m_spheres.clear();
}



// -------------------------------------------------------
/// <summary>
/// 当たり判定一覧から指定したオブジェクトの当たり判定を削除
/// </summary>
/// <param name="object">オブジェクトのポインタをIDとして使用</param>
// -------------------------------------------------------
void CollisionManager::DeleteOBBCollision(IObject* object)
{
	// m_obbsからobjectを削除
	m_obbs.erase(std::remove_if(m_obbs.begin(), m_obbs.end(),
		[object](const OBBCollision& obbCollision) { return obbCollision.object == object; }),
		m_obbs.end());
}



// -------------------------------------------------------
/// <summary>
/// 当たり判定一覧から指定したオブジェクトの当たり判定を削除
/// </summary>
/// <param name="object">オブジェクトのポインタをIDとして使用</param>
// -------------------------------------------------------
void CollisionManager::DeleteSphereCollision(IObject* object)
{
	// m_spheresからobjectを削除
	m_spheres.erase(std::remove_if(m_spheres.begin(), m_spheres.end(),
		[object](const SphereCollision& sphereCollision) { return sphereCollision.object == object; }),
		m_spheres.end());
}