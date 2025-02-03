#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

// �O���錾
class IObject;

// �I�u�W�F�N�g�̎��
enum class ObjectType : UINT
{
	Player, Boss, Goblin, Sword, Cudgel, Stage
};

// �Փ˔���̌`��
enum class CollisionType : UINT
{
	OBB, Sphere
};

// �Փ˔�����i�[����\����
template<typename T>
struct CollisionData
{
	ObjectType		objType;
	CollisionType	colType;
	IObject* object;
	T* collision;

	// �R���X�g���N�^
	CollisionData(ObjectType objType, CollisionType collType, IObject* obj, T* collision)
		: objType(objType)
		, colType(collType)
		, object(obj)
		, collision(collision)
	{}
};

// �Փ˔�����폜����\����
struct DeleteCollisionData
{
	CollisionType collType;
	IObject* object;
};

// �Փ˂����Ƃ��ɑ���ɓn���f�[�^�̍\����
struct InterSectData
{
	ObjectType					objType;
	CollisionType				colType;
	DirectX::BoundingSphere* collision;
};

// �Փ˔�����Ǘ�����N���X
class CollisionManager
{
public:

	CollisionManager();		// �R���X�g���N�^
	~CollisionManager();	// �f�X�g���N�^

	void Initialize();		// �������֐�
	void Update();			// �X�V�֐�
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection); // �`��

	// �ǉ��֐�
	template<typename T>
	void AddCollision(void* args);

	// �폜�֐�
	void DeleteCollision(void* args);

	//void DeleteCollision(CollisionType collType, IObject* object);

	// �������֐�
	void Clear();

	// �����o�֐�
private:
	// �C�x���g�̓o�^
	inline void AddEventMessenger();

	inline void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	inline std::unique_ptr<DirectX::BoundingSphere> CreateProxySphere(const DirectX::BoundingOrientedBox* collision);

	// �����o�ϐ�
private:
	std::vector<CollisionData<DirectX::BoundingOrientedBox>>	m_obbs;			// �l�p�̓����蔻����i�[
	std::vector<CollisionData<DirectX::BoundingSphere>>			m_spheres;		// ���̂̓����蔻����i�[

	std::vector<std::unique_ptr<DirectX::BoundingSphere>> m_obbProxies;	// OBB�Փ˔���̃v���L�V�p���̔�����i�[

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
