#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Player/Player.h"
#include "Game/Player/State/Header/PlayerDodging.h"

// 固定値
const float PlayerDodging::DODGING_TIME = 1.0f;
const float PlayerDodging::DODGE_FUNCTION = 0.9f;
const int   PlayerDodging::TRUNCATION_DIGIT = 5;

// コンストラクタ
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_totalSeconds()
	,m_direction()
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

	m_velocity = m_player->GetVelocity() * 4.f;
}

// 更新処理
void PlayerDodging::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// ステートの経過時間を記録する
	m_totalSeconds += elapsedTime;

	if (m_totalSeconds >= DODGING_TIME)
	{
		// 一定時間経過後にシーンを待機に戻す
		m_player->ChangeState(m_player->GetPlayerIdlingState());
		return;
	}

	// プレイヤーの回転を取得
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// 速度を設定
	m_velocity *= DODGE_FUNCTION;



	// 移動量を座標に反映させながら座標を移動させる。
	parentPos -=Vector3::Transform(m_velocity,angle);
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
	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();

	// デバッグ文字の描画
	auto debugString = resources->GetDebugString();
	debugString->AddString("PlayerDodging");
	debugString->AddString("Dodging::Direction = [ %f, %f, %f ]", m_direction.x, m_direction.y, m_direction.z);

}


// 終了処理
void PlayerDodging::Finalize()
{
}
