/// ---------------------------
///
/// 鬼の金棒の攻撃状態
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"

// コンストラクタ
Cudgel_Attacking::Cudgel_Attacking(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
}

// デストラクタ
Cudgel_Attacking::~Cudgel_Attacking()
{
}


// 初期化処理
void Cudgel_Attacking::Initialize()
{
	using namespace DirectX::SimpleMath;
	// モデルを取得
	m_model = m_cudgel->GetModel();

	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;
}


// 事前処理
void Cudgel_Attacking::PreUpdate()
{
}

// 更新処理
void Cudgel_Attacking::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// 敵を取得
	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
}

// 事後処理
void Cudgel_Attacking::PostUpdate()
{
}

// 描画処理
void Cudgel_Attacking::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	auto debugString = resources->GetDebugString();
	debugString->AddString("Cudgel, %f : %f : %f", m_position.x, m_position.y, m_position.z);
}


// 終了処理
void Cudgel_Attacking::Finalize()
{
}