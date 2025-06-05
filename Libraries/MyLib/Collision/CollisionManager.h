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

// インクルード
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Interface/IRenderable.h"

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
class CollisionManager : public IRenderable
{
	// -----------------------------
	// アクセサ
	// -----------------------------
public:
	// 描画レイヤーを取得する
	UINT GetLayer() const override { return static_cast<UINT>(Layer::Object); }

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
	// 削除要求を登録するメソッド
	void QueueDeleteCollision(void* args);
	// 初期化関数
	void Clear();

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// 別スレッドへの登録
	inline void RegisterThread();
	// 別スレッドの終了
	inline void ExitThread();

	// イベントの登録
	inline void AddEventMessenger();
	// 球と球の当たり判定
	inline void CheckCollisionSphereToSphere();
	// OBBと球の当たり判定
	inline void CheckCollisionOBBToSphere();
	// 小鬼同士の衝突通知
	inline bool IsGoblinCollision(
		const CollisionData<DirectX::BoundingSphere>& collisionA,
		const CollisionData<DirectX::BoundingSphere>& collisionB
	);
	// 衝突判定の描画
	inline void DrawCollision(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection
	);
	// 描画コマンドの登録
	void RecordRenderCommands(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		ID3D11DeviceContext* deferredContext)override;


	// 有向境界ボックスのプロキシ球体を生成
	inline std::unique_ptr<DirectX::BoundingSphere> CreateProxySphere(
		const DirectX::BoundingOrientedBox* collision
	);

	// 実際に削除処理を行うメソッド（内部使用）
	void ProcessDeleteQueue();

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


	// 衝突判定専用のスレッドオブジェクト
	std::thread m_collisionThread;
	// スレッド保護用のミューテックス
	std::mutex m_mutex;
	// スレッド起床通知のための条件変数
	std::condition_variable m_cv;
	// Update関数内における衝突判定要求フラグ
	bool m_updateRequested = false;
	// デストラクタでのスレッド終了を支持するフラグ
	bool m_exitRequested = false;


	// 削除処理の待機キュー
	std::vector<DeleteCollisionData> m_pendingDeleteQueue;
	// 削除キュー保護用ミューテックス
	std::mutex m_deleteQueueMutex;
	// 衝突処理中かどうかのフラグ（主に待機判断に使用）
	std::atomic<bool> m_isProcessingCollisions;
	// Update終了通知用の条件変数
	std::condition_variable m_cv_complete;
};

#endif // !COLLISION_MANAGER
