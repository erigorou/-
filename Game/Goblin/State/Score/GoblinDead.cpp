// --------------------------------------------------
//名前:	GoblinDead.cpp
//内容:	ゴブリンの死亡状態
//作成:	池田桜輔
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "../Header/GoblinDead.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/EasingFunctions.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="goblin">ゴブリンのポインタ</param>
// ---------------------------------------------
GoblinDead::GoblinDead(Goblin* goblin)
	: m_goblin{ goblin }
	, m_totalTime{}
	, m_scale{1.0f}
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
GoblinDead::~GoblinDead()
{
}

// ---------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------
void GoblinDead::Initialize()
{
}

// ---------------------------------------------
/// <summary>
/// 初期化（in処理）
/// </summary>
// ---------------------------------------------
void GoblinDead::PreUpdate()
{
	// 攻撃フラグを無効にする
	m_goblin->SetIsAttacking(false);
	m_totalTime = 0.0f;
	m_scale = 1.0f;

	// ゴブリンの状態開始時の位置を取得
	m_startPos = m_goblin->GetPosition();
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void GoblinDead::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;

	// アニメーションを更新する
	UpdateAnimation();

	// 一定時間経過したらゴブリンを消滅させる
	if (m_totalTime > DEAD_TIME) m_goblin->DeleteGoblin();
}

// ---------------------------------------------
/// <summary>
/// アニメーション更新処理
/// </summary>
// ---------------------------------------------
void GoblinDead::UpdateAnimation()
{
	// イージングアニメーションを使ってスケールを変更する
	m_scale = 1 - Easing::easeOutBounce(m_totalTime / DEAD_TIME);

	// 座標をアニメーションを用いながら0にする
	m_startPos.y = m_startPos.y - m_startPos.y * Easing::easeOutBounce(m_totalTime / DEAD_TIME);

	// ゴブリンに計算した情報を与える
	m_goblin->SetScale(DirectX::SimpleMath::Vector3(m_scale, m_scale, m_scale));
	m_goblin->SetPosition(m_startPos);
}

// ---------------------------------------------
/// <summary>
/// 初期化（out処理）
/// </summary>
// ---------------------------------------------
void GoblinDead::PostUpdate()
{
}

// ---------------------------------------------
/// <summary>
/// 終了処理を行う
/// </summary>
// ---------------------------------------------
void GoblinDead::Finalize()
{
}