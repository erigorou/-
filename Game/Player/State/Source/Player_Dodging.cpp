#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/EasingFunctions.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/Player_Dodging.h"


// --------------------------------------
// コンストラクタ
// --------------------------------------
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_model(nullptr)
	,m_totalSeconds(0.0f)
	,m_finishTime(0.0f)
	,m_rollingValue()

{
}

// --------------------------------------
// デストラクタ
// --------------------------------------
PlayerDodging::~PlayerDodging()
{
}


// -------------------------------------
// 初期化処理
// --------------------------------------
void PlayerDodging::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
}


// -------------------------------------
// プレイヤーの事前更新処理
// --------------------------------------
void PlayerDodging::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	// 移動方向の設定
	m_velocity = DirectX::SimpleMath::Vector3::Forward * DODGING_SPEED;
}


// -------------------------------------
// プレイヤーの更新処理
// --------------------------------------
void PlayerDodging::Update(const float& elapsedTime)
{
	float t = elapsedTime;	
	// 経過時間を加算
	m_totalSeconds += t;
	// アニメーションの更新
	UpdateAnimation(m_totalSeconds);
	// ステート開始から時間を計測、一定時間で別のStateへ遷移させる
	m_player->TimeComparison(m_totalSeconds, ANIMATION_END_TIME, m_player->GetPlayerIdlingState(), elapsedTime);
}


// -------------------------------------
// プレイヤーのアニメーション用更新処理
// --------------------------------------
void PlayerDodging::UpdateAnimation(float totalTime)
{
	// プレイヤーの座標を取得
	m_position = m_player->GetPosition();

	// ローリングアニメーション
	RollingAnimation(totalTime);
	// キャラクターの移動を調整
	AdjustCharacterTransition(totalTime);
}


// -------------------------------
// ローリングアニメーション
// -------------------------------
void PlayerDodging::RollingAnimation(float totalTime)
{
	// イージングの要素数
	float value;

	// 回転処理の部分
	if (totalTime <= SPIN_TIME)
	{
		value = Easing::easeInOutQuad(totalTime / SPIN_TIME);

		m_rollingValue.x = DirectX::XMConvertToRadians(value * ROLLING_ROT + ROLLING_REST_ROT);
	}
	// ローリング後の硬直処理
	else if (totalTime <= SPIN_REST_TIME)
	{
		// SPIN_TIME を越えた分の時間で割合を計算
		value = (totalTime - SPIN_TIME) / (SPIN_REST_TIME - SPIN_TIME);
		float easeValue = Easing::easeOutBack(value);

		float rot = ROLLING_REST_ROT - (easeValue * ROLLING_REST_ROT);

		// 回転をラジアンに変換
		m_rollingValue.x = DirectX::XMConvertToRadians(rot);
	}
	// 回転を適用
	m_player->SetAnimarionRotate(m_rollingValue);
}


// -----------------------------------------
// プレイヤーの移動を調整
// -----------------------------------------
void PlayerDodging::AdjustCharacterTransition(float totalTime)
{
	// イージングの要素数
	float value;

	// ローリング処理の部分
	if (totalTime <= ANIMATION_TIME)
	{
		// イージングを掛けるための時間
		value = Easing::easeOutCubic(totalTime / ANIMATION_TIME);
		m_position.y = value * UP_VALUE;
	}
	// ローリング後の硬直処理
	else if (totalTime - ANIMATION_TIME <= DODGING_RECOVERY_TIME - ANIMATION_TIME)
	{
		// 後処理を行う
		value = Easing::easeInOutQuad((totalTime - ANIMATION_TIME) / (DODGING_RECOVERY_TIME - ANIMATION_TIME));
		m_position.y = UP_VALUE - (value * UP_VALUE);
	}

	// プレイヤーの移動を適用
	ApplyPlayerMovement(m_position);
}



// プレイヤーの移動を適用
void PlayerDodging::ApplyPlayerMovement(DirectX::SimpleMath::Vector3& parentPos)
{
	// プレイヤーの回転を取得
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// 速度を設定
	m_velocity *= DODGE_FUNCTION;
	// 端数を消し飛ばす。
	m_velocity = Math::truncate_vector(m_velocity, TRUNCATION_DIGIT);
	// 移動量を座標に反映させながら座標を移動させる。
	parentPos += Vector3::Transform(m_velocity, angle);
	// プレイヤーの座標を更新
	m_player->SetPosition(parentPos);
}




// キー入力
void PlayerDodging::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

void PlayerDodging::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


// 事後更新処理
void PlayerDodging::PostUpdate()
{
	m_position.y = 0.0f;
	// プレイヤーの座標を更新
	m_player->SetPosition(m_position);
}


// 描画処理
void PlayerDodging::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);

#ifdef _DEBUG
	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();
	// デバッグ文字の描画
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // _DEBUG
}


// 終了処理
void PlayerDodging::Finalize()
{
}
