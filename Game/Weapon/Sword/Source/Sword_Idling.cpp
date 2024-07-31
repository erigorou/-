/// ---------------------------
///
/// プレイヤーの剣の待機状態
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Header/Sword_Idling.h"

// コンストラクタ
Sword_Idling::Sword_Idling(Sword* sword)
	:
	m_sword(sword),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
}

// デストラクタ
Sword_Idling::~Sword_Idling()
{
}


// 初期化処理
void Sword_Idling::Initialize()
{
	using namespace DirectX::SimpleMath;
	// モデルを取得
	m_model = m_sword->GetModel();
	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;
	// 当たり判定
	m_boundingBox = DirectX::BoundingOrientedBox();
}


// 事前処理
void Sword_Idling::PreUpdate()
{
}

// 更新処理
void Sword_Idling::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// プレイヤーの座標を取得
	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();
	// プレイヤーの回転を取得
	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();

	// ワールド行列を更新する
	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);								// サイズの設定
	m_worldMatrix *= SimpleMath::Matrix::CreateTranslation(Sword::SWORD_DIR_FOR_PLAYER)		// 原点で、少しだけずらす
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)									// 回転を行う
		*= SimpleMath::Matrix::CreateTranslation(m_position);								// プレイヤの位置に設定する
}

// 事後処理
void Sword_Idling::PostUpdate()
{
}

// 描画処理
void Sword_Idling::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	auto debugString = resources->GetDebugString();
	debugString->AddString("sword, %f : %f : %f", m_position.x, m_position.y, m_position.z);
}


// 終了処理
void Sword_Idling::Finalize()
{
}