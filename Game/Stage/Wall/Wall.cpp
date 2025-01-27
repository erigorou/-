// -----------------------------------------------------------------
// * 天球によって違和感が出ないようにする為のステージ
// -----------------------------------------------------------------
#include "pch.h"
#include <Model.h>
#include <cassert>
// ヘッダー
#include "Game/CommonResources.h"	
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "Wall.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Messenger/EventMessenger.h"

// 固定値
const float Wall::WALL_SCALE = 2.0f;

// コンストラクタ
Wall::Wall(PlayScene* playScene)
	: m_playScene(playScene)
	, m_worldMatrix()
	, m_model()
{
}

// デストラクタ
Wall::~Wall()
{
}

// 初期化処理
void Wall::Initialize()
{
	// モデルを取得する
	m_model = GameResources::GetInstance()->GetModel("wall");

	// 当たり判定の生成
	CreateCollision();
}

// 当たり判定の生成
void Wall::CreateCollision()
{
	m_collision		= std::make_unique<DirectX::BoundingSphere>(DirectX::SimpleMath::Vector3::Zero, COLLISION_RADIUS);
	m_overCollision = std::make_unique<DirectX::BoundingSphere>(DirectX::SimpleMath::Vector3::Zero, COLLISION_RADIUS * COLLISION_SCALE);


	// 衝突データの作成
	CollisionData<DirectX::BoundingSphere>data =
	{
		ObjectType::Stage,		// オブジェクトの種類
		CollisionType::Sphere,	// 当たり判定の種類
		this,					// このクラスのポインタ
		m_collision.get()		// 当たり判定のポインタ
	};


	// 衝突データの作成
	CollisionData<DirectX::BoundingSphere>overData =
	{
		ObjectType::Stage,		// オブジェクトの種類
		CollisionType::Sphere,	// 当たり判定の種類
		this,					// このクラスのポインタ
		m_overCollision.get()	// 当たり判定のポインタ
	};


	// 衝突判定をManagerに登録
	EventMessenger::Execute("AddSphereCollision", &data);
	EventMessenger::Execute("AddSphereCollision", &overData);
}


// ワールド座標更新処理
void Wall::UpdateWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_worldMatrix = Matrix::Identity;						// 初期化
	m_worldMatrix *= Matrix::CreateScale(WALL_SCALE);		// 拡大　縮小
	m_worldMatrix *= Matrix::CreateTranslation(WALL_POS);	// 移動
}



// 描画
void Wall::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ワールド行列の更新処理
	UpdateWorldMatrix();
	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// 終了処理
void Wall::Finalize()
{
}



void Wall::HitAction(InterSectData data)			
{
	UNREFERENCED_PARAMETER(data);
}