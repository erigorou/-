#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER


class IObject;

/// <summary>
/// 当たり判定の持ち主の種類
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

	void Initialize();		// 初期化関数
	void Update();			// 更新関数
	void Render				// 描画関数
	(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// 追加関数
	void AddCollision(ObjectType type, IObject* obj, const DirectX::BoundingOrientedBox* obb)	{ m_obbs	.emplace_back(OBBCollision		(type, obj, obb))	; }
	void AddCollision(ObjectType type, IObject* obj, const DirectX::BoundingSphere* sphere)		{ m_spheres	.emplace_back(SphereCollision	(type, obj, sphere)); }

	// 削除関数
	void DeleteOBBCollision		(IObject* object);
	void DeleteSphereCollision	(IObject* object);

	// 初期化関数
	void Clear();

// メンバ関数
private:
	inline void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	

// メンバ変数
private:
	std::vector<OBBCollision>		m_obbs;		// 四角の当たり判定を格納
	std::vector<SphereCollision>	m_spheres;	// 球体の当たり判定を格納


	// デバッグ用に描画するための変数 *********************************
	
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	bool m_drawFlag;	// 描画フラグ


	// キーボード用の変数
	DirectX::Keyboard::State				m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;

};

#endif // !COLLISION_MANAGER
