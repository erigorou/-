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

	static const DirectX::SimpleMath::Vector3 ARM_LENGTH;		// 架空の腕の長さ
	static const DirectX::SimpleMath::Vector3 ZERO_DIREC;		// 原点から移動する地点


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
	
	// Cudgelの回転を計算する関数
	void UpdateCudgelRotation();
	// 初期値として使用するワールド行列を計算する関数
	void CalculateModelMatrix();
	// Cudgelの攻撃モーションの回転を計算する関数
	DirectX::SimpleMath::Matrix CalculateAttackMatrix();
	// 根本と頂点の座標を取得する ※ both ends = 両端
	void GetCudgelBothEnds(float _totalTime);


private:
	DirectX::SimpleMath::Vector3 m_position;	// 座標
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	float m_angleRL;							// 左右角度
	float m_angleUD;							// 上下角度

	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド行列
	DirectX::Model* m_model;					// モデルのポインタ
	float m_totalSeconds;						// ステートの経過時間
	float m_recordPointTimer;					// 座標を記録するインターバルの計測用変数 

	DirectX::BoundingOrientedBox m_boundingBox;	// 金棒の当たり判定
	DirectX::BoundingOrientedBox m_originalBox;	// 金棒の大元となる当たり判定（初期値等が記録されている）

	Cudgel* m_cudgel;							// ステートを所有する親

	// 剣の軌跡のエフェクト
	// 頂点を保存する用の双極配列
	std::list<DirectX::SimpleMath::Vector3> m_rootPos;	// 根本
	std::list<DirectX::SimpleMath::Vector3> m_tipPos;	// 先端




	// 根本の位置と先端の位置がちゃんと保存できているのかが問題
	// この問題を解決するためにBoundingSphereを生成して、二つの位置に置く。
	DirectX::SimpleMath::Vector3 m_rootDeb;
	DirectX::SimpleMath::Vector3 m_tipDeb;

	std::vector<DirectX::GeometricPrimitive> m_sphere;
};