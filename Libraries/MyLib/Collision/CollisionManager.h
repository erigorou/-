#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

// �O���錾
class IObject;

// �I�u�W�F�N�g�̎��
enum class ObjectType : UINT
{ Player, Enemy, Goblin, Sword, Cudgel, Stage };

// �Փ˔���̌`��
enum class CollisionType : UINT
{ OBB, Sphere };

// �Փ˔�����i�[����\����
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


// �Փ˂����Ƃ��ɑ���ɓn���f�[�^�̍\����
struct InterSectData
{
	ObjectType		objType;
	CollisionType	colType;
	IObject*		object;
};

// �Փ˔�����Ǘ�����N���X
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
	template<typename T>
	void AddCollision(ObjectType objType, CollisionType colType, IObject* obj, T collision);

	// �폜�֐�
	void DeleteCollision(CollisionType collType, IObject* object);

	// �������֐�
	void Clear();

// �����o�֐�
private:
	inline void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	inline DirectX::BoundingSphere CreateProxySphere(const DirectX::BoundingOrientedBox* collision);


// �����o�ϐ�
private:
	std::vector<CollisionData<DirectX::BoundingOrientedBox>>	m_obbs;			// �l�p�̓����蔻����i�[
	std::vector<CollisionData<DirectX::BoundingSphere>>			m_spheres;		// ���̂̓����蔻����i�[

	std::vector<DirectX::BoundingSphere> m_obbProxies;	// OBB�Փ˔���̃v���L�V�p���̔�����i�[


	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �`��t���O
	bool m_drawFlag;

	// �L�[�{�[�h�p�̕ϐ�
	DirectX::Keyboard::State				m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;

};

#endif // !COLLISION_MANAGER
