#pragma once	// 多重読み込み防止
#include "Interface/IState.h"
#include "Interface/IObject.h"
#include "Interface/IFace.h"
#include "Interface/IEnemy.h"

class PlayScene;
class BehaviorTree;
class EnemyEffect;
class HPSystem;
class Cudgel;

#include "Face/Header/BossFaceIdling.h"
#include "Face/Header/BossFaceAttacking.h"

// ===== 敵の状態 =================================================================
class BossStarting;
class BossIdling;
class BossAttacking;
class BossSweeping;
class BossDashAttacking;
class BossApproaching;
class BossDead;

// 敵の状態
enum class BossState
{
	Starting = 0,	// 開始状態
	Idling,			// 待機状態
	Attacking,		// たたきつけ攻撃
	Sweeping,		// 薙ぎ払い攻撃
	DashAttacking,	// 突撃状態
	Approaching,	// 追尾状態
	Dead,			// 死亡状態
};

// 顔の状態
enum class FaceState
{
	Idling = 0,	// 待機
	Attacking,	// 攻撃
};

class Boss : public IEnemy
{
	// --------------------------------
	//  固定値
	// --------------------------------
public:
	// 速さ
	static constexpr float BOSS_SPEED = 0.1f;
	// 大きさ
	static constexpr float BOSS_SCALE = 0.6f;
	// 攻撃のクールタイム
	static constexpr float COOL_TIME = 0.3f;
	// 衝突判定のY座標
	static constexpr float COLISION_POS_Y = 10.0f;
	// 最大HP
	static constexpr float HP = 1.0f;
	// 当たり判定の大きさ
	static constexpr float COLLISION_RADIUS = 20.0f;
	// ボスの高さ
	static constexpr float BOSS_HEIGHT = 50.0f;

	// カメラの揺れる強さ
	static constexpr float SHAKE_POWER = 0.25f;

	// --------------------------------
	//  アクセサ関数
	// --------------------------------
public:
	// HPの取得・設定
	HPSystem* GetBossHP() const { return m_hp.get(); }
	HPSystem* GetHPSystem() override { return m_hp.get(); }

	// 座標の取得・設定
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }
	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }

	// 回転角の取得・設定
	float GetAngle() const { return m_angle; }
	void SetAngle(const float angle) { m_angle = angle; }

	// 体の傾きの取得・設定
	float GetBodyTilt() const { return m_bodyTilt; }
	void SetBodyTilt(const float tilt) { m_bodyTilt = tilt; }

	// 衝突可能
	void CanHit(bool flag) override { m_canHit = flag; }

public:
	// 刀のダメージを受ける許可を出す
	void CanHitSword() { m_canHit = true; }
	// 刀のダメージを受ける許可を取り消す
	void CanNotHitSword() { m_canHit = false; }

	// --------------------------------
	//  メンバ関数(公開)
	// --------------------------------
public:
	// コンストラクタ
	Boss();
	// デストラクタ
	~Boss();
	// 初期化処理
	void Initialize();
	// 状態を変更する
	void ChangeState(void* state);
	// 顔を変更する
	void ChangeFace(void* face);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// 終了処理
	void Finalize();
	// 死亡処理を行う
	void DeadAction();
	// 当たったときの処理
	void HitAction(InterSectData data)override;

	// --------------------------------
	//  メンバ関数(非公開)
	// --------------------------------
private:
	// ワールド行列の計算
	void CalcrationWorldMatrix();
	// ステートの作成処理
	void CreateState();
	// 顔の作成処理
	void CreateFace();
	// 当たり判定の生成処理
	void CreateCollision();
	// イベントの登録
	void AttachEvent();
	// 衝突クールタイムを計測
	void CheckHitCoolTime(float elapsedTime);
	// 刀の衝突判定
	void HitSword(InterSectData data);
	// ステージの衝突判定
	void HitStage(InterSectData data);
	// 死亡処理
	void CheckAlive();

	// --------------------------------
	//  メンバ変数
	// --------------------------------
private:
	// 武器
	std::unique_ptr<Cudgel> m_cudgel;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 角度
	float m_angle;
	// 体の傾き
	float m_bodyTilt;
	// 敵用のワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// HP
	std::unique_ptr<HPSystem> m_hp;

	// 現在のステート（ステートパターン）
	IState* m_currentState;
	// 開始状態
	std::unique_ptr<BossStarting> m_starting;
	// 待機状態
	std::unique_ptr<BossIdling> m_idling;
	// 攻撃状態
	std::unique_ptr<BossAttacking> m_attacking;
	// 薙ぎ払い状態
	std::unique_ptr<BossSweeping> m_sweeping;
	// 突撃状態
	std::unique_ptr<BossDashAttacking> m_dashAttacking;
	// 追尾状態
	std::unique_ptr<BossApproaching> m_approaching;
	// 死亡状態
	std::unique_ptr<BossDead> m_dead;
	// 状態の配列
	std::vector<IState*> m_states;

	// 現在の顔
	IFace* m_currentFace;
	// 待機顔
	std::unique_ptr<BossFaceIdling>	m_faceIdling;
	// 攻撃顔
	std::unique_ptr<BossFaceAttacking>	m_faceAttacking;
	// 顔の配列
	std::vector<IFace*> m_faces;

	// 敵のダメージエフェクト
	std::unique_ptr<EnemyEffect> m_effect;
	// ビヘイビアツリー
	std::unique_ptr<BehaviorTree> m_pBT;
	// モデル
	DirectX::Model* m_model;
	//　エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// 押し戻し量の計算
	DirectX::SimpleMath::Vector3 m_pushBackValue;
	// 体の当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
	// 衝突しているか
	bool m_isHit;
	// 衝突クールタイム
	float m_coolTime;
	// 衝突可能か
	bool m_canHit;
	// カメラの揺らすちから
	float m_shakePower;
};
