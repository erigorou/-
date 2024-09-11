#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Cudgel;
class Enemy;



/// <summary>
/// 攻撃状態
/// </summary>
class Cudgel_Attacking : public IWeapon
{
public:
	// 固定値
	static const float CHARGE_TIME;	// 攻撃の貯めモーションの時間
	static const float WINDUP_TIME;	// 攻撃する前の時間
	static const float ATTACK_TIME;	// 攻撃する時間
	static const float END_TIME;	// 攻撃（全体）が終了する時間

	static const DirectX::SimpleMath::Vector3 ARM_LENGTH;	// 架空の腕の長さ
	static const DirectX::SimpleMath::Vector3 ZERO_DIREC;	// 原点から移動する地点


	/// <summary>
	/// 金棒の当たり判定を取得する
	/// </summary>
	/// <returns>箱の当たり判定</returns>
	DirectX::BoundingOrientedBox GetBoundingBox()override { return m_boundingBox; }

	// コンストラクタ
	Cudgel_Attacking(Cudgel* cudgel);

	// デストラクタ
	~Cudgel_Attacking()override;
	// 初期化処理
	void Initialize()override;
	// 事前処理
	void PreUpdate()override;
	// 更新処理
	void Update(float elapsedTime)override;
	// 事後処理
	void PostUpdate()override;
	// 描画処理
	void Render(ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;
	// 終了処理
	void Finalize()override;

private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 角度
	float m_angleRL;
	// 上下の角度
	float m_angleUD;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	DirectX::Model* m_model;
	// 合計時間
	float m_totalSeconds;
	// 金棒の当たり判定1(実際の当たり判定)　
	DirectX::BoundingOrientedBox m_boundingBox;

	// オリジナルの当たり判定 (オリジナルは生成をするだけのもの)
	DirectX::BoundingOrientedBox m_originalBox;

private:
	// 金棒の元
	Cudgel* m_cudgel;


	// 剣の軌跡のエフェクト
	DirectX::SimpleMath::Vector3 m_tipPosition;
	DirectX::SimpleMath::Vector3 m_rootPosition;
};