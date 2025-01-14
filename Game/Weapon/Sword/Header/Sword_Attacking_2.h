#pragma once
#include "pch.h"
#include "Game/Weapon/Sword/Sword.h"

class Sword;
class Easying;
class Particle;

/// <summary>
/// 桃太郎が持つ刀の攻撃
/// </summary>
class Sword_Attacking_2 : public IWeapon
{
// 固定値 **
public:
	// 90度のラジアン
	static const float RADIAN_90;
	// 攻撃の時間
	static constexpr float ATTACK_TIME = 0.5f;
	// 位置調整用
	static constexpr DirectX::SimpleMath::Vector3 MATRIX_DIRECTION = DirectX::SimpleMath::Vector3(1.0f, 2.0f, 0.0f);

	// 横の貯め角度
	static constexpr float MAX_SIDE_ANGLE = 250.0f;
	// 初期角度
	static constexpr float INIT_ANGLE = 10.0f;
	// 縦の最大
	static constexpr float MAX_VERTICAL_ANGLE = 30.0f;

// 公開関数 **
public:
	// コンストラクタ
	Sword_Attacking_2(Sword* sword);
	// デストラクタ
	~Sword_Attacking_2();

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
	// 衝突処理
	void HitAction(InterSectData data)override;


// ---------------------
// 非公開関数
// ---------------------
private:
	// アニメーション更新処理
	void UpdateAnimation();
	// ワールド行列の更新
	void UpdateWorldMatrix();
	// 根本と頂点の座標を取得する ※ both ends = 両端
	void GetCudgelBothEnds();

	// ソードのパーティクルを生成
	void CreateSwordParticle();


	// ソードの元を取得
	Sword* m_sword;

	DirectX::SimpleMath::Vector3	m_position;		// 座標
	DirectX::SimpleMath::Vector3	m_velocity;		// 速度
	float							m_angle;		// 剣の回転
	DirectX::SimpleMath::Vector3	m_rot;			// 3方向の剣の回転
	DirectX::SimpleMath::Matrix		m_worldMatrix;	// ワールド行列
	DirectX::Model* m_model;		// モデルのポインタ
	Particle* m_particles;							// パーティクル
	float m_totalSeconds;

	std::vector<DirectX::SimpleMath::Vector3>	m_rootPos;	// 根本の座標
	std::vector<DirectX::SimpleMath::Vector3>	m_tipPos;	// 先端の座標

};