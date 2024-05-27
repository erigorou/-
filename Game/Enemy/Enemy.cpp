// ----------------
//
// 鬼の挙動
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"

#include "Game/Enemy/Enemy.h"

// --------------------------------
//  コンストラクタ
// --------------------------------
Enemy::Enemy()
{
}

// --------------------------------
//  デストラクタ
// --------------------------------
Enemy::~Enemy()
{
}

// --------------------------------
//  イニシャライズ
// --------------------------------
void Enemy::Initialize(
	ID3D11Device* device,
	const ID3D11DeviceContext* context,
	const DirectX::CommonStates* states
	)
{
	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// モデルを読み込む(仮でサイコロを読み込む)
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/akaoni.cmo", *fx);

	// 座標の初期設定
	m_position = DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, 0.0f };
	// 回転角
	m_angle = 0.0f;
	// ワールド行列の初期か
	m_worldMatrix  = DirectX::SimpleMath::Matrix::Identity;

}

// --------------------------------
//  更新処理
// --------------------------------
void Enemy::Update()
{
	using namespace DirectX::SimpleMath;

	m_worldMatrix = Matrix::CreateScale(Vector3(2.0f, 2.0f, 2.0f));

	// キー入力を受け付ける。
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1)
	{
		m_position += Vector3(0.01f, 0.0f, 0.0f);
	}
	if (keyboardState.F2)
	{
		m_position -= Vector3(0.01f, 0.0f, 0.0f);
	}

	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}


// --------------------------------
//  表示処理
// --------------------------------
void Enemy::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources
	)
{
	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// デバッグ情報を「DebugString」で表示する
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyPos.x : %f", m_position.x);
	debugString->AddString("enemyPos.y : %f", m_position.y);
	debugString->AddString("enemyPos.z : %f", m_position.z);
}


// --------------------------------
//  終了処理
// --------------------------------
void Enemy::Finalize()
{
}