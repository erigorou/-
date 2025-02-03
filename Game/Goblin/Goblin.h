// -------------------------------------------------------
// 小鬼の親クラス
// -------------------------------------------------------

#pragma once

#ifndef GOBLIN_OBJECT
#define GOBLIN_OBJECT

#include "pch.h"
#include "Interface/IObject.h"
#include "Interface/IState.h"
#include "Interface/IEnemy.h"

class Player;
class PlayScene;
class HPSystem;
class EnemyEffect;

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"
#include "State/Header/GoblinDead.h"
#include "State/Header/GoblinTutorial.h"

enum class GoblinState
{
	IDLING,
	ATTACKING,
	DEAD,
	TUTORIAL
};

class Goblin : public IEnemy
{
	// ---------------
	// 固定値
	// ---------------
public:
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	static constexpr float GOBLIN_HP = 6.0f;
	static constexpr float COLLISION_RADIUS = 16.0f;
	static constexpr float COLLISION_POS_Y = 2.0f;

	static constexpr float COOL_TIME = 0.1f;

	// ---------------
	// アクセサ
	// ---------------
public:
	// 座標を取得
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }
	// 座標を設定
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// 速度を取得
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// 速度を設定
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_velocity = velocity; }
	// 回転角を取得
	float GetAngle() const { return m_angle; }
	// 回転角を設定
	void SetAngle(const float angle) { m_angle = angle; }
	// ワールド座標を取得
	DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMatrix; }
	// スケールを設定
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	// 体の当たり判定を取得
	DirectX::BoundingSphere GetCollision() const { return *m_bodyCollision.get(); }
	// 攻撃中かどうかを取得
	bool IsAttacking() const { return m_nowAttacking; }
	// HPを取得
	HPSystem* GetHPSystem() override { return m_hp.get(); }
	// 攻撃中かどうかを設定
	void SetIsAttacking(bool isAttacking) { m_nowAttacking = isAttacking; }

	// ---------------
	// 公開関数
	// ---------------
public:
	// コンストラクタ
	Goblin(PlayScene* playScene);
	// デストラクタ
	~Goblin();
	// 初期化関数
	void Initialize();
	// 当たり判定の生成
	void CreateCollision();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// 終了処理
	void Finalize();
	// 衝突可能
	void CanHit(bool flag) override { m_canHit = flag; }
	// 衝突処理
	void HitAction(InterSectData data) override;
	// ステートの変更
	void ChangeState(GoblinState state);
	// ゴブリンを消す
	void DeleteGoblin();

	// ---------------
	// 内部関数
	// ---------------
private:
	// ワールド行列の計算
	void CalcWorldMatrix();
	// 当たり判定の移動
	void MoveCollision();
	// 生存確認
	void CheckAlive();

	void HitPlayer(InterSectData data);	// プレイヤーに当たったときの処理
	void HitGoblin(InterSectData data);	// 小鬼に当たったときの処理
	void HitBoss(InterSectData data);	// 敵に当たったときの処理
	void HitStage(InterSectData data);	// ステージに当たったときの処理
	void HitSword(InterSectData data);	// 剣に当たったときの処理

	void Damaged(float damage);			// ダメージを受けたときの処理
	void CountCoolTime(float elapsedTime);	// クールタイムのカウント

	void CreateState();		// ステートの作成

	// ---------------
	// メンバ変数
	// ---------------
private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転角
	float m_angle;
	// スケール
	DirectX::SimpleMath::Vector3 m_scale;
	// ワールド座標
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	DirectX::Model* m_model;
	// 押し戻し量
	DirectX::SimpleMath::Vector3 m_pushBackValue;

	// 現在のステート
	IState* m_currentState;
	// 待機
	std::unique_ptr<GoblinIdling> m_idling;
	// 攻撃
	std::unique_ptr<GoblinAttacking> m_attacking;
	// 死亡
	std::unique_ptr<GoblinDead> m_dead;
	// チュートリアル
	std::unique_ptr<GoblinTutorial> m_tutorial;
	// ステートのリスト
	std::vector<IState*> m_states;

	// HP
	std::unique_ptr<HPSystem> m_hp;
	// 敵エフェクト
	std::unique_ptr<EnemyEffect> m_enemyEffect;

	// プレイシーン(当たり判定の処理に使用)
	PlayScene* m_playScene;

	// 体の当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;

	// 攻撃中フラグ
	bool m_nowAttacking;
	// 攻撃を受けたフラグ
	bool m_isHit;
	// 衝突可能フラグ
	bool m_canHit;
	// クールタイム
	float m_coolTime;
};

#endif // !GOBLIN_OBJECT