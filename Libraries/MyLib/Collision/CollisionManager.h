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
	Cudgel,
	Stage
};


/// <summary>

/// 当たり判定の形状の種類
/// </summary>
enum class CollisionType : UINT
{
	OBB,
	Sphere
};

/// <summary>
/////////////// OBBCollision構造体 /////////////////////////
/// </summary>
struct OBBCollision
{
	ObjectType							objType;
	CollisionType						colType;
	IObject*							object;
	const DirectX::BoundingOrientedBox*	obb;

	// コンストラクタ
	OBBCollision
	(
		ObjectType						type,
		IObject*						obj,
		const DirectX::BoundingOrientedBox*	obb
	) 
		: objType	(type)
		, colType	(CollisionType::OBB)
		, object	(obj)
		, obb		(obb)
	{}
};

template<typename T>
void Sum(T a, T b)
{
	return a + b;
}


/// <summary>
///	//////////// SphereCollision構造体 ////////////////////////
/// </summary>
struct SphereCollision
{
	ObjectType						objType;
	CollisionType					colType;
	IObject*						object;
	const DirectX::BoundingSphere*	sphere;

	// コンストラクタ
	SphereCollision
	(
		ObjectType					type,
		IObject*					obj,
		const DirectX::BoundingSphere*	sphere
	)
		: objType	(type)
		, colType	(CollisionType::Sphere)
		, object	(obj)
		, sphere	(sphere)
	{}
};


/// <summary>
/// //////////////// オブジェクトの情報 //////////////////
/// </summary>
struct InterSectData
{
	ObjectType		objType;
	CollisionType	colType;
	IObject*		object;
};



class CollisionManager
{
public:
	CollisionManager();		// コンストラクタ
	~CollisionManager();	// デストラクタ

	// 更新処理
	void Update();

	// 追加関数
	void AddCollision(ObjectType type, IObject* obj, const DirectX::BoundingOrientedBox* obb)	{ m_obbs	.emplace_back(OBBCollision		(type, obj, obb))	; }
	void AddCollision(ObjectType type, IObject* obj, const DirectX::BoundingSphere* sphere)		{ m_spheres	.emplace_back(SphereCollision	(type, obj, sphere)); }

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
