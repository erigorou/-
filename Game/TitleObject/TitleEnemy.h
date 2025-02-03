#pragma once	// 多重読み込み防止
#include "pch.h"
#include "Interface/IState.h"
#include "Game/Scene/TitleScene.h"

// ===== 敵の状態 =================================================================
class EnemyTitleMoving;
class EnemyTitleIdling;

class TitleEnemy
{
public:
	// 固定値
	static const float TITLE_ENEMY_SPEED;
	static const float TITLE_ENEMY_SCALE;

	static const float COOL_TIME;

public:

	// /////////////////敵の基礎情報を渡す関数/////////////////////////////////////////////////////////////////////
	DirectX::SimpleMath::Vector3	GetPosition()	const { return m_position; }		// 鬼の座標を取得する
	float							GetAngle()		const { return m_angle; }			// 鬼の回転角を取得する
	DirectX::SimpleMath::Matrix		GetWorldMatrix()const { return m_worldMatrix; }	// 敵のワールド座標を取得する

	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }		// 鬼の座標を設定する
	void SetAngle(const float angle) { m_angle = angle; }		// 鬼の回転角を設定する
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }	// 敵のワールド座標を設定する

	// /////////////////敵のステートを渡す関数/////////////////////////////////////////////////////////////////////
	EnemyTitleMoving* GetTitleEnemyTitleMoving() const { return m_titleMoving.get(); }	// タイトル画面の敵の移動状態
	EnemyTitleIdling* GetTitleEnemyIdling() const { return m_titleIdling.get(); }	// タイトル画面の敵の待機状態

	// 現在のステートを返す
	IState* GetCurrentState() const { return m_currentState; }

public:
	// コンストラクタ
	TitleEnemy();
	// デストラクタ
	~TitleEnemy();
	// 初期化処理
	void Initialize();
	// 新しい状態に遷移する（ステートパターン）
	void ChangeState(IState* newState);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// 終了処理
	void Finalize();

private:
	// ステートの作成処理
	void CreateState();

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

	// ==== ステートパターンに使用 =============================================　
	IState* m_currentState;			// 現在のステート（ステートパターン）
	std::unique_ptr<EnemyTitleMoving> m_titleMoving;		// タイトル画面の敵の移動状態
	std::unique_ptr<EnemyTitleIdling> m_titleIdling;		// タイトル画面の敵の待機状態

private:
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
