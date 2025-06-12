// ----------------------------------------------------------------------------------
//
// 名前:	ExecutionNode.h
// 説明:    ビヘイビアツリーのノード
//			実行するアクションをまとめたクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#pragma once
#include "Game/Messenger/EventMessenger.h"
#include "Game/Messenger/EventList.h"


/// <summary>
/// ビヘイビアツリーのエクセキューションノード
/// </summary>
class ExecutionNode final
{
	//------------------------
	// アクセサ
	// -----------------------
public:
	// ステート変更（敵）：待機
	void BossEnemyIdleAction();
	// ステート変更（敵）：叩きつけ
	void BossEnemyAttackAction();
	// ステート変更（敵）：薙ぎ払い
	void BossEnemySweepAttackAction();
	// ステート変更（敵）：突進
	void BossEnemyDashAttackAction();
	// ステート変更（敵）：近づく
	void BossEnemyApproachAction();

	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:
	// コンストラクタ
	ExecutionNode();
	// デストラクタ
	~ExecutionNode();
};

