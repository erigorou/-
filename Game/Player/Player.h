// --------------------------------------------------
// 名前:	Player.h
// 内容:	プレイヤークラス
//			プレイヤーのステートや衝突判定、描画やアニメーション処理
// 作成:	池田桜輔
// --------------------------------------------------
// インクルード
#pragma once
#include "Interface/IObserver.h"
#include "Game/Player/State/Header/PlayerIdling.h"
#include "Game/Player/State/Header/PlayerDodging.h"
#include "Game/Player/State/Header/PlayerAttacking1.h"
#include "Game/Player/State/Header/PlayerAttacking2.h"
#include "Game/Player/State/Header/PlayerNockBacking.h"

// クラスの前方宣言
class Sword;
class HPSystem;

/// <summary>
/// プレイヤーのステート
/// </summary>
enum class PlayerState : int
{
	Idling,		// 待機
	Dodging,	// 回避
	Attacking1,	// 攻撃１
	Attacking2,	// 攻撃２
	NockBacking,// やられ
};

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public IObserver, public IObject
{
	// -----------------
	// 定数
	// -----------------
public:
	// 初期座標
	static constexpr DirectX::SimpleMath::Vector3 HOME_POSITION = { 40.0f, 0.0f, 40.0f };
	// 速さ
	static constexpr float PLAYER_SPEED = 0.4f;
	// 摩擦力
	static constexpr float PLAYER_FRICTION = 0.05f;
	// 加速度
	static constexpr float PLAYER_ACCELERATION = 0.05f;
	// 大きさ
	static constexpr float PLAYER_SCALE = 0.1f;
	// 当たり判定の大きさ
	static constexpr float COLLISION_RADIUS = 12.0f;
	// プレイヤーのHP
	static constexpr float PLAYER_HP = 5.0f;
	// ２段目以降の攻撃が許されるようになる時間
	static constexpr float APPLIED_ATTACK_TIME = 1.4f;
	// 回避ができるようになる時間
	static constexpr float APPLIED_DODGE_TIME = 0.5f;
	// 攻撃のクールタイム
	static constexpr float COOL_TIME = 0.5f;
	// 次の攻撃ができるようになる時間
	static constexpr float X_COOL_TIME = 0.7f;
	// 通常攻撃のアニメーション時間
	static constexpr float NORMAL_ATTACK_TIME = 0.5f;

	// ボスの攻撃力
	static constexpr float BOSS_ATTACK_POWER = 1.0f;
	// ボスの武器(金棒)の攻撃力
	static constexpr float BOSS_CUDGEL_POWER = 1.0f;
	// 小鬼の攻撃力
	static constexpr float GOBLIN_ATTACK_POWER = 1.0f;

	// ステートの最大数
	static constexpr int STATE_MAX = 5;

	// 移動時パーティクルを生成する間隔
	static constexpr float PARTICLE_INTERVAL = 0.15f;

	// キー入力
	static constexpr DirectX::SimpleMath::Vector2 INPUT_FLONT = { 0.0f	, 1.0f };	// 前
	static constexpr DirectX::SimpleMath::Vector2 INPUT_BACK = { 0.0f	, -1.0f };	// 後
	static constexpr DirectX::SimpleMath::Vector2 INPUT_LEFT = { -1.0f	, 0.0f };	// 左
	static constexpr DirectX::SimpleMath::Vector2 INPUT_RIGHT = { 1.0f	, 0.0f };	// 右

	// -----------------
	// アクセサ
	// -----------------
public:
	// プレイヤーのオブジェクトを取得
	Player* GetObject() { return this; }
	// 座標を取得
	DirectX::SimpleMath::Vector3 GetPosition()override { return m_position; }
	// 座標を設定
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	// 速度を取得
	DirectX::SimpleMath::Vector3 GetVelocity()	const { return m_velocity; }
	// 速度を設定
	void SetSpeed(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }
	// 向きを取得
	DirectX::SimpleMath::Vector3 GetDirection()	const { return m_direction; }
	// 入力ベクトルを取得
	DirectX::SimpleMath::Vector2 GetinputVector()	const { return m_inputVector; }
	// 回転角を取得
	float GetAngle()	const { return m_angle; }
	// 回転角を設定
	void SetAngle(const float angle) { m_angle = angle; }
	// HPを取得
	HPSystem* GetPlayerHP()	const { return m_hp.get(); }
	// アニメーション用回転を設定
	void SetAnimationRotate(DirectX::SimpleMath::Vector3 rotate) { m_animationRotate = rotate; }
	// 入力ベクトルを設定
	void SetInputVector(DirectX::SimpleMath::Vector2 inputVector) { m_inputVector = inputVector; }
	// 加速度を設定
	void SetAcceleration(DirectX::SimpleMath::Vector3 acceleration) { m_acceleration = acceleration; }
	// 現在のステートを取得
	IPlayer* GetCurrentState() const { return m_currentState; }
	// 回避ステートを取得
	IPlayer* GetPlayerDodgingState() const { return m_playerDodging.get(); }
	// 攻撃ステートを取得
	IPlayer* GetPlayerAttackingState1() const { return m_playerAttacking1.get(); }
	// 攻撃ステートを取得
	IPlayer* GetPlayerAttackingState2() const { return m_playerAttacking2.get(); }	


	// -------------------
	// 公開関数
	// -------------------
public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();
	//  ステートを変更する
	void ChangeState(PlayerState state);
	// 初期化処理
	void Initialize();
	// 時間計測を行う
	void TimeComparison(float& nowTime, const float totalTime, PlayerState state, const float elapsedTime);
	// 更新処理
	void Update(const float elapsedTime);
	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
	// 終了処理
	void Finalize();
	// 回転角の計算
	float CalucratePlayerRotation(DirectX::SimpleMath::Vector3 const enemyPos);
	// Matrixの計算
	void CalculationMatrix();
	// 移動の管理
	void MovePlayer();
	// 当たったときの処理
	void HitAction(InterSectData data) override;
	// キーボードの入力を取得する
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// キーボードの入力を取得する
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;
	// 被ダメを受けられる
	void CanHitBoss(void* flag);
	// 武器との当たり判定
	void CanHitCudgel(void* flag);
	// ゴブリンとの当たり判定
	void CanHitGoblin(void* flag);

	// -------------------
	// 内部関数
	// -------------------
private:
	// 当たり判定の生成関数
	void CreateCollision();
	// ステートの作成関数
	void CreateState();
	// 描画関連の初期化設定
	void InitializeRender();
	// イベントの登録
	void AttachEvent();
	// クールタイムの計算
	void CalculationCoolTime(const float elapsedTime);
	// 敵の体との衝突判定
	void HitBossBody(InterSectData data);
	// 敵のブキとの衝突判定
	void HitCudgel(InterSectData data);
	// ゴブリンと衝突
	void HitGoblin(InterSectData data);
	// ステージとの衝突判定
	void HitStage(InterSectData data);
	// ダメージ
	void Damage(float damage);

	// -------------------
	// 内部変数
	// -------------------
private:
	// 武器
	std::unique_ptr<Sword> m_sword;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 入力保持用変数
	DirectX::SimpleMath::Vector2 m_inputVector;
	// 向き
	DirectX::SimpleMath::Vector3 m_direction;
	// 加速度
	DirectX::SimpleMath::Vector3 m_acceleration;
	// 回転アニメーション
	DirectX::SimpleMath::Vector3 m_animationRotate;
	// 回転角
	float m_angle;
	// 前後の傾角
	float m_tilt;
	// 押し戻し量
	DirectX::SimpleMath::Vector3 m_pushBackValue;
	// プレイヤー用のワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 移動キーが押されているか
	bool m_isInputMoveKey;

	// 現在のステート
	IPlayer* m_currentState;
	// 待機状態
	std::unique_ptr<PlayerIdling> m_playerIdling;
	// 回避状態
	std::unique_ptr<PlayerDodging> m_playerDodging;
	// 攻撃状態
	std::unique_ptr<PlayerAttacking1> m_playerAttacking1;
	// 攻撃状態
	std::unique_ptr<PlayerAttacking2> m_playerAttacking2;
	// やられ状態
	std::unique_ptr<PlayerNockBacking> m_playerNockBacking;
	// ステートのリスト
	IPlayer* m_states[STATE_MAX];

	// HPシステム
	std::unique_ptr<HPSystem> m_hp;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// パーティクルの時間
	float m_particleTime;
	// 経過時間
	float m_elapsedTime;
	// モデル
	DirectX::Model* m_model;

	// 衝突判定
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
	// 衝突したか
	bool m_isHit;
	// 衝突判定のクールタイム
	float m_coolTime;

	// ボスからの攻撃を受けられるか
	bool m_canHitBoss;
	// ボスの武器からの攻撃を受けられるか
	bool m_canHitCudgel;
	// ゴブリンからの攻撃を受けられるか
	bool m_canHitGoblin;
};
