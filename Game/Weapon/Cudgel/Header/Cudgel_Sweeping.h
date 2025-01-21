#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Cudgel;
class Enemy;
class Particle;
class Easying;


/// <summary>
/// 攻撃状態
/// </summary>
class Cudgel_Sweeping : public IWeapon
{
public:
	// 固定値
	static const float CHARGE_TIME;	// 攻撃の貯めモーションの時間
	static const float WINDUP_TIME;	// 攻撃する前の時間
	static const float ATTACK_TIME;	// 攻撃する時間
	static const float END_TIME;	// 攻撃（全体）が終了する時間

	static const float CHARGE_ROTATE_ANGLE;	// 攻撃の貯めモーションの角度
	static const float WINDUP_ROTATE_ANGLE;	// 攻撃後の角度

	static const DirectX::SimpleMath::Vector3 ARM_LENGTH;		// 架空の腕の長さ
	static const DirectX::SimpleMath::Vector3 ZERO_DIREC;		// 原点から移動する地点


	// コンストラクタ
	Cudgel_Sweeping(Cudgel* cudgel);

	// デストラクタ
	~Cudgel_Sweeping()override;
	// 初期化処理
	void Initialize()override;
	// 事前処理
	void PreUpdate()override;
	// 更新処理
	void Update(float elapsedTime)override;
	// 事後処理
	void PostUpdate()override;
	// 終了処理
	void Finalize()override;

	void HitAction(InterSectData data)override;
	
private:
	// Cudgelの回転を計算する関数
	void UpdateCudgelRotation();
	// 初期値として使用するワールド行列を計算する関数
	void CalculateModelMatrix();
	// Cudgelの攻撃モーションの回転を計算する関数
	DirectX::SimpleMath::Matrix CalculateAttackMatrix();
	// 根本と頂点の座標を取得する ※ both ends = 両端
	void GetCudgelBothEnds();

private:
	DirectX::SimpleMath::Vector3 m_position;	// 座標
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	float m_angleRL;							// 左右角度
	float m_angleUD;							// 上下角度
	float m_parentAngleRL;						// 親の左右角度

	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド行列
	DirectX::SimpleMath::Matrix m_collMatrix;	// 当たり判定用の行列
	float m_totalSeconds;						// ステートの経過時間

	Cudgel*		m_cudgel;						// ステートを所有する親
	Particle*	m_particles;					// パーティクル

	// 剣の軌跡のエフェクト
	// 頂点を保存する用の可変長配列
	std::vector<DirectX::SimpleMath::Vector3> m_rootPos;	// 根本
	std::vector<DirectX::SimpleMath::Vector3> m_tipPos;		// 先端

	// パーティクルを生成したかのフラグ
	bool m_canGenerateSlamParticles;

	// 効果音を再生したかのフラグ
	bool m_playSound;
};