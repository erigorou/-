#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER


class IObject;

/// <summary>
/// �����蔻��̎�����̎��
/// </summary>
enum class ObjectType : UINT
{
	Player,
	Enemy,
	Goblin,
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


template<typename T>
struct CollisionData
{
	ObjectType		objType;
	CollisionType	colType;
	IObject*		object;
	const T*		collision;

	// �R���X�g���N�^
	CollisionData(ObjectType objType,CollisionType collType, IObject* obj, const T* collision)
		: objType(objType)
		, colType(collType)
		, object(obj)
		, collision(collision)
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

	void Initialize();		// �������֐�
	void Update();			// �X�V�֐�
	void Render				// �`��֐�
	(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// �ǉ��֐�
	void AddCollision(ObjectType objType, IObject* obj, const DirectX::BoundingOrientedBox* obb)	
	{ 
		m_obbs.emplace_back(CollisionData<DirectX::BoundingOrientedBox>(objType,CollisionType::OBB, obj, obb)); 
	}

	void AddCollision(ObjectType objType, IObject* obj, const DirectX::BoundingSphere* sphere)		
	{ 
		m_spheres	.emplace_back(CollisionData<DirectX::BoundingSphere>(objType,CollisionType::Sphere, obj, sphere)); 
	}

	// �e���v���[�g�폜�֐�
	void DeleteCollision(CollisionType collType, IObject* object);

	// �������֐�
	void Clear();

// �����o�֐�
private:
	inline void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	

// �����o�ϐ�
private:
	std::vector<CollisionData<DirectX::BoundingOrientedBox>>	m_obbs;		// �l�p�̓����蔻����i�[
	std::vector<CollisionData<DirectX::BoundingSphere>>			m_spheres;	// ���̂̓����蔻����i�[



	// �f�o�b�O�p�ɕ`�悷�邽�߂̕ϐ� *********************************
	
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	bool m_drawFlag;	// �`��t���O


	// �L�[�{�[�h�p�̕ϐ�
	DirectX::Keyboard::State				m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;

};

#endif // !COLLISION_MANAGER
