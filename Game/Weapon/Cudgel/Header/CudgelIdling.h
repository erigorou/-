// ----------------------------------------------------------
// 名前:	CudgelIdling
// 
// 内容:	ボス鬼の武器の金棒　待機攻撃のステート
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

/// <summary>
/// 待機状態
/// </summary>
class CudgelIdling : public IWeapon
{
	// --------------------------
	// メンバ関数(公開)
	// ---------------------------
public:
	// コンストラクタ
	CudgelIdling(Cudgel* cudgel);
	// デストラクタ
	~CudgelIdling()override;
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
	// 衝突イベント
	void HitAction(InterSectData data)override;

	// ---------------------------
	// メンバ変数
	// ---------------------------
private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 角度
	float m_angle;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 金棒の元
	Cudgel* m_cudgel;
};