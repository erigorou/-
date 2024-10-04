#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

#include "pch.h"

class IObject;

/// <summary>
/// 当たり判定の持ち主の種類
/// </summary>
enum class CollisionTag : UINT
{
	Player,
	Enemy,
	Sword,
	Cudgel
};


/// <summary>
///	OBBCollision構造体
/// </summary>
struct OBBCollision
{
	CollisionTag					m_tag;
	IObject*						m_object;
	DirectX::BoundingOrientedBox*	m_obb;

	// コンストラクタ
	OBBCollision
	(
		CollisionTag					tag,
		IObject*						obj,
		DirectX::BoundingOrientedBox*	obb
	) 
		: m_tag		(tag)
		, m_object	(obj)
		, m_obb		(obb)
	{}
};



/// <summary>
///	OBBCollision構造体
/// </summary>
struct SphereCollision
{
	CollisionTag				m_tag;
	IObject*					m_object;
	DirectX::BoundingSphere*	m_sphere;

	// コンストラクタ
	SphereCollision
	(
		CollisionTag				tag,
		IObject*					obj,
		DirectX::BoundingSphere*	sphere
	)
		: m_tag		(tag)
		, m_object	(obj)
		, m_sphere	(sphere)
	{}
};



class CollisionManager
{
public:
	CollisionManager();		// コンストラクタ
	~CollisionManager();	// デストラクタ

	// 更新処理
	void Update();

	// 当たり判定を追加
	void AddCollision(CollisionTag tag, IObject* obj, DirectX::BoundingOrientedBox obb	) { m_obbs.		push_back(OBBCollision		(tag, obj, &obb		)); }
	void AddCollision(CollisionTag tag, IObject* obj, DirectX::BoundingSphere	sphere	) { m_spheres.	push_back(SphereCollision	(tag, obj, &sphere	)); }

	// 当たり判定を削除
	void DeleteOBBCollision		(IObject* object);
	void DeleteSphereCollision	(IObject* object);

	// 当たり判定をクリア
	void Clear();


private:
	std::vector<OBBCollision>		m_obbs;		// 四角の当たり判定を格納
	std::vector<SphereCollision>	m_spheres;	// 球体の当たり判定を格納
};

#endif // !COLLISION_MANAGER
