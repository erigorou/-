// -----------------------------------------------------------------
// 
// 名前: Wall.cpp
// 内容: ステージの境界壁を描画するクラス
// 作成: 池田桜輔
// 
// -----------------------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "Wall.h"
#include "Game/Messenger/EventMessenger.h"
#include "Libraries/MyLib/ThreadedRenderer/ThreadedRenderer.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
Wall::Wall()
	:
	m_worldMatrix{},
	m_model{}
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
Wall::~Wall()
{
	Finalize();
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void Wall::Initialize()
{
	// モデルを取得する
	m_model = GameResources::GetInstance()->GetModel("wall");

	// 当たり判定の生成
	CreateCollision();

	// マルチスレッドに自身を登録
	auto threadedRenderer = ThreadedRenderer::GetInstance();
	threadedRenderer->RegisterRenderable(this);
}

// ---------------------------------------------------------
/// <summary>
/// 当たり判定の生成
/// </summary>
// ---------------------------------------------------------
void Wall::CreateCollision()
{
	m_collision = std::make_unique<DirectX::BoundingSphere>(DirectX::SimpleMath::Vector3::Zero, COLLISION_RADIUS);
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
	EventMessenger::Execute(EventList::AddSphereCollision, &data);
	EventMessenger::Execute(EventList::AddSphereCollision, &overData);
}

// ---------------------------------------------------------
/// <summary>
/// ワールド座標更新処理
/// </summary>
// ---------------------------------------------------------
void Wall::UpdateWorldMatrix()
{
	// 初期化
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	// 大きさの変更
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(WALL_SCALE);
	// 移動
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(WALL_POS);
}

// ---------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ---------------------------------------------------------
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

/// <summary>
/// 描画コマンド登録
/// </summary>
/// <param name="view">ビューポート</param>
/// <param name="proj">プロジェクション行列</param>
/// <param name="defferedConetxt">ディファードコンテキスト</param>
void Wall::RecordRenderCommands(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* deferredContext)
{
	// ワールド行列の更新処理
	UpdateWorldMatrix();

	CommonResources* resources = CommonResources::GetInstance();
	auto states = resources->GetCommonStates();

	// RenderTargetViewを取得
	auto rtv = resources->GetDeviceResources()->GetRenderTargetView();
	// デプスステンシルビューを取得
	auto dsv = resources->GetDeviceResources()->GetDepthStencilView();
	// ビューポートを取得
	auto viewport = resources->GetDeviceResources()->GetScreenViewport();

	// レンダーターゲットを設定
	deferredContext->OMSetRenderTargets(1, &rtv, dsv);
	// ビューポートを設定
	deferredContext->RSSetViewports(1, &viewport);

	// 描画コマンドを登録する
	m_model->Draw(deferredContext, *states, m_worldMatrix, view, proj);
}


// ---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void Wall::Finalize()
{
	// 衝突判定をManagerから削除
	EventMessenger::Execute(EventList::DeleteCollision, this);
	EventMessenger::Execute(EventList::DeleteCollision, this);

	// 描画コマンドの削除
	auto threadedRenderer = ThreadedRenderer::GetInstance();
	threadedRenderer->UnregisterRenderable(this);
}

// ---------------------------------------------------------
/// <summary>
/// 衝突処理
/// </summary>
// ---------------------------------------------------------
void Wall::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}