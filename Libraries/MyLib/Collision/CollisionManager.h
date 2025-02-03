#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

// 前方宣言
class IObject;

// オブジェクトの種類
enum class ObjectType : UINT
{
	Player, Boss, Goblin, Sword, Cudgel, Stage
};

// 衝突判定の形状
enum class CollisionType : UINT
{
	OBB, Sphere
};

// 衝突判定を格納する構造体
template<typename T>
struct CollisionData
{
	ObjectType		objType;
	CollisionType	colType;
	IObject* object;
	T* collision;

	// コンストラクタ
	CollisionData(ObjectType objType, CollisionType collType, IObject* obj, T* collision)
		: objType(objType)
		, colType(collType)
		, object(obj)
		, collision(collision)
	{}
};

// 衝突判定を削除する構造体
struct DeleteCollisionData
{
	CollisionType collType;
	IObject* object;
};

// 衝突したときに相手に渡すデータの構造体
struct InterSectData
{
	ObjectType					objType;
	CollisionType				colType;
	DirectX::BoundingSphere* collision;
};

// 衝突判定を管理するクラス
class CollisionManager
{
public:

	CollisionManager();		// コンストラクタ
	~CollisionManager();	// デストラクタ

	void Initialize();		// 初期化関数
	void Update();			// 更新関数
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection); // 描画

	// 追加関数
	template<typename T>
	void AddCollision(void* args);

	// 削除関数
	void DeleteCollision(void* args);

	//void DeleteCollision(CollisionType collType, IObject* object);

	// 初期化関数
	void Clear();

	// メンバ関数
private:
	// イベントの登録
	inline void AddEventMessenger();

	inline void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	inline std::unique_ptr<DirectX::BoundingSphere> CreateProxySphere(const DirectX::BoundingOrientedBox* collision);

	// メンバ変数
private:
	std::vector<CollisionData<DirectX::BoundingOrientedBox>>	m_obbs;			// 四角の当たり判定を格納
	std::vector<CollisionData<DirectX::BoundingSphere>>			m_spheres;		// 球体の当たり判定を格納

	std::vector<std::unique_ptr<DirectX::BoundingSphere>> m_obbProxies;	// OBB衝突判定のプロキシ用球体判定を格納

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 描画フラグ
	bool m_drawFlag;

	// キーボード用の変数
	DirectX::Keyboard::State				m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;
};

#endif // !COLLISION_MANAGER
