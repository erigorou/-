#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

#include "pch.h"

class IObject;

/// <summary>
/// 当たり判定の持ち主の種類
/// </summary>
enum class ObjectType : UINT
{
	Player,
	Enemy,
	Sword,
	Cudgel
};


/// <summary>
/////////////// OBBCollision構造体 /////////////////////////
/// </summary>
struct OBBCollision
{
	ObjectType						m_type;
	IObject*						m_object;
	DirectX::BoundingOrientedBox*	m_obb;

	// コンストラクタ
	OBBCollision
	(
		ObjectType						type,
		IObject*						obj,
		DirectX::BoundingOrientedBox*	obb
	) 
		: m_type	(type)
		, m_object	(obj)
		, m_obb		(obb)
	{}
};


/// <summary>
///	//////////// OBBCollision構造体 ////////////////////////
/// </summary>
struct SphereCollision
{
	ObjectType					m_type;
	IObject*					m_object;
	DirectX::BoundingSphere*	m_sphere;

	// コンストラクタ
	SphereCollision
	(
		ObjectType					type,
		IObject*					obj,
		DirectX::BoundingSphere*	sphere
	)
		: m_type	(type)
		, m_object	(obj)
		, m_sphere	(sphere)
	{}
};


/// <summary>
/// //////////////// オブジェクトの情報 //////////////////
/// </summary>
struct InterSectData
{
	ObjectType type;
	IObject* object;
};



class CollisionManager
{
public:
	CollisionManager();		// コンストラクタ
	~CollisionManager();	// デストラクタ

	// 更新処理
	void Update();

	// 追加関数
	void AddCollision(ObjectType type, IObject* obj, DirectX::BoundingOrientedBox	obb	) { m_obbs.		push_back(OBBCollision		(type, obj, &obb	)); }
	void AddCollision(ObjectType type, IObject* obj, DirectX::BoundingSphere	 sphere	) { m_spheres.	push_back(SphereCollision	(type, obj, &sphere	)); }

	// 削除関数
	void DeleteOBBCollision		(IObject* object);
	void DeleteSphereCollision	(IObject* object);

	// 初期化関数
	void Clear();


private:
	std::vector<OBBCollision>		m_obbs;		// 四角の当たり判定を格納
	std::vector<SphereCollision>	m_spheres;	// 球体の当たり判定を格納
};

#endif // !COLLISION_MANAGER
