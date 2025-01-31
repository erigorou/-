// プレイヤー（桃太郎）が持つ刀

#pragma once
#include "Interface/IWeapon.h"
#include "Game/Scene/PlayScene.h"
#include "Interface/IObject.h"
#include "Game/Weapon/WeaponState.h"

class SwordIdling;
class SwordAttacking1;
class SwordAttacking2;
class Player;

class Sword : public IObject
{
// 固定値**
public:
	static const float SWORD_SCALE;

	static constexpr DirectX::SimpleMath::Vector3 SWORD_DIR_FOR_PLAYER = { -1.5f, 1.0f, -2.0f };	// プレイヤーの刀の位置

	static constexpr float MODEL_TOP_HEIGHT  = 55.0f;
	static constexpr float MODEL_ROOT_HEIGHT = 50.0f;

// 公開関数**
	DirectX::BoundingOrientedBox GetCollision() const { return *m_collision.get(); }	// 当たり判定の取得

	Player* GetPlayer() { return m_player; }	// プレイヤーの取得

	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }	// ワールド行列の設定

	void SetAttackFlag	(bool flag) { m_canAttack = flag; }	// 攻撃可能かどうかの設定
	bool GetAttackFlag	()			{ return m_canAttack; }	// 攻撃可能かどうか

	// 位置のゲッター
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }

	// 当たり判定の位置の設定
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }

	// コンストラクタ
	Sword(Player* player);
	// デストラクタ
	~Sword();
	// 初期化
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);
	// 終了処理
	void Finalize();

	// ステートを更新する
	void ChangeState(void* state);

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

	std::unique_ptr<SwordIdling> m_swordIdling;
	std::unique_ptr<SwordAttacking1> m_swordAttacking_1;
	std::unique_ptr<SwordAttacking2> m_swordAttacking_2;
	// 待機モーションを格納する配列
	std::vector<IWeapon*> m_states;


	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プレイヤー
	Player* m_player;

};