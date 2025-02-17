// ----------------------------------------------------------
// 名前:	Cudgel
// 
// 内容:	ボス鬼の武器の金棒クラス
//			更新処理や描画を行う
// 			ステートマシーンを使用
//
// 制作:	池田桜輔
// ----------------------------------------------------------
#pragma once
// インクルード
#include "Interface/IWeapon.h"
#include "Interface/IObject.h"

// 前方宣言
class Boss;
class CudgelIdling;
class CudgelAttacking;
class CudgelSweeping;

/// <summary>
/// ボス鬼の金棒クラス
/// </summary>
class Cudgel : public IObject
{
	// --------------------------
	// 固定値
	// ---------------------------
public:
	// 大きさ
	static constexpr float CUDGEL_SCALE = Boss::BOSS_SCALE * 1.4f;
	// 敵との離れている距離
	static constexpr DirectX::SimpleMath::Vector3 DIRECTION_ENEMY = { 8.0f, 5.0f, 0.0f };
	// 金棒の長さ
	static constexpr DirectX::SimpleMath::Vector3 CUDGEL_LENGTH = { 0.0f, 50.0f, 0.0f };
	// 金棒の取っ手の位置
	static constexpr DirectX::SimpleMath::Vector3 CUDGEL_HADLE_POS = { 0.0f, 10.0f, 0.0f };

	// ---------------------------
	// アクセサ
	// ---------------------------
public:
	// このオブジェクトのポインタを渡す
	void* GetCudgelObject() { return this; }
	// ボスのポインタを渡す
	Boss* GetBoss() { return m_boss; }
	// ボスが死亡したことを伝える
	void BossDead() { m_isDead = true; }
	// 位置の取得
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }
	// モデルの取得
	DirectX::Model* GetModel() const { return m_model; }
	// 待機
	CudgelIdling* GetIdling() const { return m_idling.get(); }
	// 攻撃
	CudgelAttacking* GetAttacking() const { return m_attacking.get(); }
	// 薙ぎ払い
	CudgelSweeping* GetSweeping() const { return m_sweeping.get(); }
	// 当たり判定の位置の設定
	void SetCollisionPosition(DirectX::SimpleMath::Matrix mat) { m_originalBox.Transform(*m_collision.get(), mat); }
	// ワールド行列の設定
	void SetWorldMatrix(DirectX::SimpleMath::Matrix mat) { m_worldMatrix = mat; }

	// ---------------------------
	// メンバ関数(公開)
	// ---------------------------
public:
	// コンストラクタ
	Cudgel(Boss* boss);
	// デストラクタ
	~Cudgel();
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
	// 当たったときの処理
	void HitAction(InterSectData) override;
	// ステートを変更する
	void ChangeState(void* state);

	// ---------------------------
	// メンバ関数(非公開)
	// ---------------------------
private:
	// ステートを生成
	void CreateState();
	// 当たり判定の生成
	void CreateCollision();

	// ---------------------------
	// メンバ変数
	// ---------------------------
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
	DirectX::Model* m_model;

	// 金棒の当たり判定1(実際の当たり判定)　
	std::unique_ptr<DirectX::BoundingOrientedBox>	m_collision;
	// オリジナルの当たり判定 (オリジナルは生成をするだけのもの)
	DirectX::BoundingOrientedBox m_originalBox;

	// 現在のステート
	IWeapon* m_currentState;
	// 待機
	std::unique_ptr<CudgelIdling> m_idling;
	// たたきつけ
	std::unique_ptr<CudgelAttacking> m_attacking;
	// 薙ぎ払い
	std::unique_ptr<CudgelSweeping>	m_sweeping;
	// ステートのリスト
	std::vector<IWeapon*> m_states;

	// ボス
	Boss* m_boss;

	// ボスが死亡したか
	bool m_isDead;
};