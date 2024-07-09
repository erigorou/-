#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"

class Sword;

/// <summary>
/// 桃太郎が持つ刀の攻撃
/// </summary>
class Sword_Attacking_1 : public IWeapon
{
public:
	// 固定値
	static const float RADIAN_90;
	static const float ATTACK_TIME;


	// 境界ボックスの当たり判定を渡す
	DirectX::BoundingBox GetBoundingBox(){ return m_boundingBox; }


	// コンストラクタ
	Sword_Attacking_1(Sword* sword);
	// デストラクタ
	~Sword_Attacking_1();

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
	float m_angle;
	// 剣の回転(x軸, y軸, z軸)
	DirectX::SimpleMath::Vector3 m_rot;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	DirectX::Model* m_model;

	// このシーンになってからのステート
	float m_totalSeconds;

	// 剣の当たり判定1(実際の当たり判定)　
	DirectX::BoundingBox m_boundingBox;
	// オリジナルの当たり判定 (オリジナルは生成をするだけのもの)
	DirectX::BoundingBox m_originalBox;


private:
	// ソードの元を取得
	Sword* m_sword;
};