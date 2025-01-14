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
class EnemyDamageEffect;
#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"
#include "State/Header/GoblinDead.h"

class Goblin : public IEnemy
{
	// ---------------
	// 固定値
	// ---------------
public:
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	static constexpr float GOBLIN_HP = 1.0f;
	static constexpr float COLLISION_RADIUS = 16.0f;
	static constexpr float COLLISION_POS_Y = 2.0f;

	static constexpr float COOL_TIME = 0.4f;


	// ---------------
	// アクセサ
	// ---------------
public:
	DirectX::SimpleMath::Vector3	GetPosition		()	override{ return m_position;			}	// 鬼の座標を取得する
	DirectX::SimpleMath::Vector3	GetVelocity		()	const	{ return m_velocity;			}	// 速度の取得
	float							GetAngle		()	const	{ return m_angle;				}	// 回転角の取得
	DirectX::SimpleMath::Matrix		GetWorldMatrix	()	const	{ return m_worldMatrix;			}	// ワールド座標の取得
	DirectX::BoundingSphere 		GetCollision	()	const	{ return *m_bodyCollision.get();}	// 体の当たり判定の取得
	PlayScene*						GetPlayScene	()	const	{ return m_playScene;			}	// PlaySceneの取得
	bool							IsAttacking		()	const	{ return m_nowAttacking;		}	// 攻撃中かどうかの取得
	HPSystem*						GetHPSystem		()	override{ return m_hp.get();			}	// HPの取得

	void SetPosition(const DirectX::SimpleMath::Vector3& position)	{ m_position = position; }		// 鬼の座標を設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity)	{ m_velocity = velocity; }		// 速度の設定
	void SetAngle	(const float angle)								{ m_angle = angle; }			// 回転角の設定
	void SetScale	(const DirectX::SimpleMath::Vector3& scale)		{ m_scale = scale; }			// スケールの設定
	void SetIsAttacking(bool isAttacking)							{ m_nowAttacking = isAttacking; }// 攻撃中かどうかの設定

	GoblinIdling*	 GetIdling		()	const { return m_idling.get(); }	// 待機状態の取得
	GoblinAttacking* GetAttacking	()	const { return m_attacking.get(); }	// 攻撃状態の取得
	GoblinDead*		 GetDead		()	const { return m_dead.get(); }		// 死亡状態の取得
	IState* GetCurrentState			()	const { return m_currentState; }	// 現在のステートの取得

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
		const DirectX::SimpleMath::Matrix& projection);

	// 終了処理
	void Finalize();
	// 衝突処理
	void HitAction(InterSectData data) override;
	// ステートの変更
	void ChangeState(IState* state);

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
	void HitEnemy(InterSectData data);	// 敵に当たったときの処理
	void HitStage(InterSectData data);	// ステージに当たったときの処理
	void HitSword(InterSectData data);	// 剣に当たったときの処理

	void Damaged		(float damage);			// ダメージを受けたときの処理
	void CountCoolTime	(float elapsedTime);	// クールタイムのカウント

	void CreateState();		// ステートの作成


// ---------------
// メンバ変数
// ---------------
private:
	DirectX::SimpleMath::Vector3 m_position;	// 座標
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	float m_angle;								// 回転角
	DirectX::SimpleMath::Vector3 m_scale;		// スケール
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド座標

	std::unique_ptr<DirectX::Model> m_model;	// モデル

	// 押し戻し量
	DirectX::SimpleMath::Vector3 m_pushBackValue;

	// ステート用変数 **
	IState* m_currentState;								// 現在のステート
	std::unique_ptr<GoblinIdling>		m_idling;		// 待機
	std::unique_ptr<GoblinAttacking>	m_attacking;	// 攻撃
	std::unique_ptr<GoblinDead>			m_dead;			// 死亡

	// システム **
	std::unique_ptr<HPSystem> m_hp;						// HP
	std::unique_ptr<EnemyDamageEffect> m_damageEffect;	// ダメージエフェクト

	// プレイシーン(当たり判定の処理に使用)
	PlayScene* m_playScene;

	// 体の当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;

	bool m_nowAttacking;	// 攻撃中かどうか
	bool m_isHit;			// 攻撃を受けたかどうか
	float m_coolTime;		// クールタイム
};


#endif // !GOBLIN_OBJECT