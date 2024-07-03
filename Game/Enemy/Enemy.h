#pragma once	// 多重読み込み防止
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

#include "Game/Enemy/States/Header/EnemyIdling.h"		// 待機ステート
#include "Game/Enemy/States/Header/EnemyApproaching.h"	// 追尾ステート
#include "BehaviourTree/Header/BehaviorTree.h"	// ビヘイビアツリー


class Enemy
{
public:
	// 固定値
	static const float ENEMY_SPEED;
	static const float ENEMY_SCALE;


public:
	// プレイシーンを取得する
	PlayScene* GetPlayScene() const { return m_playScene; }
	// 鬼の座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 鬼の座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// 鬼の回転角を取得する
	float GetAngle() const { return m_angle; }
	// 鬼の回転角を設定する
	void SetAngle(const float angle){ m_angle = angle; }
	// 敵のワールド座標を取得する
	DirectX::SimpleMath::Matrix	GetWorldMatrix() const { return m_worldMatrix; }
	// 敵のワールド座標を設定する
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }


	// 敵のアイドリングを取得する
	EnemyIdling* GetEnemyIdling() const { return m_enemyIdling.get(); }
	// 敵の追尾状態を取得する
	EnemyApproaching* GetEnemyApproaching() const { return m_enemyApproaching.get(); }
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

	// 現在のステート（ステートパターン）
	IState* m_currentState;
	// 待機ステート
	std::unique_ptr<EnemyIdling> m_enemyIdling;
	// 追尾ステート
	std::unique_ptr<EnemyApproaching> m_enemyApproaching;

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
