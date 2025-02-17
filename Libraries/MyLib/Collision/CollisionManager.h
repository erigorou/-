// -------------------------------------------------------
//
// 名前:	CollisionManager
// 内容:	衝突判定を管理するクラス
//			オブジェクトがCollision生成時に
//			このクラスに登録され、
//			当たり判定の更新を行う
// 作成:	池田桜輔
// 
// -------------------------------------------------------
#pragma once
#ifndef COLLISION_MANAGER
#define COLLISION_MANAGER

// 前方宣言
class IObject;

// -------------------------------------------------------
/// <summary>
/// オブジェクトの種類
/// </summary>
// -------------------------------------------------------
enum class ObjectType : UINT
{
	Player, Boss, Goblin, Sword, Cudgel, Stage
};

// -------------------------------------------------------
/// <summary>
/// 衝突判定の種類
/// </summary>
// -------------------------------------------------------
enum class CollisionType : UINT
{
	OBB, Sphere
};

/// <summary>
/// 衝突判定を格納する構造体
/// ObjectType : オブジェクトの種類
/// CollisionType : 衝突判定の形状
/// IObject : 持ち主
/// T : 衝突判定
/// </summary>
/// <typeparam name="T">衝突判定</typeparam>
template<typename T>
struct CollisionData
{
	ObjectType objType;
	CollisionType colType;
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

/// <summary>
/// 衝突判定を削除する際に使用する構造体
/// CollisionType : 衝突判定の形状
/// IObject : 持ち主
/// </summary>
struct DeleteCollisionData
{
	CollisionType collType;
	IObject* object;
};

/// <summary>
/// 衝突時に相手に渡すデータ
/// </summary>
struct InterSectData
{
	ObjectType objType;	// オブジェクトの種類
	CollisionType colType; // 衝突判定の形状
	DirectX::BoundingSphere* collision; // 当たり判定のポインタ
};


/// <summary>
/// 衝突判定を管理するクラス
/// </summary>
class CollisionManager
{
	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	CollisionManager();
	// デストラクタ
	~CollisionManager();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);
	// 追加関数
	template<typename T>
	void AddCollision(void* args);
	// 削除関数
	void DeleteCollision(void* args);
	// 初期化関数
	void Clear();

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// イベントの登録
	inline void AddEventMessenger();
	// 球と球の当たり判定
	inline void CheckCollisionSphereToSphere();
	// OBBと球の当たり判定
	inline void CheckCollisionOBBToSphere();
	// 衝突判定の描画
	inline void DrawCollision(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection
	);
	// 有向境界ボックスのプロキシ球体を生成
	inline std::unique_ptr<DirectX::BoundingSphere> CreateProxySphere(
		const DirectX::BoundingOrientedBox* collision
	);

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 四角の当たり判定を格納
	std::vector<CollisionData<DirectX::BoundingOrientedBox>> m_obbs;
	// 球体の当たり判定を格納
	std::vector<CollisionData<DirectX::BoundingSphere>> m_spheres;
	// OBB衝突判定のプロキシ用球体判定を格納
	std::vector<std::unique_ptr<DirectX::BoundingSphere>> m_obbProxies;

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 描画フラグ
	bool m_drawFlag;
	// キーボードステート
	DirectX::Keyboard::State m_keyboardState;
	// キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;
};

#endif // !COLLISION_MANAGER
