// プレイヤー（桃太郎）が持つ刀

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"
#include "Interface/IObject.h"

// 剣の状態 ===============================================================
#include "Game/Weapon/Sword/Header/Sword_Idling.h"			// 待機状態
#include "Game/Weapon/Sword/Header/Sword_Attacking_1.h"		// 攻撃状態１
#include "Game/Weapon/Sword/Header/Sword_Attacking_2.h"		// 攻撃状態２
#include "Game/Weapon/Sword/Header/Sword_Attacking_3.h"		// 攻撃状態３
#include "Game/Weapon/Sword/Header/Sword_Attacking_4.h"		// 攻撃状態４



class Sword : public IObject
{
// 固定値**
public:
	static const float SWORD_SCALE;

	static constexpr DirectX::SimpleMath::Vector3 SWORD_DIR_FOR_PLAYER = { -1.5f, 1.0f, -2.0f };	// プレイヤーの刀の位置

	static constexpr float MODEL_TOP_HEIGHT  = 55.0f;
	static constexpr float MODEL_ROOT_HEIGHT = 50.0f;

// 公開関数**
	PlayScene*	GetPlayScene()		const { return m_playScene;		}	// プレイシーンのゲッター
	DirectX::BoundingOrientedBox GetCollision() const { return *m_collision.get(); }	// 当たり判定の取得

	// 状態のゲッター
	IWeapon* GetIdlingState()		const { return m_swordIdling.get();		}	// 待機状態
	IWeapon* GetAttacking_1State()	const { return m_swordAttacking_1.get();}	// 攻撃状態１
	IWeapon* GetAttacking_2State()	const { return m_swordAttacking_2.get();}	// 攻撃状態２
	IWeapon* GetAttacking_3State()	const { return m_swordAttacking_3.get();}	// 攻撃状態３
	IWeapon* GetAttacking_4State()	const { return m_swordAttacking_4.get();}	// 攻撃状態４
	IWeapon* GetCurrentState()		const { return m_currentState;			}	// 現在のステートの取得

	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }	// ワールド行列の設定

	void SetAttackFlag	(bool flag) { m_canAttack = flag; }	// 攻撃可能かどうかの設定
	bool GetAttackFlag	()			{ return m_canAttack; }	// 攻撃可能かどうか

	// 位置のゲッター
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }

	// 当たり判定の位置の設定
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }

	// コンストラクタ
	Sword(PlayScene* playScene);
	// デストラクタ
	~Sword();
	// 初期化
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
	// 終了処理
	void Finalize();

	// ステートを更新する
	void ChangeState(IWeapon* state);
	// 当たったときの処理
	void HitAction(InterSectData data)  override;

// 内部関数**
private:
	// ステートを生成
	void CreateState();
	void CreateCollision();


	DirectX::SimpleMath::Vector3 m_position;	// 位置
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	DirectX::SimpleMath::Vector3 m_angle;		// 角度
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド行列
	DirectX::Model*				m_model;		// モデル

	// 剣の当たり判定１（実際の当たり判定）
	std::unique_ptr<DirectX::BoundingOrientedBox> m_collision;
	// オリジナルの当たり判定（オリジナルは生成をするだけのもの）
	DirectX::BoundingOrientedBox m_originalBox;
	// 攻撃できるかどうか
	bool m_canAttack;

	// 現在のステート
	IWeapon* m_currentState;
	// 待機モーション
	std::unique_ptr<Sword_Idling> m_swordIdling;
	std::unique_ptr<Sword_Attacking_1> m_swordAttacking_1;
	std::unique_ptr<Sword_Attacking_2> m_swordAttacking_2;
	std::unique_ptr<Sword_Attacking_3> m_swordAttacking_3;
	std::unique_ptr<Sword_Attacking_4> m_swordAttacking_4;


	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プレイシーン（当たり判定の処理に使用）
	PlayScene* m_playScene;

};