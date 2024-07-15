// 鬼が持つ金棒

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"

// 金棒の状態 ========================================================
#include "Cudgel_Idling.h"		// 待機
#include "Cudgel_Attacking.h"	// 攻撃

class Cudgel
{
public:
	// 固定値
	static const float CUDGEL_SCALE;

public:
	// モデルを設定する
	DirectX::Model* GetModel()const { return m_model.get(); }
	// プレイシーンの取得
	PlayScene* GetPlayScene()const { return m_playScene; }


	// ===状態の取得================================================================
	Cudgel_Idling* GetIdling()const { return m_idling.get(); }				// 待機
	Cudgel_Attacking* GetAttacking()const { return m_attacking.get(); }		// 攻撃




public:
	// コンストラクタ
	Cudgel(PlayScene* playScene);
	// デストラクタ
	~Cudgel();

	// 初期化
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// 境界ボックスの描画
	void DrawBoundingBox(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const DirectX::BoundingBox boundingBox
	);

	// 終了処理
	void Finalize();

	// モデルの生成
	void CreateModel(ID3D11Device1* device);
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
	// 待機状態
	std::unique_ptr<Cudgel_Idling> m_idling;
	// 攻撃状態
	std::unique_ptr<Cudgel_Attacking> m_attacking;


private:
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プレイシーン（当たり判定の処理に使用）
	PlayScene* m_playScene;

};