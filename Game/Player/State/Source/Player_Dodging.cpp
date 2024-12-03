#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/Player_Dodging.h"

// 固定値
const float PlayerDodging::DODGING_TIME = 0.5f;
const float PlayerDodging::DODGE_FUNCTION = 0.9f;
const int   PlayerDodging::TRUNCATION_DIGIT = 4;

// コンストラクタ
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_model(nullptr)
	,m_totalSeconds(0.0f)
	,m_inputVelocity(0.0f, 0.0f, 0.0f)
	,m_finishTime(0.0f)
{
}

// デストラクタ
PlayerDodging::~PlayerDodging()
{
}


// 初期化処理
void PlayerDodging::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
}

// 事前更新処理
void PlayerDodging::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;

	m_velocity = DirectX::SimpleMath::Vector3::Forward * DODGING_SPEED;

	m_position = m_player->GetPosition();
}

// 更新処理
void PlayerDodging::Update(const float& elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// プレイヤーの座標を取得
	Vector3 parentPos = m_player->GetPosition();

	// ステート開始から時間を計測、一定時間で別のStateへ遷移させる
	m_player->TimeComparison(m_totalSeconds, DODGING_TIME, m_player->GetPlayerIdlingState(), elapsedTime);

	// プレイヤーの回転を取得
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// 速度を設定
	m_velocity *= DODGE_FUNCTION;
	// 端数を消し飛ばす。
	m_velocity = Math::truncate_vector(m_velocity, TRUNCATION_DIGIT);
	// 移動量を座標に反映させながら座標を移動させる。
	parentPos +=Vector3::Transform(m_velocity,angle);

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
	// 修正点があればここに記載
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
	debugString->AddString("PlayerDodging");
#endif // _DEBUG
}


// 終了処理
void PlayerDodging::Finalize()
{
}
