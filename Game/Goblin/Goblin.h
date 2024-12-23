// -------------------------------------------------------
// 小鬼の親クラス
// -------------------------------------------------------


#pragma once

#ifndef GOBLIN_OBJECT
#define GOBLIN_OBJECT

#include "pch.h"
#include "Interface/IObject.h"
#include "Interface/IState.h"

class Player;
class PlayScene;
class HPSystem;

class Goblin : public IObject
{
public:
	class GoblinIdling;
	class GoblinAttacking;

	// 固定値
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	static constexpr float GOBLIN_HP = 3.0f;
	static constexpr float COLLISION_RADIUS = 16.0f;

	// 譲渡関数
	DirectX::SimpleMath::Vector3	GetPosition()	override{ return m_position;	}			// 鬼の座標を取得する
	DirectX::SimpleMath::Vector3	GetVelocity()	const	{ return m_velocity;	}			// 速度の取得
	DirectX::SimpleMath::Vector3	GetAngle()		const	{ return m_angle;		}			// 回転角の取得
	DirectX::SimpleMath::Matrix		GetWorldMatrix()const	{ return m_worldMatrix; }			// ワールド座標の取得
	DirectX::BoundingSphere 		GetCollision() const	{ return *m_bodyCollision.get(); }	// 体の当たり判定の取得

	// ステートパターン
	GoblinIdling*		GetIdling()		const { return m_idling.get		(); }	// 待機ステートの取得
	GoblinAttacking*	GetAttacking()	const { return m_attacking.get	();	}	// 攻撃ステートの取得
	
	IState* GetCurrentState() const { return m_currentState; }	// 現在のステートの取得

	void SetIsAttacking(bool isAttacking) { m_nowAttacking = isAttacking; }	// 攻撃中かどうかの設定
	bool IsAttacking() const { return m_nowAttacking; }	// 攻撃中かどうか

	PlayScene* GetPlayScen() const { return m_playScene; }	// PlaySceneの取得

	Goblin(PlayScene* playScene);
	~Goblin();

	void Initialize();									// 初期化関数
	void CreateCollision();								// 当たり判定の生成

	void Update(const float elapsedTime);				// 更新処理
	void MoveCollision();								// 当たり判定の移動

	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);	// 描画関数
	
	void Finalize();									// 終了関数
	void HitAction(InterSectData data) override;		// 当たったときの処理

	void ChangeState(IState* state);	// ステートの変更
private:

	void HitPlayer(InterSectData data);	// プレイヤーに当たったときの処理
	void HitGoblin(InterSectData data);	// 小鬼に当たったときの処理
	void HitEnemy(InterSectData data);	// 敵に当たったときの処理
	void HitStage(InterSectData data);	// ステージに当たったときの処理

	void Damaged(float damage);	// ダメージを受けたときの処理


	void CreateState();		// ステートの作成
	
	DirectX::SimpleMath::Vector3 m_position;	// 座標
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	DirectX::SimpleMath::Vector3 m_angle;		// 回転角
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド座標

	std::unique_ptr<DirectX::Model> m_model;	// モデル

	// 押し戻し量
	DirectX::SimpleMath::Vector3 m_oushBackValue;

	// 現在のステート
	IState* m_currentState;										// 現在のステート
	std::unique_ptr<Goblin::GoblinIdling	> m_idling		;	// 待機
	std::unique_ptr<Goblin::GoblinAttacking	> m_attacking	;	// 攻撃
 
	// HP
	std::unique_ptr<HPSystem> m_hp;	// HP

	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;										// エフェクト
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;	// プリミティブバッチ

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;	// 入力レイアウト
	
	// プレイシーン(当たり判定の処理に使用)
	PlayScene* m_playScene;

	// 体の当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;

	bool m_nowAttacking;	// 攻撃中かどうか
};


#endif // !GOBLIN_OBJECT