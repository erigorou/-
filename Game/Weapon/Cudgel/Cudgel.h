// 鬼が持つ金棒

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"
#include "Interface/IObject.h"

// 金棒の状態 ========================================================
#include "header/Cudgel_Idling.h"		// 待機
#include "header/Cudgel_Attacking.h"	// 攻撃
#include "header/Cudgel_Sweeping.h"		// 薙ぎ払い

class Cudgel : public IObject
{
public:
	// 固定値
	static const float CUDGEL_SCALE;							// 大きさ
	static const DirectX::SimpleMath::Vector3 DIRECTION_ENEMY;	// 敵との離れている距離
	static const DirectX::SimpleMath::Vector3 CUDGEL_LENGTH;	// 金棒の長さ
	static const DirectX::SimpleMath::Vector3 CUDGEL_HADLE_POS;	// 金棒の取っ手の位置


	// ===取得系====================================================================
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }	// 位置の取得


	DirectX::Model* GetModel()		const { return m_model.get();	}	// モデルの取得
	PlayScene*		GetPlayScene()	const { return m_playScene;		}	// プレイシーンの取得


	// ===状態の取得================================================================
	Cudgel_Idling*		GetIdling()		const { return m_idling.get();		}	// 待機
	Cudgel_Attacking*	GetAttacking()	const { return m_attacking.get();	}	// 攻撃
	Cudgel_Sweeping*	GetSweeping()	const { return m_sweeping.get();	}	// 薙ぎ払い	

	// ===設定系====================================================================
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }	// 当たり判定の位置の設定

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
		const DirectX::SimpleMath::Matrix& projection
	);

	// 終了処理
	void Finalize();
	// 当たったときの処理
	void HitAction(InterSectData) override;
	// ステートを更新する
	void ChangeState(IWeapon* state);

private:

	// モデルの生成
	void CreateModel(ID3D11Device1* device);
	// ステートを生成
	void CreateState();
	// 当たり判定の生成
	void CreateCollision();



	DirectX::SimpleMath::Vector3 m_position;	// 位置
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	DirectX::SimpleMath::Vector3 m_angle;		// 角度
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド行列
	std::unique_ptr<DirectX::Model> m_model;	// モデル

	// 金棒の当たり判定1(実際の当たり判定)　
	std::unique_ptr<DirectX::BoundingOrientedBox>	m_collision;
	// オリジナルの当たり判定 (オリジナルは生成をするだけのもの)
	DirectX::BoundingOrientedBox m_originalBox;

private:
	// 現在のステート
	IWeapon* m_currentState;

	std::unique_ptr<Cudgel_Idling>		m_idling;		// 待機
	std::unique_ptr<Cudgel_Attacking>	m_attacking;	// たたきつけ
	std::unique_ptr<Cudgel_Sweeping>	m_sweeping;		// 薙ぎ払い

private:
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プレイシーン（当たり判定の処理に使用）
	PlayScene* m_playScene;
};