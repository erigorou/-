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
	Cudgel
};


/// <summary>
/////////////// OBBCollision�\���� /////////////////////////
/// </summary>
struct OBBCollision
{
	ObjectType						m_type;
	IObject*						m_object;
	DirectX::BoundingOrientedBox*	m_obb;

	// �R���X�g���N�^
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
///	//////////// OBBCollision�\���� ////////////////////////
/// </summary>
struct SphereCollision
{
	ObjectType					m_type;
	IObject*					m_object;
	DirectX::BoundingSphere*	m_sphere;

	// �R���X�g���N�^
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
/// //////////////// �I�u�W�F�N�g�̏�� //////////////////
/// </summary>
struct InterSectData
{
	ObjectType type;
	IObject* object;
};



class CollisionManager
{
public:
	CollisionManager();		// �R���X�g���N�^
	~CollisionManager();	// �f�X�g���N�^

	// �X�V����
	void Update();

	// �ǉ��֐�
	void AddCollision(ObjectType type, IObject* obj, DirectX::BoundingOrientedBox	obb	) { m_obbs.		push_back(OBBCollision		(type, obj, &obb	)); }
	void AddCollision(ObjectType type, IObject* obj, DirectX::BoundingSphere	 sphere	) { m_spheres.	push_back(SphereCollision	(type, obj, &sphere	)); }

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
