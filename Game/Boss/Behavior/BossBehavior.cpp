// ----------------------------------------------------------------------------------
//
// 名前:	BossBehavior.h
// 説明:    ボスのビヘイビアツリーを管理する
//			BehaviorTreeのルートノードとなるクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "../Boss.h"
#include "BossBehavior.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="boss"></param>
BossBehavior::BossBehavior(Boss* boss)
	: 
	m_boss(boss)
{
}


/// <summary>
/// デストラクタ
/// </summary>
BossBehavior::~BossBehavior()
{
	Finalize();
}


/// <summary>
/// 初期化処理
/// </summary>
void BossBehavior::Initialize()
{
}


/// <summary>
/// 終了処理
/// </summary>
void BossBehavior::Finalize()
{
}


/// <summary>
/// ビヘイビアツリーの生成
/// </summary>
void BossBehavior::CreateBehaviorTree()
{

}


/// <summary>
/// 待機アクション作成
/// </summary>
void BossBehavior::CreateIdleActionNode()
{
}


void BossBehavior::CreateSweepActionNode()
{
}



