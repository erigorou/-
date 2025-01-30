#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries//MyLib/EasingFunctions.h"

#include "Game/Boss/Boss.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "../../States/Header/BossDashAttacking.h"


// --------------------
// コンストラクタ
// --------------------
BossDashAttacking::BossDashAttacking(Boss* boss)
	:
	m_angle(0.0f),
	m_bodyTilt(0.0f),
	m_boss(boss),
	m_totalSeconds()
{
}


// --------------------
// デストラクタ
// --------------------
BossDashAttacking::~BossDashAttacking()
{
}


// --------------------
// 初期化処理
// --------------------
void BossDashAttacking::Initialize()
{
}


// --------------------
// 事前更新処理
// --------------------
void BossDashAttacking::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;

	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceAttacking());

	// 最初は攻撃中ではない
	m_isAttacking = false;
}


// --------------------
// 更新処理
// --------------------
void BossDashAttacking::Update(const float& elapsedTime)
{
	// 経過時間を更新
	m_elapsedTime = elapsedTime;
	m_totalSeconds += m_elapsedTime;

	// 敵の挙動を更新する
	UpdateAction();

	// 待機状態に遷移
	if (m_totalSeconds >= TOTAL_TIME)
		m_boss->ChangeState(m_boss->GetBossIdling());
}


// --------------------
// 敵の挙動更新処理
// --------------------
void BossDashAttacking::UpdateAction()
{
	// ためモーションの時
	if (m_totalSeconds		<= CHARGE_TIME)	ChargeAction();	// 貯め
	else if (m_totalSeconds <= DASH_TIME)	DashAction();	// ダッシュ
	else if (m_totalSeconds <= WAIT_TIME)	WaitAction();	// 待機
	else if (m_totalSeconds <= RETURN_TIME)	ReturnAction();	// 元に戻る

	// 衝突可能かどうか
	m_boss->GetPlayScene()->GetPlayer()->CanHit(m_isAttacking);
}


// --------------------
// 貯め
// --------------------
void BossDashAttacking::ChargeAction()
{
	// プレイヤーの座標を取得
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	// 敵の座標を取得
	Vector3 parentPos = m_boss->GetPosition();
	// 敵から見たプレイヤーの位置を設定する
	m_angle = Math::CalculationAngle(parentPos, playerPos);
	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);
	m_boss->SetAngle(m_angle);

	// イージング用の変数
	float t = m_totalSeconds / CHARGE_TIME;
	// 体の傾きの角度設定
	m_bodyTilt = DirectX::XMConvertToRadians(-20 * Easing::easeOutBack(t));
	m_boss->SetBodyTilt(m_bodyTilt);
}


// --------------------
// ダッシュ
// --------------------
void BossDashAttacking::DashAction()
{
	// アタック中
	m_isAttacking = true;

	// 現在の時間に基づいてサイン波で加速度を計算
	float t = (m_totalSeconds - CHARGE_TIME) / (DASH_TIME - CHARGE_TIME);
	
	// 座標の更新 *
	float accelerationFactor = sin(static_cast<float>(t * M_PI)); // サイン波で速度を変化

	// 敵の向きに基づいて前方向を計算
	Vector3 position = m_boss->GetPosition();
	m_velocity = Vector3(0, 0, -MAX_SPEED * accelerationFactor);
	position += Vector3::Transform(m_velocity, m_rotMatrix) * m_elapsedTime;

	// サイン波で上下運動
	float y = fabsf(sin(t * 15.0f)) * accelerationFactor;
	position.y = y;

	// 計算した座標を本体に設定する
	m_boss->SetPosition(position);

	// 傾きの更新 *
	// プレイヤーを傾ける
	m_bodyTilt = DirectX::XMConvertToRadians(-20 + 40 * Easing::easeOutBack(t));
	m_boss->SetBodyTilt(m_bodyTilt);
}


// --------------------
// 待機
// --------------------
void BossDashAttacking::WaitAction()
{
	// アタック終わり
	m_isAttacking = false;

	// イージングに使用する秒数を計算（秒数のNormalize)
	float t = (m_totalSeconds - DASH_TIME) / (WAIT_TIME - DASH_TIME);

	// プレイヤーを傾ける *
	m_bodyTilt = DirectX::XMConvertToRadians(20 - 20 * Easing::easeOutBounce(t));
	m_boss->SetBodyTilt(m_bodyTilt);
}


// --------------------
// 元に戻る
// --------------------
void BossDashAttacking::ReturnAction()
{
	// 時間の正規化
	float t = (m_totalSeconds - WAIT_TIME) / (RETURN_TIME - WAIT_TIME);

	// プレイヤーの座標を取得
	Vector3 playerPos = m_boss->GetPlayScene()->GetPlayer()->GetPosition();
	// 敵の座標を取得
	Vector3 parentPos = m_boss->GetPosition();
	// 敵から見たプレイヤーの位置を設定する
	float angle = Math::CalculationAngle(parentPos, playerPos);

	m_angle = Math::LerpFloat(m_angle, angle, t);

	m_rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle);
	m_boss->SetAngle(m_angle);
}



// --------------------
// 事後更新処理
// --------------------
void BossDashAttacking::PostUpdate()
{
	// 武器のステートを変更する
	auto cudgel = m_boss->GetPlayScene()->GetCudgel();
	cudgel->ChangeState(cudgel->GetIdling());

	// 顔のステートを変更
	m_boss->SetFace(m_boss->GetFaceIdling());
}


// --------------------
// 終了処理
// --------------------
void BossDashAttacking::Finalize()
{
}