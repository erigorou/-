// プレイヤー（桃太郎）が持つ刀

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"

// 剣の状態 ========================================================
#include "Game/Weapon/Sword/Header/Sword_Idling.h"					// 待機状態
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"		// 攻撃状態１



class Sword
{
public:
	// 固定値
	static const float SWORD_SCALE;	// 刀の大きさ
	static const DirectX::SimpleMath::Vector3 SWORD_DIR_FOR_PLAYER;	// 刀とプレイヤの離れてる距離

public:
	// モデルを設定する
	DirectX::Model* GetModel()const { return m_model.get(); }
	// プレイシーンの取得
	PlayScene* GetPlayScene()const { return m_playScene; }


	// 状態のゲッター
	IWeapon* GetIdlingState()const { return m_swordIdling.get(); }				// 待機状態
	IWeapon* GetAttacking_1State()const { return m_swordAttacking_1.get(); }	// 攻撃状態１

public:
	// コンストラクタ
	Sword(PlayScene* playScene);
	// デストラクタ
	~Sword();

	// 初期化
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources
	);

	// 境界ボックスの描画
	void DrawBoundingBox(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// 終了処理
	void Finalize();

	// ステートを生成
	void CreateState();
	// ステートを更新する
	void ChangeState(IWeapon* state);

private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 角度
	DirectX::SimpleMath::Vector3 m_angle;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;

private:
	// 現在のステート
	IWeapon* m_currentState;
	// 待機モーション
	std::unique_ptr<Sword_Idling> m_swordIdling;
	std::unique_ptr<Sword_Attacking_1> m_swordAttacking_1;

private:
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プレイシーン（当たり判定の処理に使用）
	PlayScene* m_playScene;

};