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

#include "Game/Weapon/Sword/Sword.h"
#include "Game/Player/Player.h"

// コンストラクタ
SwordIdling::SwordIdling(Sword* sword)
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
SwordIdling::~SwordIdling()
{
}


// 初期化処理
void SwordIdling::Initialize()
{
	using namespace DirectX::SimpleMath;
	// モデルを取得
	m_model = m_sword->GetModel();
	// ワールド行列を初期化

}


// 事前処理
void SwordIdling::PreUpdate()
{
}

// 更新処理
void SwordIdling::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	m_position = m_sword->GetPlayScene()->GetPlayer()->GetPosition();

	m_angle = m_sword->GetPlayScene()->GetPlayer()->GetAngle();

	m_worldMatrix = Matrix::CreateScale(Sword::SWORD_SCALE);

	m_worldMatrix *= SimpleMath::Matrix::CreateTranslation(Vector3(4.0f, 0.5f, 0.0f))
		*= SimpleMath::Matrix::CreateRotationY(-m_angle)
		*= SimpleMath::Matrix::CreateTranslation(m_position);
}

// 事後処理
void SwordIdling::PostUpdate()
{
}

// 描画処理
void SwordIdling::Render(ID3D11DeviceContext* context,
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
void SwordIdling::Finalize()
{
}