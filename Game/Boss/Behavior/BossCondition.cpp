// ----------------------------------------------------------------------------------
//
// 名前:	BossCondition.cpp
// 説明:    ビヘイビアツリーで条件ノードの実装を行うクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------

// インクルード
#pragma once
#include "pch.h"
#include "BossCondition.h"
#include "Interface/IObject.h"
#include "Game/Boss/Boss.h"

#include "Game/Messenger/EventMessenger.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Boss/Boss.h"
#include "Game/Data/HPSystem.h"


/// <summary>
/// コンストラクタ
/// </summary>
BossCondition::BossCondition(Boss* boss)
	:
	m_boss(boss),
	m_player(nullptr),
	m_playerPos{},
	m_bossPos{}
{
	// 必要な情報を取得する
	GetNecessaryInfo();
}


/// <summary>
/// デストラクタ
/// </summary>
BossCondition::~BossCondition()
{
}


/// <summary>
/// 必要な情報を取得する
/// ボス* プレイヤー*
/// </summary>
void BossCondition::GetNecessaryInfo()
{
	// プレイヤーの取得
	void* object = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = object ? static_cast<IObject*>(object) : nullptr;
}


//__________________________
//
// コンディションの実装
//__________________________


/// <summary>
/// 攻撃範囲内か
/// </summary>
/// <returns>正誤</returns>
bool BossCondition::IsInCloseRange()
{
	// プレイヤの位置を取得
	m_playerPos = m_player->GetPosition();
	// ボスの位置を取得
	m_bossPos = m_boss->GetPosition();

	// 距離を図る
	float distance = DirectX::SimpleMath::Vector3::Distance(m_playerPos, m_bossPos);

	// 攻撃範囲内かどうか
	return distance <= ATTACK_RANGE;
}


/// <summary>
/// 二分の一の確率で成功する
/// </summary>
/// <returns>正誤</returns>
bool BossCondition::IsHalfSuccess()
{
	// 0か1をランダムに取得
	int random = Math::RandomInt(0, 100);

	// 偶数なら成功
	return random % 2 == 0;
}


/// <summary>
/// ボスのHPが半分以上かどうか
/// </summary>
/// <returns>正誤</returns>
bool BossCondition::IsHpOverHalf()
{
	// ボスのHPSystemを取得
	HPSystem* hpSystem = m_boss->GetBossHP();
	// HPを取得
	float currentHp = hpSystem->GetHP();
	float maxHp = hpSystem->GetMaxHP();

	// 半分以上かどうか
	return currentHp >= (maxHp / 2);
}
