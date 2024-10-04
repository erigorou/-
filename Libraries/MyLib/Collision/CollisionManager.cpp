// 当たり判定やるやつ

#include "pch.h"
#include "CollisionManager.h"


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
	// m_obbsの各オブジェクトを解放
	for (auto& obbCollision : m_obbs)
		delete obbCollision.m_obb;  // OBBを解放

	// m_spheresの各オブジェクトを解放
	for (auto& sphereCollision : m_spheres)
		delete sphereCollision.m_sphere;  // Sphereを解放
}


// -------------------------------------------------------
/// <summary>
/// 更新関数
/// </summary>
// -------------------------------------------------------
void CollisionManager::Update()
{
 //////////////////OBBと球の当たり判定/////////////////////
    for (size_t i = 0; i < m_obbs.size() - 1; i++)
    {
        for (size_t j = 0; j < m_spheres.size(); j++)
        {
            if (m_obbs[i].m_obb->Intersects(*m_spheres[j].m_sphere))
            {
                // 衝突した場合の処理をここに書く
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
		[object](const OBBCollision& obbCollision) { return obbCollision.m_object == object; }),
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
		[object](const SphereCollision& sphereCollision) { return sphereCollision.m_object == object; }),
		m_spheres.end());
}