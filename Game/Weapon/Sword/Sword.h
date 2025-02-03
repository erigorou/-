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
	// ------------------------------------
	// 固定値
	// ------------------------------------
public:
	// 刀の大きさ
	static const float SWORD_SCALE;
	// 刀の位置
	static constexpr DirectX::SimpleMath::Vector3 SWORD_DIR_FOR_PLAYER = { -1.5f, 1.0f, -2.0f };
	// モデルの先端の高さ
	static constexpr float MODEL_TOP_HEIGHT = 55.0f;
	// モデルの根本の高さ
	static constexpr float MODEL_ROOT_HEIGHT = 20.0f;

	// ------------------------------------
	// アクセサ
	// ------------------------------------
		// プレイヤーの取得
	Player* GetPlayer() { return m_player; }
	// ワールド行列の設定
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }
	// 攻撃可能かどうかの設定
	void SetAttackFlag(bool flag) { m_canAttack = flag; }
	// 攻撃可能かどうか
	bool GetAttackFlag() { return m_canAttack; }
	// 位置のゲッター
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }
	// 当たり判定の位置の設定
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }

	// ------------------------------------
	// メンバ関数(公開)
	// ------------------------------------
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

	// ------------------------------------
	// メンバ関数(非公開)
	// ------------------------------------
private:
	// ステートを生成
	void CreateState();
	void CreateCollision();

	// ------------------------------------
	// メンバ変数
	// ------------------------------------
private:
	// プレイヤー
	Player* m_player;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	DirectX::Model* m_model;
	// 剣の当たり判定１（実際の当たり判定）
	std::unique_ptr<DirectX::BoundingOrientedBox> m_collision;
	// オリジナルの当たり判定（オリジナルは生成をするだけのもの）
	DirectX::BoundingOrientedBox m_originalBox;
	// 攻撃できるかどうか
	bool m_canAttack;

	// 現在のステート
	IWeapon* m_currentState;
	// 待機状態
	std::unique_ptr<SwordIdling> m_idling;
	// 攻撃状態
	std::unique_ptr<SwordAttacking1> m_attacking1;
	// 攻撃状態
	std::unique_ptr<SwordAttacking2> m_attacking2;
	// 待機モーションを格納する配列
	std::vector<IWeapon*> m_states;
};