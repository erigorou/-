#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

// 前方宣言
class IObject;

// オブジェクトの種類
enum class ObjectType : UINT
{ Player, Enemy, Goblin, Sword, Cudgel, Stage };

// 衝突判定の形状
enum class CollisionType : UINT
{ OBB, Sphere };

// 衝突判定を格納する構造体
template<typename T>
struct CollisionData
{
	ObjectType		objType;
	CollisionType	colType;
	IObject*		object;
	const T*		collision;

	// コンストラクタ
	CollisionData(ObjectType objType,CollisionType collType, IObject* obj, const T* collision)
		: objType(objType)
		, colType(collType)
		, object(obj)
		, collision(collision)
	{}
};


// 衝突したときに相手に渡すデータの構造体
struct InterSectData
{
	ObjectType		objType;
	CollisionType	colType;
	IObject*		object;
};

// 衝突判定を管理するクラス
class CollisionManager
{
public:

	CollisionManager();		// コンストラクタ
	~CollisionManager();	// デストラクタ

	void Initialize();		// 初期化関数
	void Update();			// 更新関数
	void Render				// 描画関数
	(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// 追加関数
	template<typename T>
	void AddCollision(ObjectType objType, CollisionType colType, IObject* obj, T collision);

	// 削除関数
	void DeleteCollision(CollisionType collType, IObject* object);

	// 初期化関数
	void Clear();

// メンバ関数
private:
	inline void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	inline DirectX::BoundingSphere CreateProxySphere(const DirectX::BoundingOrientedBox* collision);


// メンバ変数
private:
	std::vector<CollisionData<DirectX::BoundingOrientedBox>>	m_obbs;			// 四角の当たり判定を格納
	std::vector<CollisionData<DirectX::BoundingSphere>>			m_spheres;		// 球体の当たり判定を格納

	std::vector<DirectX::BoundingSphere> m_obbProxies;	// OBB衝突判定のプロキシ用球体判定を格納


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
