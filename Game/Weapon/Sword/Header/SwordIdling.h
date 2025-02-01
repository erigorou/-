#pragma once
#include "pch.h"
#include "Interface/IWeapon.h"

class Sword;

/// <summary>
/// 桃太郎が持つ刀の待機状態
/// </summary>
class SwordIdling : public IWeapon
{
// -----------------------
// 固定値
// -----------------------
public:
	static constexpr float INIT_Y_ROT = DirectX::XMConvertToRadians(-90.0f);
	static constexpr float INIT_Z_ROT = DirectX::XMConvertToRadians(90.0f);


// -----------------------
// メンバ関数(公開)
// -----------------------
public:
	// コンストラクタ
	SwordIdling(Sword* sword);
	// デストラクタ
	~SwordIdling();
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
	// 衝突処理
	void HitAction(InterSectData data)override;


// -------------------------
// メンバ関数(非公開)
// -------------------------
private:
	// ワールド行列の更新処理
	void UpdateWorldMatrix();


// -----------------------
// メンバ変数
// -----------------------
private:
	// ソードの元を取得
	Sword* m_sword;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
};