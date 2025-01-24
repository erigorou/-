#pragma once	// 多重読み込み防止
#include "Interface/IState.h"
#include "Interface/IObject.h"
#include "Interface/IFace.h"
#include "Interface/IEnemy.h"

class PlayScene;
class BehaviorTree;
class EnemyDamageEffect;
class EnemyDeadEffect;
class HPSystem;


#include "Face/Header/EnemyFaceIdling.h"
#include "Face/Header/EnemyFaceAttacking.h"

// ===== 敵の状態 =================================================================
#include "States/Header/EnemyStarting.h"		// 開始状態
#include "States/Header/EnemyIdling.h"			// 待機状態
#include "States/Header/Enemy_Attacking.h"		// たたきつけ攻撃
#include "States/Header/Enemy_Sweeping.h"		// 薙ぎ払い攻撃
#include "States/Header/EnemyDashAttacking.h"	// 突撃状態
#include "States/Header/EnemyApproaching.h"		// 追尾状態
#include "States/Header/EnemyDead.h"			// 死亡状態



class Enemy : public IEnemy
{
public:
	// 固定値
	static const float ENEMY_SPEED;
	static const float ENEMY_SCALE;
	static const float COOL_TIME;

	static constexpr float COLISION_POS_Y	= 10.0f;
	static constexpr float HP				= 20.0f;
	static constexpr float COLLISION_RADIUS = 20.0f;
	static constexpr float ENEMY_HEIGHT		= 50.0f;


public:
	// 刀のダメージを受ける許可を出す
	void CanHitSword() { m_canHit = true; }
	// 刀のダメージを受ける許可を取り消す
	void CanNotHitSword() { m_canHit = false; }

public:
	// /////////////////敵の基礎情報を渡す関数/////////////////////////////////////////////////////////////////////
	PlayScene*						GetPlayScene	()	const	{ return m_playScene;	}	// PlaySceneの取得 
	HPSystem*						GetEnemyHP		()	const	{ return m_hp.get();	}	// HPの取得	
	DirectX::SimpleMath::Vector3	GetPosition		()	override{ return m_position;	}	// 鬼の座標を取得する
	float							GetAngle		()	const	{ return m_angle;		}	// 鬼の回転角を取得する
	float							GetBodyTilt		()	const	{ return m_bodyTilt;	}	// 体の傾きを取得する
	DirectX::SimpleMath::Matrix		GetWorldMatrix	()	const	{ return m_worldMatrix; }	// 敵のワールド座標を取得する
	HPSystem*						GetHPSystem		()	override{ return m_hp.get();	}	// HPの取得

	void SetPosition	(const DirectX::SimpleMath::Vector3 pos)	{ m_position = pos;		}	// 鬼の座標を設定する
	void SetAngle		(const float angle)							{ m_angle = angle;		}	// 鬼の回転角を設定する
	void SetBodyTilt	(const float tilt)							{ m_bodyTilt = tilt;	}	// 体の傾きを設定する
	void SetWorldMatrix	(DirectX::SimpleMath::Matrix mat)			{ m_worldMatrix = mat;	}	// 敵のワールド座標を設定する

	////////////////////敵の当たり判定を渡す関数/////////////////////////////////////////////////////////////////////
	DirectX::BoundingSphere GetBodyCollision() const { return *m_bodyCollision.get(); }	// 体の当たり判定を取得する

	////////////////////敵のステートを渡す関数/////////////////////////////////////////////////////////////////////
	EnemyIdling*		GetEnemyIdling		() const { return m_idling		.get();	}	// 待機状態
	Enemy_Attacking*	GetEnemyAttacking	() const { return m_attacking	.get();	}	// 攻撃状態
	Enemy_Sweeping*		GetEnemySweeping	() const { return m_sweeping	.get();	}	// 薙ぎ払い状態
	EnemyDashAttacking* GetEnemyDashAttacking()const { return m_dashAttacking.get();}	// 突撃状態
	EnemyApproaching*	GetEnemyApproaching	() const { return m_approaching	.get();	}	// 追尾状態
	EnemyDead*			GetEnemyDead		() const { return m_dead		.get(); }	// 死亡状態

	////////////////////　顔　/////////////////////////////////////////////////////////////////////////////////////
	void SetFace(IFace* face) { m_currentFace = face; }	// 顔の設定
	EnemyFaceIdling*	GetFaceIdling	() const { return m_faceIdling		.get();	}	// 待機顔
	EnemyFaceAttacking* GetFaceAttacking() const { return m_faceAttacking	.get();	}	// 攻撃顔

	void SetTargetLockOn(bool flag) { m_isTargetLockOn = flag; }	// ロックオンするかどうか

	// 現在のステートを返す
	IState* GetCurrentState() const { return m_currentState; }

public:
	// コンストラクタ
	Enemy(PlayScene* playScene);
	// デストラクタ
	~Enemy();
	// 初期化処理
	void Initialize();
	// 新しい状態に遷移する（ステートパターン）
	void ChangeState(IState* newState);
	// 更新処理
	void Update(float elapsedTime);
	void CalcrationWorldMatrix();
	// 描画処理
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// 終了処理
	void Finalize();
	// 死亡処理を行う
	void DeadAction();

	// ステートの作成処理
	void CreateState();
	// 顔の作成処理
	void CreateFace();

	// 当たり判定の生成処理
	void CreateCollision();
	// 当たったときの処理
	void HitAction(InterSectData data)override;
	// 衝突クールタイムを計測
	void CheckHitCoolTime(float elapsedTime);

	void HitSword(InterSectData data);
	void HitStage(InterSectData data);

	void CheckAlive();

private:
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


	// ==== ステートパターンに使用 =============================================　
	IState* m_currentState;			// 現在のステート（ステートパターン）
	std::unique_ptr<EnemyStarting>		m_starting;		// 開始状態
	std::unique_ptr<EnemyIdling>		m_idling;		// 待機状態
	std::unique_ptr<Enemy_Attacking>	m_attacking;	// 攻撃状態
	std::unique_ptr<Enemy_Sweeping>		m_sweeping;		// 薙ぎ払い状態
	std::unique_ptr<EnemyDashAttacking>	m_dashAttacking;// 突撃状態
	std::unique_ptr<EnemyApproaching>	m_approaching;	// 追尾状態
	std::unique_ptr<EnemyDead>			m_dead;			// 死亡状態


	// ==== 顔パーツ ============================================================
	IFace* m_currentFace;			// 現在の顔
	std::unique_ptr<EnemyFaceIdling>	m_faceIdling;		// 待機顔
	std::unique_ptr<EnemyFaceAttacking>	m_faceAttacking;	// 攻撃顔

	// 敵のダメージエフェクト
	std::unique_ptr<EnemyDamageEffect> m_damageEffect;
	// 敵の死亡エフェクト
	std::unique_ptr<EnemyDeadEffect> m_deadEffect;

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

	// プレイシーン(当たり判定の処理に使用)
	PlayScene* m_playScene;

	// 体の当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
	bool m_isHit;
	float m_coolTime;

	// 衝突可能かどうか
	bool m_canHit;

	// ロックオンするかどうか
	bool m_isTargetLockOn;









	bool debug = false;
};
