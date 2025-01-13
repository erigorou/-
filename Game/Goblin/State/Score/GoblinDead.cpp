#include "pch.h"
#include "../Header/GoblinDead.h"
#include "../../Goblin.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Scene/PlayScene.h"
#include "Game/EnemyManager/EnemyManager.h"


// -------------------
// コンストラクタ
// -------------------
GoblinDead::GoblinDead(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
	, m_scale(1.0f)
{
}


// -------------------
// デストラクタ
// -------------------
GoblinDead::~GoblinDead()
{
}


// -------------------
// 初期化する
// -------------------
void GoblinDead::Initialize()
{
}



// -------------------
// 初期化(in)
// -------------------
void GoblinDead::PreUpdate()
{
	m_goblin->SetIsAttacking(false);
	m_totalTime = 0.0f;
	m_scale = 1.0f;

	// ゴブリンの位置を取得
	m_startPos = m_goblin->GetPosition();
	m_startPos_y = m_startPos.y;
}



// -------------------
// 更新する
// -------------------
void GoblinDead::Update(const float& elapsedTime)
{
	// 時間を加算する
	m_totalTime += elapsedTime;

	// アニメーションを更新する
	UpdateAnimation();

	// 一定時間経過したら
	if (m_totalTime > DEAD_TIME)	m_goblin->DeleteGoblin();
}


// -------------------
// アニメーション更新処理
// -------------------
void GoblinDead::UpdateAnimation()
{
	// イージングアニメーションを使ってスケールを変更する
	m_scale = 1 - Easing::easeInBack(m_totalTime / DEAD_TIME);

	// イージングアニメーションを使って位置を変更する
	m_startPos.y = m_startPos_y - m_startPos_y * Easing::easeOutBounce(m_totalTime / DEAD_TIME);

	// ゴブリンに計算した情報を与える
	m_goblin->SetScale(DirectX::SimpleMath::Vector3(m_scale, m_scale, m_scale));
	m_goblin->SetPosition(m_startPos);
}


// -------------------
// 初期化(out)
// -------------------
void GoblinDead::PostUpdate()
{
}


// -------------------
// 後処理を行う
// -------------------
void GoblinDead::Finalize()
{
}