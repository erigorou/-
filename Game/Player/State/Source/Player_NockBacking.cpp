#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/Player_NockBacking.h"


// -------------------------------
// コンストラクタ
// -------------------------------
PlayerNockBacking::PlayerNockBacking(Player* player)
	: m_player(player)
	, m_totalSeconds(0.0f)
	, m_elapsedTime(0.0f)
	, m_velocity(0.0f, 0.0f, -SPEED)
	, m_bodyTilt(0.0f)
{
}

// -------------------------------
// デストラクタ
// -------------------------------
PlayerNockBacking::~PlayerNockBacking()
{
}


// -------------------------------
// 初期化処理
// -------------------------------
void PlayerNockBacking::Initialize()
{
}


// -------------------------------
// 事前更新処理
// -------------------------------
void PlayerNockBacking::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;
}


// -------------------------------
// 更新処理
// -------------------------------
void PlayerNockBacking::Update(const float& elapsedTime)
{
	// 経過時間を記録
	m_elapsedTime = elapsedTime;

	m_position = m_player->GetPosition();

	DirectX::SimpleMath::Vector3 enemyPos = m_player->GetPlayScene()->GetEnemy()->GetPosition();

	m_player->SetAngle(Math::CalculationAngle(m_position, enemyPos));

	// アニメーションの更新
	UpdateAnimation();

	// ステート開始から時間を計測、一定時間で別のStateへ遷移させる
	m_player->TimeComparison(m_totalSeconds, END_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// -------------------------------
// アニメーション更新用処理
// -------------------------------
void PlayerNockBacking::UpdateAnimation()
{
	// 経過時間を加算
	m_totalSeconds += m_elapsedTime;

	// ノックバックのアニメーション
	if (m_totalSeconds <= NOCKBACKING_TIME)
	{
		NockBackAnimation();
	}
	// 
	else if (m_totalSeconds <= RETURN_TIME)
	{
		ReturnAnimation();
	}
}


// -------------------------------
// ノックバックアニメーション
// -------------------------------
void PlayerNockBacking::NockBackAnimation()
{
	// 時間を正規化する
	float t = std::min(1.0f, m_totalSeconds / NOCKBACKING_TIME);

	// 必要情報の取得
	Vector3 playerPos = m_player->GetPlayScene()->GetEnemy()->GetPosition();
	Vector3 enemyPos = m_player->GetPosition();

	// どっちの方向にノックバックするのか
	m_nockBackAngle = Math::CalculationAngle(playerPos, enemyPos);
	Matrix rotMatrix = Matrix::CreateRotationY(-m_nockBackAngle);

	// ノックバック量
	m_position += Vector3::Transform(m_velocity, rotMatrix) * m_elapsedTime;

	// sin波で上下移動を実装
	m_position.y = Math::NormalizeSin(t) * MAX_UP_VALUE;

	// 傾き
	m_bodyTilt.x = DirectX::XMConvertToRadians(-90 * Easing::easeOutBack(t));

	// 設定項目
	m_player->SetPosition(m_position);
	m_player->SetAnimationRotate(m_bodyTilt);
}


// -------------------------------
// 復帰アニメーション
// -------------------------------
void PlayerNockBacking::ReturnAnimation()
{
	// 時間を正規化する
	float t = std::min(1.0f, (m_totalSeconds - NOCKBACKING_TIME) / (RETURN_TIME - NOCKBACKING_TIME));

	// ノックバック量
	Matrix rotMatrix = Matrix::CreateRotationY(-m_nockBackAngle);
	m_position += Vector3::Transform(m_velocity, rotMatrix) * m_elapsedTime;

	// sin波で上下移動を実装
	m_position.y = Math::NormalizeSin(t) * RETURN_UP_VALUE;

	// 傾き
	m_bodyTilt.x = DirectX::XMConvertToRadians(-90 - 270 * Easing::easeOutBack(t));

	m_player->SetPosition(m_position);
	m_player->SetAnimationRotate(m_bodyTilt);
}



// -------------------------------
// キー入力イベント
// -------------------------------
void PlayerNockBacking::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}
void PlayerNockBacking::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// -------------------------------
// 事後更新処理
// -------------------------------
void PlayerNockBacking::PostUpdate()
{
}


// -------------------------------
// 終了処理
// -------------------------------
void PlayerNockBacking::Finalize()
{
}
