/// ---------------------------
///
/// 鬼の金棒の待機状態
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
#include "Game/Weapon/Cudgel/Header/Cudgel_Idling.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"

// コンストラクタ
Cudgel_Idling::Cudgel_Idling(Cudgel* cudgel)
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
Cudgel_Idling::~Cudgel_Idling()
{
}


// 初期化処理
void Cudgel_Idling::Initialize()
{
	using namespace DirectX::SimpleMath;
	// モデルを取得
	m_model = m_cudgel->GetModel();

	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;
}


// 事前処理
void Cudgel_Idling::PreUpdate()
{
}

// 更新処理
void Cudgel_Idling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// 敵を取得
	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();

	// 鬼の座標を取得
	m_position = enemy->GetPosition();
	// 鬼の回転を取得
	m_angle	= enemy->GetAngle();

	// ワールド行列を計算する
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE)		// 大きさの設定　＆　リセット
		*= Matrix::CreateTranslation(Vector3(14.0f, 2.0f, 0.0f))	// 原点の位置からすこしずらす
		*= Matrix::CreateRotationY(-m_angle)						// 剣全体の回転を行う
		*= Matrix::CreateTranslation(m_position);					// 鬼の座標に移動する
}

// 事後処理
void Cudgel_Idling::PostUpdate()
{
}

// 描画処理
void Cudgel_Idling::Render(ID3D11DeviceContext* context,
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
void Cudgel_Idling::Finalize()
{
}