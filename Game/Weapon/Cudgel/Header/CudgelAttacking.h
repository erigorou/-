// ----------------------------------------------------------
// 名前:	CudgelAttacking
// 
// 内容:	ボス鬼の武器の金棒　たたきつけ攻撃のステート
// 			ステートマシーンを使用
//
// 制作:	池田桜輔
// ----------------------------------------------------------

#pragma once
// インクルード
#include "pch.h"
#include "Interface/IWeapon.h"

// 前方宣言
class Cudgel;
class Enemy;
class Particle;
class Easying;

/// <summary>
/// 攻撃状態
/// </summary>
class CudgelAttacking : public IWeapon
{
	// --------------------------
	// 固定値
	// ---------------------------
public:
	// 攻撃の貯めモーションの時間
	static constexpr float CHARGE_TIME = 0.8f;
	// 攻撃する前の時間
	static constexpr float WINDUP_TIME = 0.9f;
	// 攻撃する時間
	static constexpr float ATTACK_TIME = 1.5f;
	// 攻撃が終わってから待機する時間
	static constexpr float STOP_TIME = 2.3f;
	// 攻撃が終わってから元に戻る時間
	static constexpr float RETURN_TIME = 2.8f;
	// 攻撃（全体）が終了する時間
	static constexpr float END_TIME = 3.3f;
	// 腕の長さ
	static constexpr DirectX::SimpleMath::Vector3 ARM_LENGTH = { 0.0f, 4.0f, 0.0f };
	// 原点から移動する地点
	static constexpr DirectX::SimpleMath::Vector3 ZERO_DIREC = { 8.0f, 1.0f, 0.0f };
	// ため時の角度
	static constexpr float CHARGE_ANGLE = 40.0f;
	// 終了時の角度
	static constexpr float END_ANGLE = 135.0f;
	// 初期の角度
	static constexpr float INIT_ANGLE = 20.0f;
	// 攻撃の角度
	static constexpr float ATTACK_ANGLE = 95.0f;

	// SEを再生する時間
	static constexpr float SOUND_TIME = 0.9f;

	// ---------------------------
	// メンバ関数(公開)
	// ---------------------------
public:
	// コンストラクタ
	CudgelAttacking(Cudgel* cudgel);
	// デストラクタ
	~CudgelAttacking()override;
	// 初期化処理
	void Initialize()override;
	// 事前処理
	void PreUpdate()override;
	// 更新処理
	void Update(float elapsedTime)override;
	void UpdateAnimation();
	// 事後処理
	void PostUpdate()override;
	// 終了処理
	void Finalize()override;
	// 衝突イベント
	void HitAction(InterSectData data)override;

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// Cudgelの回転を計算する関数
	void UpdateCudgelRotation();
	// 初期値として使用するワールド行列を計算する関数
	void CalculateModelMatrix();
	// Cudgelの攻撃モーションの回転を計算する関数
	DirectX::SimpleMath::Matrix CalculateAttackMatrix();
	// 根本と頂点の座標を取得する ※ both ends = 両端
	void GetCudgelBothEnds(float _totalTime);
	// ためモーション
	void HandleChargePhase(float t);
	// 攻撃前のモーション
	void HandleWindoupPhase();
	// 攻撃モーション
	void HandleAttackPhase(float t);
	// 攻撃後のモーション
	void KeepStampPhase();
	// 元に戻るモーション
	void ReturnToOriginalPhase(float t);
	// パーティクルを生成する
	void HandleSlamParticles();
	// 攻撃状態を更新する
	void UpdateAttackState();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 攻撃フラグ
	bool m_canHit;
	// 敵の角度
	float m_enemyAngle;
	// 左右角度
	float m_angleRL;
	// 上下角度
	float m_angleUD;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ステートの経過時間
	float m_totalSeconds;
	// 座標を記録するインターバルの計測用変数
	float m_recordPointTimer;
	// ステートを所有する親
	Cudgel* m_cudgel;

	// パーティクル
	Particle* m_particles;
	// 根本の座標配列
	std::vector<DirectX::SimpleMath::Vector3> m_rootPos;
	// 先端の座標配列
	std::vector<DirectX::SimpleMath::Vector3> m_tipPos;
	// パーティクルを生成したかのフラグ
	bool m_canGenerateSlamParticles;
	// 効果音を再生したかのフラグ
	bool m_playSound;
};