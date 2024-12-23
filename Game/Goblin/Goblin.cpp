// -------------------------------------------------------
// 小鬼の親クラス
// -------------------------------------------------------

#include "pch.h"
#include "Goblin.h"
#include "Game/Player/Player.h"
#include "Game/Scene/PlayScene.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/DebugString.h"
#include "Interface/IState.h"
#include "Game/Data/HPSystem.h"
#include "../Enemy/Enemy.h"

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"


// ---------------
// 固定値
// ---------------
const float Goblin::GOBLIN_SPEED = Enemy::ENEMY_SPEED / 2.0f;	// 移動速
const float Goblin::GOBLIN_SCALE = Enemy::ENEMY_SCALE / 4.0f;	// サイズ


// コンストラクタ
Goblin::Goblin(PlayScene* playScene)
	: m_playScene(playScene)
	, m_position{ 80.f, 0.f, 0.f }
	, m_velocity{ 0.f, 0.f, 0.f }
	, m_angle{ 0.f, 0.f, 0.f }
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
	, m_nowAttacking(false)
{
}


// デストラクタ
Goblin::~Goblin()
{
}


// 初期化関数
void Goblin::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Body/oni.cmo", *fx);

	// ステートの作成
	CreateState();
	// 当たり判定の生成
	CreateCollision();
	// HPの生成
	m_hp = std::make_unique<HPSystem>(GOBLIN_HP);
}


// ステートの作成
void Goblin::CreateState()
{
	m_idling		=	std::make_unique<GoblinIdling>		(this);	// 待機
	m_attacking		=	std::make_unique<GoblinAttacking>	(this);	// 攻撃

	m_idling	->	Initialize();
	m_attacking	->	Initialize();

	m_currentState = m_idling.get();
}

// プレイヤーに当たったときの処理
void Goblin::HitPlayer(InterSectData data)
{
	if (data.objType == ObjectType::Player && data.colType == CollisionType::Sphere)
	{
	}
}


// 小鬼に当たったときの処理
void Goblin::HitGoblin(InterSectData data)
{
	if (data.objType == ObjectType::Goblin && data.colType == CollisionType::Sphere)
	{
	}
}

void Goblin::HitEnemy(InterSectData data)
{
}

void Goblin::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		// リセット
		m_oushBackValue = DirectX::SimpleMath::Vector3::Zero;
	}
}


// 当たり判定の生成
void Goblin::CreateCollision()
{
	// 当たり判定の生成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE * COLLISION_RADIUS);

	// 当たり判定をCollisionManagerに登録する
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Goblin,
		CollisionType::Sphere,
		this,
		m_bodyCollision.get());
}


// 更新処理
void Goblin::Update(const float elapsedTime)
{
	// ワールド行列の初期化
	m_worldMatrix = 
		DirectX::SimpleMath::Matrix::CreateScale(GOBLIN_SCALE) * DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	// ステートの更新処理
	m_currentState->Update(elapsedTime);
	// 衝突判定の移動
	MoveCollision();
}


// 衝突判定の移動
void Goblin::MoveCollision()
{
	DirectX::SimpleMath::Vector3 pos = m_position;
	pos.y = 2.0f;
	m_bodyCollision->Center = pos;
}


// 描画関数
void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// 描画に必要なデータを取得する
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルの描画
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// 終了関数
void Goblin::Finalize()
{
	m_idling	->	Finalize();
	m_attacking	->	Finalize();

	// 当たり判定の削除
	m_playScene->GetCollisionManager()->DeleteCollision(CollisionType::Sphere, this);
}


// 当たったときの処理
void Goblin::HitAction(InterSectData data)
{
	switch (data.objType)
	{
	case ObjectType::Player:	HitPlayer(data);	break;
	case ObjectType::Goblin:	HitGoblin(data);	break;
	case ObjectType::Enemy:		HitEnemy(data);		break;
	case ObjectType::Stage:		HitStage(data);		break;
	}
}


// ステートの変更
void Goblin::ChangeState(IState* state)
{
	// すでに同じステートの場合は変更しない
	if (m_currentState == state) return;
	// 変更前ステートの最終処理
	m_currentState->PostUpdate();
	// ステートの変更
	m_currentState = state;
	// 変更後ステートの初期処理
	m_currentState->PreUpdate();
}


// ダメージを受けたときの処理
void Goblin::Damaged(float damage)
{
	m_hp->Damage(damage);
}