#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

#include "pch.h"

class IObject;

/// <summary>
/// �����蔻��̎�����̎��
/// </summary>
enum class CollisionTag : UINT
{
	Player,
	Enemy,
	Sword,
	Cudgel
};


/// <summary>
///	OBBCollision�\����
/// </summary>
struct OBBCollision
{
	CollisionTag					m_tag;
	IObject*						m_object;
	DirectX::BoundingOrientedBox*	m_obb;

	// �R���X�g���N�^
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
///	OBBCollision�\����
/// </summary>
struct SphereCollision
{
	CollisionTag				m_tag;
	IObject*					m_object;
	DirectX::BoundingSphere*	m_sphere;

	// �R���X�g���N�^
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
	CollisionManager();		// �R���X�g���N�^
	~CollisionManager();	// �f�X�g���N�^

	// �X�V����
	void Update();

	// �����蔻���ǉ�
	void AddCollision(CollisionTag tag, IObject* obj, DirectX::BoundingOrientedBox obb	) { m_obbs.		push_back(OBBCollision		(tag, obj, &obb		)); }
	void AddCollision(CollisionTag tag, IObject* obj, DirectX::BoundingSphere	sphere	) { m_spheres.	push_back(SphereCollision	(tag, obj, &sphere	)); }

	// �����蔻����폜
	void DeleteOBBCollision		(IObject* object);
	void DeleteSphereCollision	(IObject* object);

	// �����蔻����N���A
	void Clear();


private:
	std::vector<OBBCollision>		m_obbs;		// �l�p�̓����蔻����i�[
	std::vector<SphereCollision>	m_spheres;	// ���̂̓����蔻����i�[
};

#endif // !COLLISION_MANAGER
