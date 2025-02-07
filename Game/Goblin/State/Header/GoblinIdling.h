// --------------------------------------------------
//名前:	GoblinIdling.h
//内容:	ゴブリンの待機状態
//作成:	池田桜輔
// --------------------------------------------------
#pragma once
#ifndef GOBLIN_IDLING
#define GOBLIN_IDLING
// インクルード
#include "pch.h"
#include "Interface/IState.h"

// 前方宣言
class Goblin;

/// <summary>
///	ゴブリンの待機状態
/// </summary>
class GoblinIdling : public IState
{
	// -----------------------
	// 固定値
	// -----------------------
public:
	// 最小クールタイム
	static constexpr float MIN_COOLTIME = 1.0f;
	// 最大クールタイム
	static constexpr float MAX_COOLTIME = 4.0f;

	// -----------------------
	// メンバ関数(公開)
	// -----------------------
public:
	// コンストラクタ
	GoblinIdling(Goblin* goblin);
	// デストラクタ
	~GoblinIdling() override;
	// 初期化する
	void Initialize() override;
	// 事前更新する
	void PreUpdate() override;
	// 更新する
	void Update(const float& elapsedTime) override;
	// 事後更新する
	void PostUpdate() override;
	// 後処理を行う
	void Finalize() override;
	// プレイヤーを探す
	void SearchPlayer();

	// -----------------------
	// メンバ変数
	// -----------------------
private:
	// 親
	Goblin* m_goblin;
	// 経過時間
	float m_TotalTime;
	// クールタイム
	float m_coolTime;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 角度
	float m_angle;
	// 回転行列
	DirectX::SimpleMath::Matrix m_rotMatrix;
};

#endif