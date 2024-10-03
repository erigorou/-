#pragma once	// 多重読み込み防止
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

#include "Game/Enemy/EnemyHP.h"	// HP

// ===== 敵の状態 =================================================================
#include "States/Header/EnemyIdling.h"		// 待機状態
#include "States/Header/Enemy_Attacking.h"	// たたきつけ攻撃
#include "States/Header/Enemy_Sweeping.h"	// 薙ぎ払い攻撃
#include "States/Header/EnemyApproaching.h"	// 追尾状態


#include "BehaviourTree/Header/BehaviorTree.h"			// ビヘイビアツリー


class Enemy
{
public:
	// 固定値
	static const float ENEMY_SPEED;
	static const float ENEMY_SCALE;


public:
	PlayScene* GetPlayScene()					const { return m_playScene;}		// PlaySceneの取得 
	EnemyHP* GetEnemyHP()						const { return m_hp.get(); }		// HPの取得	
	
	DirectX::SimpleMath::Vector3 GetPosition()	const { return m_position; }		// 鬼の座標を取得する
	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }	// 鬼の座標を設定する
	
	float GetAngle() const { return m_angle; }										// 鬼の回転角を取得する	
	void SetAngle(const float angle) { m_angle = angle; }							// 鬼の回転角を設定する	

	DirectX::SimpleMath::Matrix	GetWorldMatrix() const { return m_worldMatrix; }	// 敵のワールド座標を取得する
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }	// 敵のワールド座標を設定する

	// ===== 敵の状態 =================================================================
	EnemyIdling* GetEnemyIdling() const { return m_idling.get(); }					// 待機状態
	Enemy_Attacking* GetEnemyAttacking() const { return m_attacking.get(); }		// 攻撃状態
	Enemy_Sweeping* GetEnemySweeping() const { return m_sweeping.get(); }			// 薙ぎ払い状態
	EnemyApproaching* GetEnemyApproaching() const { return m_approaching.get(); }	// 追尾状態


	// 現在のステートを返す
	IState* GetCurrentState() const { return m_currentState; }


public:
	// コンストラクタ
	Enemy(PlayScene* playScene);
	// デストラクタ
	~Enemy();
	// 初期化処理
	void Initialize();
	// ステートの作成処理
	void CreateState();
	// 新しい状態に遷移する（ステートパターン）
	void ChangeState(IState* newState);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
	// 境界球の描画処理
	void DrawBoundingSphere(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::BoundingSphere boundingSphere);
	// 終了処理
	void Finalize();


private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 角度
	float m_angle;
	// 敵用のワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;


	// HP
	std::unique_ptr<EnemyHP> m_hp;

	// ==== ステートパターンに使用 =============================================　
	IState* m_currentState;			// 現在のステート（ステートパターン）
	std::unique_ptr<EnemyIdling> m_idling;					// 待機状態
	std::unique_ptr<Enemy_Attacking> m_attacking;			// 攻撃状態
	std::unique_ptr<Enemy_Sweeping> m_sweeping;				// 薙ぎ払い状態
	std::unique_ptr<EnemyApproaching> m_approaching;		// 追尾状態

	// ビヘイビアツリー
	std::unique_ptr<BehaviorTree> m_pBT;

private:
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プレイシーン(当たり判定の処理に使用)
	PlayScene* m_playScene;
};
