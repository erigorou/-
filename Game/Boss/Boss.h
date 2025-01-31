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
#include "States/Header/BossStarting.h"		// 開始状態
#include "States/Header/BossIdling.h"		// 待機状態
#include "States/Header/BossAttacking.h"	// たたきつけ攻撃
#include "States/Header/BossSweeping.h"		// 薙ぎ払い攻撃
#include "States/Header/BossDashAttacking.h"// 突撃状態
#include "States/Header/BossApproaching.h"	// 追尾状態
#include "States/Header/BossDead.h"			// 死亡状態


class Boss : public IEnemy
{
public:
	// 固定値
	static constexpr float BOSS_SPEED = 0.1f;
	static constexpr float BOSS_SCALE= 0.6f;
	static constexpr float COOL_TIME = 0.3f;

	static constexpr float COLISION_POS_Y	= 10.0f;
	static constexpr float HP				= 20.0f;
	static constexpr float COLLISION_RADIUS = 20.0f;
	static constexpr float Boss_HEIGHT		= 50.0f;


public:
	// 刀のダメージを受ける許可を出す
	void CanHitSword() { m_canHit = true; }
	// 刀のダメージを受ける許可を取り消す
	void CanNotHitSword() { m_canHit = false; }


// --------------------------------
//  アクセサ関数
// --------------------------------
public:
	// 取得
	HPSystem*						GetBossHP		()	const	{ return m_hp.get();	}	// HP
	DirectX::SimpleMath::Vector3	GetPosition		()	override{ return m_position;	}	// 鬼の座標
	float							GetAngle		()	const	{ return m_angle;		}	// 鬼の回転角
	float							GetBodyTilt		()	const	{ return m_bodyTilt;	}	// 体の傾き
	DirectX::SimpleMath::Matrix		GetWorldMatrix	()	const	{ return m_worldMatrix; }	// 敵のワールド座標
	HPSystem*						GetHPSystem		()	override{ return m_hp.get();	}	// HP
	DirectX::BoundingSphere			GetBodyCollision()	const	{ return *m_bodyCollision.get(); }	// 体の当たり判定

	// 状態設定
	IState*			GetCurrentState			() const { return m_currentState;		}	// 現在
	BossIdling*		GetBossIdling			() const { return m_idling.get();		}	// 待機
	BossAttacking*	GetBossAttacking		() const { return m_attacking.get();	}	// 攻撃
	BossSweeping*		GetBossSweeping		() const { return m_sweeping.get();		}	// 薙ぎ払い
	BossDashAttacking* GetBossDashAttacking	() const { return m_dashAttacking.get();}	// 突撃
	BossApproaching*	GetBossApproaching		() const { return m_approaching.get();	}	// 追尾
	BossDead*			GetBossDead			() const { return m_dead.get();			}	// 死亡

	// 設定
	void SetPosition	(const DirectX::SimpleMath::Vector3 pos)	{ m_position = pos;		}	// 鬼の座標
	void SetAngle		(const float angle)							{ m_angle = angle;		}	// 鬼の回転角
	void SetBodyTilt	(const float tilt)							{ m_bodyTilt = tilt;	}	// 体の傾き
	void SetWorldMatrix	(DirectX::SimpleMath::Matrix mat)			{ m_worldMatrix = mat;	}	// 敵のワールド座標

	// 顔のパーツ
	void SetFace(IFace* face) { m_currentFace = face; }	// 顔の設定
	BossFaceIdling*	GetFaceIdling	() const { return m_faceIdling		.get();	}	// 待機顔
	BossFaceAttacking* GetFaceAttacking() const { return m_faceAttacking	.get();	}	// 攻撃顔


public:
	// コンストラクタ
	Boss();
	// デストラクタ
	~Boss();
	// 初期化処理
	void Initialize();
	// 新しい状態に遷移する（ステートパターン）
	void ChangeState(IState* newState);
	// 更新処理
	void Update(float elapsedTime);
	// ワールド行列の計算
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
	// 衝突可能
	void CanHit(bool flag) override { m_canHit = flag; }
	// 当たったときの処理
	void HitAction(InterSectData data)override;
	// 衝突クールタイムを計測
	void CheckHitCoolTime(float elapsedTime);
	// 刀の衝突判定
	void HitSword(InterSectData data);
	// ステージの衝突判定
	void HitStage(InterSectData data);
	// 死亡処理
	void CheckAlive();

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

	IState* m_currentState;			// 現在のステート（ステートパターン）
	std::unique_ptr<BossStarting>		m_starting;		// 開始状態
	std::unique_ptr<BossIdling>		m_idling;		// 待機状態
	std::unique_ptr<BossAttacking>	m_attacking;	// 攻撃状態
	std::unique_ptr<BossSweeping>		m_sweeping;		// 薙ぎ払い状態
	std::unique_ptr<BossDashAttacking>	m_dashAttacking;// 突撃状態
	std::unique_ptr<BossApproaching>	m_approaching;	// 追尾状態
	std::unique_ptr<BossDead>			m_dead;			// 死亡状態

	IFace* m_currentFace;			// 現在の顔
	std::unique_ptr<BossFaceIdling>	m_faceIdling;		// 待機顔
	std::unique_ptr<BossFaceAttacking>	m_faceAttacking;	// 攻撃顔
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
