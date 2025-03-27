#pragma once  // 多重読み込み防止
#include "pch.h"
#include "Interface/IState.h"

// ===== 敵の状態 =================================================================
class EnemyTitleMoving;
class EnemyTitleIdling;

class TitleEnemy
{
	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// 敵の速度
	static constexpr float TITLE_ENEMY_SPEED = 0.1f;
	// 敵のスケール
	static constexpr float TITLE_ENEMY_SCALE = 1.0f;
	// クールタイム
	static constexpr float COOL_TIME = 0.4f;
	// ステートの最大数
	static constexpr int STATE_MAX = 2;


	// -----------------------------
	// 列挙型
	// -----------------------------
	enum class BossState : int
	{
		IDLING,  // 待機状態
		MOVING,  // 移動状態
	};

public:
	// -----------------------------
	// アクセサ
	// -----------------------------
	// 鬼の座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// 鬼の座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	// 鬼の回転角を取得する
	float GetAngle() const { return m_angle; }
	// 鬼の回転角を設定する
	void SetAngle(const float angle) { m_angle = angle; }
	// 敵のワールド座標を取得する
	DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMatrix; }
	// 敵のワールド座標を設定する
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }
	
	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// コンストラクタ
	TitleEnemy();
	// デストラクタ
	~TitleEnemy();
	// 初期化処理
	void Initialize();
	// 新しい状態に遷移（ステートパターン）
	void ChangeState(BossState state);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// 終了処理
	void Finalize();

private:
	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// ステートの作成処理
	void CreateState();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転角
	float m_angle;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	DirectX::Model* m_model;
	// 現在のステート
	IState* m_currentState;
	// 待機状態
	std::unique_ptr<EnemyTitleIdling> m_titleIdling;
	// 移動状態
	std::unique_ptr<EnemyTitleMoving> m_titleMoving;
	// ステートの配列
	IState* m_states[STATE_MAX];
};
