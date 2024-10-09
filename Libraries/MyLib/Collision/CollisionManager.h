#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

#include "pch.h"

class IObject;

/// <summary>
/// �����蔻��̎�����̎��
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

/// �����蔻��̌`��̎��
/// </summary>
enum class CollisionType : UINT
{
	OBB,
	Sphere
};

/// <summary>
/////////////// OBBCollision�\���� /////////////////////////
/// </summary>
struct OBBCollision
{
	ObjectType							objType;
	CollisionType						colType;
	IObject*							object;
	const DirectX::BoundingOrientedBox*	obb;

	// �R���X�g���N�^
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
///	//////////// SphereCollision�\���� ////////////////////////
/// </summary>
struct SphereCollision
{
	ObjectType						objType;
	CollisionType					colType;
	IObject*						object;
	const DirectX::BoundingSphere*	sphere;

	// �R���X�g���N�^
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
/// //////////////// �I�u�W�F�N�g�̏�� //////////////////
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
	CollisionManager();		// �R���X�g���N�^
	~CollisionManager();	// �f�X�g���N�^

	// �X�V����
	void Update();

	// �ǉ��֐�
	void AddCollision(ObjectType type, IObject* obj, const DirectX::BoundingOrientedBox* obb)	{ m_obbs	.emplace_back(OBBCollision		(type, obj, obb))	; }
	void AddCollision(ObjectType type, IObject* obj, const DirectX::BoundingSphere* sphere)		{ m_spheres	.emplace_back(SphereCollision	(type, obj, sphere)); }

	// �폜�֐�
	void DeleteOBBCollision		(IObject* object);
	void DeleteSphereCollision	(IObject* object);

	// �������֐�
	void Clear();


private:
	std::vector<OBBCollision>		m_obbs;		// �l�p�̓����蔻����i�[
	std::vector<SphereCollision>	m_spheres;	// ���̂̓����蔻����i�[
};

#endif // !COLLISION_MANAGER
