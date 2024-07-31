#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"
#include "Interface/IWeapon.h"

class Sword;

/// <summary>
/// 桃太郎が持つ刀の待機状態
/// </summary>
class Sword_Idling : public IWeapon
{
public:
	// 固定値
	static const float ROTX;
	static const float ROTY;


	// 武器の当たり判定を取得する関数
	DirectX::BoundingOrientedBox GetBoundingBox(){ return m_boundingBox; }

	// コンストラクタ
	Sword_Idling(Sword* sword);
	// デストラクタ
	~Sword_Idling();

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
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	DirectX::Model* m_model;

	// 剣の当たり判定
	DirectX::BoundingOrientedBox m_boundingBox;
	// オリジナルの当たり判定 (オリジナルは生成をするだけのもの)
	DirectX::BoundingOrientedBox m_originalBox;

private:
	// ソードの元を取得
	Sword* m_sword;
};