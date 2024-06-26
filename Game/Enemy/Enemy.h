#pragma once	// 多重読み込み防止
#include "Interface/IState.h"
#include "Game/Enemy/States/EnemyIdling.h"	// 待機ステート
#include "BehaviourTree/Header/BehaviorTree.h"	// ビヘイビアツリー


class Enemy
{
	// 固定値
	static const float ENEMY_SPEED;

public:

public:
	// 鬼の座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 鬼の回転角を取得する
	float GetAngle() const { return m_angle; }
	// 敵のアイドリングを取得する
	EnemyIdling* GetEnemyIdling() const { return m_enemyIdling.get(); }


public:
	// コンストラクタ
	Enemy();
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
	void DrawBoundingSphere(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
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

	// 現在のステート（ステートパターン）
	IState* m_currentState;

	// ビヘイビアツリー
	std::unique_ptr<BehaviorTree> m_pBT;

	// バウンディングスフィア
	DirectX::BoundingSphere m_boundingSphereBody;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
private:
	// 待機ステート
	std::unique_ptr<EnemyIdling> m_enemyIdling;
};
