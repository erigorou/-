// -----------------------------------------------------
// 名前:	BossApproaching.cpp
// 内容:	ボスの追尾ステートクラス
// 作成:	池田桜輔
// -----------------------------------------------------
// インクルード
#include "pch.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Sound/Sound.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Boss/Boss.h"
#include "Game/Boss/States/Header/BossApproaching.h"

// --------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="boss">ボスのポインタ</param>
// --------------------------------------
BossApproaching::BossApproaching(Boss* boss)
	:
	m_boss(boss),
	m_position{},
	m_velocity{},
	m_angle{},
	m_totalSeconds{},
	m_player{}
{
}

// --------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------
BossApproaching::~BossApproaching()
{
}

// --------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------
void BossApproaching::Initialize()
{
	// 速度を設定（前にしか動かない）
	m_velocity = DirectX::SimpleMath::Vector3::Forward;

	// プレイヤーの取得
	void* player = EventMessenger::ExecuteGetter(GetterList::GetPlayer);
	m_player = player ? static_cast<IObject*>(player) : nullptr;
}

// --------------------------------------
/// <summary>
/// 変更処理(in)
/// </summary>
/// --------------------------------------
void BossApproaching::PreUpdate()
{
	// 経過時間を初期化
	m_totalSeconds = 0.0f;
	// 座標の取得
	m_position = m_boss->GetPosition();
	// 回転行列の取得
	m_angle = m_boss->GetAngle();
}

// --------------------------------------
/// <summary>
/// 更新処理
/// </summary>
// --------------------------------------
void BossApproaching::Update(const float& elapsedTime)
{
	// 合計の時間を計算する
	m_totalSeconds += elapsedTime;
	// アニメーションの更新
	UpdateAnimation(elapsedTime);
	// 次のステートに移行するかを検知
	CheckNextState();
}

// --------------------------------------
/// <summary>
/// アニメーションの更新
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// --------------------------------------
void BossApproaching::UpdateAnimation(float elapsedTime)
{
	// サイン波の計算(上下移動)
	m_position.y = Math::CalculatingSinWave(m_totalSeconds, AMPLITUDE, FREQUENCY);
	// 絶対値を取ることでジャンプしているような挙動をする　※
	m_position.y = fabsf(m_position.y);

	// プレイヤーの座標を取得
	Vector3 playerPos = m_player->GetPosition();

	// 敵から見たプレイヤーの位置を計算する
	m_angle = Math::CalculationAngle(m_position, playerPos);
	// 回転行列の作成
	Matrix angleMat = Matrix::CreateScale(Boss::BOSS_SCALE) * Matrix::CreateRotationY(-m_angle);
	// 向いている方向の前方に移動
	m_position += Vector3::Transform(m_velocity * elapsedTime * NORMALIZE_VELOCITY, angleMat);
	// 回転角を設定する
	m_boss->SetAngle(m_angle);
	// 座標を設定する
	m_boss->SetPosition(m_position);
}

// --------------------------------------
/// <summary>
/// 次のステートに移行するかを検知
/// </summary>
// --------------------------------------
void BossApproaching::CheckNextState()
{
	// プレイヤーの座標を取得
	Vector3 playerPos = m_player->GetPosition();

	// 移動でカメラを揺らす
	if (m_position.y <= MINIMAL)
	{
		// カメラを揺らす
		float shakePower = SHAKE_POWER;
		EventMessenger::Execute(EventList::ShakeCamera, &shakePower);
		// パーティクルを生成
		EventMessenger::Execute(EventList::CreateBashDust, &m_position);

		// サウンドを再生
		Sound::PlaySE(Sound::SE_TYPE::BOSS_MOVE);

		// プレイヤーとの距離が近い場合
		if (DirectX::SimpleMath::Vector3::Distance(m_position, playerPos) <= MIN_DISTANCE)
		{
			// ボスの状態を保存する変数
			BossState state = BossState::Idling;

			// ランダムに次は度の挙動をするか決める
			int random = Math::RandomInt(0, TOTAL_RATE);
			// 薙ぎ払い
			if (random <= SWEEPING_RATE) {
				state = BossState::Sweeping;
			}
			// たたきつけ
			else if (random == ATTACKING_RATE) {
				state = BossState::Attacking;
			}
			// 何もしない
			else if (random == IDLING_RATE) {
				state = BossState::Idling;
			}

			// 状態を変更
			EventMessenger::Execute(EventList::ChangeBossState, &state);
			// 以降の処理は行わない
			return;
		}
	}

	// 一定秒数経過で待機モーションに変更
	if (m_totalSeconds >= TOTAL_TIME)
	{
		// 待機状態に変更
		BossState state = BossState::Idling;
		EventMessenger::Execute(EventList::ChangeBossState, &state);
	}
}

// --------------------------------------
/// <summary>
/// 変更処理(out)
/// </summary>
// --------------------------------------
void BossApproaching::PostUpdate()
{
	// 敵の位置を0で固定する
	m_position.y = 0.0f;
	// 敵の位置を設定する
	m_boss->SetPosition(m_position);
	// カメラを揺らす
	float shakePower = SHAKE_POWER;
	EventMessenger::Execute(EventList::ShakeCamera, &shakePower);
	// パーティクルを生成
	EventMessenger::Execute(EventList::CreateBashDust, &m_position);
}

// --------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// --------------------------------------
void BossApproaching::Finalize()
{
}