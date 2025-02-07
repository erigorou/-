// --------------------------------------------------
//名前:	GoblinTutorial.h
//内容:	チュートリアル中のゴブリンステート
//作成:	池田桜輔
// --------------------------------------------------
#pragma once
#ifndef GOBLIN_TUTORIAL
#define GOBLIN_TUTORIAL
// インクルード
#include "pch.h"
#include "Interface/IState.h"
// 前方宣言
class Goblin;

/// <summary>
/// チュートリアル中のゴブリンステート
/// </summary>
class GoblinTutorial : public IState
{
	// --------------------
	// メンバ関数(公開)
	// --------------------
public:
	// コンストラクタ
	GoblinTutorial(Goblin* goblin);
	// デストラクタ
	~GoblinTutorial() override;
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

	// ----------------------
	// メンバ変数
	// ----------------------
private:
	// 親
	Goblin* m_goblin;
	// 経過時間
	float m_TotalTime;
	// クールタイム
	float m_coolTime;
};
#endif