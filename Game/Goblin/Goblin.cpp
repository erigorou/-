// -------------------------------------------------------
// 小鬼の親クラス
// -------------------------------------------------------

#include "pch.h"
#include "Goblin.h"
#include "Game/Player/Player.h"
#include "Game/Scene/PlayScene.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "Interface/IState.h"

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"

// コンストラクタ
Goblin::Goblin(PlayScene* playScene)
	: m_playScene(playScene)
	, m_position{ 0.f, 0.f, 0.f }
	, m_velocity{ 0.f, 0.f, 0.f }
	, m_angle{ 0.f, 0.f, 0.f }
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
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
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/oni.cmo", *fx);


}



void Goblin::CreateState()
{
	m_idling		=	std::make_unique<GoblinIdling>		(this);	// 待機
	m_attacking		=	std::make_unique<GoblinAttacking>	(this);	// 攻撃

	m_idling	->	Initialize(m_model.get());
	m_attacking	->	Initialize(m_model.get());

	m_currentState = m_idling.get();
}



void Goblin::CreateCollision()
{
	// 当たり判定の生成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE);

	// 当たり判定をCollisionManagerに登録する
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Goblin,
		this,
		m_bodyCollision.get());
}



void Goblin::Update(const float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// ワールド行列の初期化
	m_worldMatrix = Matrix::Identity;

	//// ステートの更新処理
	//m_currentState->Update(elapsedTime);
}


void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	//m_currentState->Render();				// ステート側の描画
	m_model->Draw(context, *states, m_worldMatrix, view, projection);	// モデルの描画
}



void Goblin::Finalize()
{
}



void Goblin::HitAction(InterSectData data)
{
}



void Goblin::ChangeState(IState* state)
{
}
