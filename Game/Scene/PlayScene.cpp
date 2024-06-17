/*
	@file	PlayScene.cpp
	@brief	プレイシーンクラス
*/
#include "pch.h"
#include "PlayScene.h"

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugCamera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

#include <Model.h>
#include <cassert>

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
PlayScene::PlayScene()
	:
	m_commonResources{}
	,m_debugCamera{}
	,m_gridFloor{}
	,m_projection{}
	,m_isChangeScene{}
	,m_player{}
	,m_skySphere{}
	,m_particles{}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
PlayScene::~PlayScene()
{
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void PlayScene::Initialize()
{
	using namespace DirectX;

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// グリッド床を作成する
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);

	// デバッグカメラを作成する
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);

	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 10000.0f
	);

	// TPSカメラを取得する
	m_camera = std::make_unique<Camera>();

	// シーン変更フラグを初期化する
	m_isChangeScene = false;

	// 天球の生成とモデルの読み込み
	m_skySphere = std::make_unique<SkySphere>();
	m_skySphere->LoadSkySphereModel(device);

	// パーティクルの生成と初期化
	m_particles = std::make_unique<Particle>();
	m_particles->Create();

	// プレイヤーの生成と初期化
	m_player = std::make_unique<Player>();
	m_player->Initialize(device, context, states);

	// 鬼の生成と初期化
	m_enemy = std::make_unique<Enemy>();
	m_enemy->Initialize(device, context, states);
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(elapsedTime);

	// プレイヤーの更新処理
	m_player->Update(m_enemy->GetPosition(), elapsedTime);
	// 敵の更新処理
	m_enemy->Update(elapsedTime);
	// カメラの回転行列の作成	引数にはプレイヤーの回転角を入れる
	SimpleMath::Matrix matrix
		= SimpleMath::Matrix::CreateRotationY(
			XMConvertToRadians(m_player->GetAngle()));
	// カメラの更新
	m_camera->Update(m_player->GetPosition(), m_enemy->GetPosition(), matrix);
	// パーティクルの更新
	m_particles->Update(
		elapsedTime,
		m_player->GetPosition(),
		m_player->GetVelocity()
	);
}


//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void PlayScene::Render()
{
	using namespace DirectX;

	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ビュー行列を取得する
	const SimpleMath::Matrix& view = m_camera->GetViewMatrix();
	// 天球の描画
	m_skySphere->DrawSkySphere(context, states, view, m_projection);
	// パーティクルのビルボード作成
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// パーティクルの描画
	m_particles->Render(view, m_projection);
	// 格子床を描画する
	m_gridFloor->Render(context, view, m_projection);
	// プレイヤーの描画を行う
	m_player->Render(context, states, view, m_projection, m_commonResources);
	// 敵の描画を行う
	m_enemy->Render(context, states, view, m_projection);


	// デバッグ情報を「DebugString」で表示する
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("Play Scene");
	debugString->AddString("Play Scene");
	debugString->AddString("Camera Angle : %f", XMConvertToDegrees(m_camera->GetCameraAngle()));
	debugString->AddString("Player Angle : %f", XMConvertToDegrees(m_player->GetAngle()));

}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void PlayScene::Finalize()
{
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}
