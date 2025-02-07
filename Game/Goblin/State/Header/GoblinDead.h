// --------------------------------------------------
//名前:	GoblinDead.h
//内容:	ゴブリンの死亡状態
//作成:	池田桜輔
// --------------------------------------------------
#pragma once
#ifndef GOBLIN_DEAD
#define GOBLIN_DEAD

// インクルード
#include "pch.h"
#include "Interface/IState.h"

// 前方宣言
class Goblin;

/// <summary>
/// 小鬼の死亡状態
/// </summary>
class GoblinDead : public IState
{
	// --------------------
	// 固定値
	// --------------------
public:
	// 死亡時間
	static constexpr float DEAD_TIME = 2.5f;


	// ---------------------
	// メンバ関数(公開)
	// ---------------------
public:
	// コンストラクタ
	GoblinDead(Goblin* goblin);
	// デストラクタ
	~GoblinDead() override;
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

	// -------------------
	// メンバ関数(非公開)
	// -------------------
private:
	// アニメーションの更新処理
	void UpdateAnimation();

	// -------------------
	// 内部変数
	// -------------------
private:
	// 小鬼
	Goblin* m_goblin;
	// 経過時間
	float m_totalTime;
	// スケール
	float m_scale;
	// 開始地点
	DirectX::SimpleMath::Vector3 m_startPos;
};

#endif