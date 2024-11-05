#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"

class Sword;
class Easying;

/// <summary>
/// 桃太郎が持つ刀の攻撃
/// </summary>
class Sword_Attacking_1 : public IWeapon
{
public:
	// 固定値
	static const float RADIAN_90;
	static const float ATTACK_TIME;

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

	void HitAction(InterSectData data)override;

private:
	// 根本と頂点の座標を取得する ※ both ends = 両端
	void GetCudgelBothEnds(float _totalTime);

	void CreateSwordParticle();


	// ソードの元を取得
	Sword* m_sword;

	DirectX::SimpleMath::Vector3	m_position;		// 座標
	DirectX::SimpleMath::Vector3	m_velocity;		// 速度
	float							m_angle;		// 剣の回転
	DirectX::SimpleMath::Vector3	m_rot;			// 3方向の剣の回転
	DirectX::SimpleMath::Matrix		m_worldMatrix;	// ワールド行列
	DirectX::Model*					m_model;		// モデルのポインタ

	float m_totalSeconds;

	std::vector<DirectX::SimpleMath::Vector3>	m_rootPos;	// 根本の座標
	std::vector<DirectX::SimpleMath::Vector3>	m_tipPos;	// 先端の座標

};