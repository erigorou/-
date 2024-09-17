#pragma once
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

// プレイヤーに付与されるもの ===========================================
#include "Game/Weapon/Sword/Sword.h"	// 武器

// プレイヤーの状態 =====================================================
#include "Game/Player/State/Header/Player_Idling.h"			// 待機状態
#include "Game/Player/State/Header/Player_Dodging.h"		// 回避状態
#include "Game/Player/State/Header/Player_Attacking_1.h"	// 攻撃状態１
#include "Game/Player/State/Header/Player_Attacking_2.h"	// 攻撃状態２
#include "Game/Player/State/Header/Player_Attacking_3.h"	// 攻撃状態３
#include "Game/Player/State/Header/Player_Attacking_4.h"	// 攻撃状態４


class Player
{
public:
	// 初期座標
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	// 速さ
	static const float PLAYER_SPEED;
	// 大きさ
	static const float PLAYER_SCALE;
	// 攻撃が適用される時間
	static const float APPLIED_ATTACK_TIME;

public:
	// プレイヤーの座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// プレイヤーの速度を取得する
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// プレイヤーの回転角を取得する
	float GetAngle() const { return m_angle; }


	// プレイヤーのアイドリングステートを取得
	PlayerIdling* GetPlayerIdlingState() const { return m_playerIdling.get(); }
	// プレイヤーのダッジングステートを取得
	PlayerDodging* GetPlayerDodgingState() const { return m_playerDodging.get(); }

	// プレイヤーのアタッキングステートを取得
	PlayerAttacking_1* GetPlayerAttackingState1() const { return m_playerAttacking_1.get(); }
	PlayerAttacking_2* GetPlayerAttackingState2() const { return m_playerAttacking_2.get(); }
	PlayerAttacking_3* GetPlayerAttackingState3() const { return m_playerAttacking_3.get(); }
	PlayerAttacking_4* GetPlayerAttackingState4() const { return m_playerAttacking_4.get(); }

	// プレイシーンの情報を取得する
	PlayScene* GetPlayScene()const { return m_playScene; }
	// キーボードの情報を取得する(長押しのみ対応)
	DirectX::Keyboard::State GetKeyboardState() const { return m_keyboardState; }
	// キーボードの情報を取得する（単押しのみ対応）
	DirectX::Keyboard::KeyboardStateTracker GetKeyboardTracker() const { return m_tracker; }

public:
	// コンストラクタ
	Player(PlayScene* playScene);
	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize();
	// ステートの作成関数
	void CreateState();
	// 新しい状態に遷移する
	void ChangeState(IState* newState);
	// 時間計測を行う
	void TimeComparison(float& nowTime, const float totalTime, IState* newState, const float elapsedTime);
	// 更新処理
	void Update(
		const DirectX::SimpleMath::Vector3 enemyPos,
		const float elapsedTime
	);
	// 描画処理
	void Render(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources
	);
	// 境界球の描画
	void DrawBoundingSphere(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::BoundingSphere boundingSphere);

	// 終了処理
	void Finalize();
	// 回転角の計算
	void CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos);
	// Matrixの計算
	void CalculationMatrix();
	// 移動の管理
	void MovePlayer();

private:
	// 体力
	int m_HP;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 加速度
	DirectX::SimpleMath::Vector3 m_acceleration;
	// 角度
	float m_angle;


	// プレイヤー用のワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// ステート関連 =================================================
	IState* m_currentState;										// 現在のステート
	std::unique_ptr<PlayerIdling> m_playerIdling;				// 待機状態
	std::unique_ptr<PlayerDodging> m_playerDodging;				// 回避状態

	std::unique_ptr<PlayerAttacking_1> m_playerAttacking_1;		// 攻撃状態１
	std::unique_ptr<PlayerAttacking_2> m_playerAttacking_2;		// 攻撃状態２
	std::unique_ptr<PlayerAttacking_3> m_playerAttacking_3;		// 攻撃状態３
	std::unique_ptr<PlayerAttacking_4> m_playerAttacking_4;		// 攻撃状態４


	// プレイヤーに付与されるもの ============
	std::unique_ptr<Sword> m_sword;


	// シェーダーに使用するもの ==============
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	float m_particleTime;
	float m_elapsedTime;

private:
	// プレイシーン(他のオブジェクトの情報の取得などに使う)
	PlayScene* m_playScene;
	// キーボードの入力
	DirectX::Keyboard::State m_keyboardState;
	DirectX::Keyboard::KeyboardStateTracker m_tracker;
};
