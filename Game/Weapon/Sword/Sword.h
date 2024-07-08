// プレイヤー（桃太郎）が持つ剣

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"

// 待機ステート
#include "Game/Weapon/Sword/Header/SwordIdling.h"


class Sword
{
public:
	// 固定値
	static const float SWORD_SCALE;

public:
	// モデルを設定する
	DirectX::Model* GetModel()const { return m_model.get(); }
	// プレイシーンの取得
	PlayScene* GetPlayScene()const { return m_playScene; }


	// 待機状態を取得する
	IWeapon* GetIdlingState()const { return m_swordIdling.get(); }

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
		const CommonResources* resources);
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
	std::unique_ptr<SwordIdling> m_swordIdling;

private:
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プレイシーン（当たり判定の処理に使用）
	PlayScene* m_playScene;

};