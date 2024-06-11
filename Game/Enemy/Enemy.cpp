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

#include "Interface/IState.h"

// 固定値
const float Enemy::ENEMY_SPEED = 0.05f;

// --------------------------------
//  コンストラクタ
// --------------------------------
Enemy::Enemy()
	:
	 m_position{0.0f, 0.0f, 0.0f}
	,m_angle{0.0f}
	,m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
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

	// 待機状態を取得する
	m_enemyIdling = std::make_unique<EnemyIdling>(this);
	m_enemyIdling->Initialize();

	// 初期のステートを待機状態に割り当てる
	m_currentState = m_enemyIdling.get();

	// ビヘイビアツリーを取得
	m_pBT = std::make_unique<BehaviorTree>();
}

// --------------------------------
//  更新処理
// --------------------------------
void Enemy::ChangeState(IState* newState)
{
	// 新規の状態遷移前に事後更新を行う
	m_currentState->PostUpdate();
	// 新規の状態を現在の状態に設定する
	m_currentState = newState;
	// 新規の状態遷移後に事前更新を行う
	m_currentState->PreUpdate();
}


// --------------------------------
//  更新処理
// --------------------------------
void Enemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_worldMatrix = Matrix::CreateScale(Vector3(2.0f, 2.0f, 2.0f));

	// ステータスを更新しまーす
	m_currentState->Update(elapsedTime);

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

	if (keyboardState.Enter)
	{
		m_pBT->run();
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
	// ステートのほうを表示する
	m_currentState->Render(context,states,view,projection, resources);

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