/*
	@file	QuestSelectScene.cpp
	@brief	タイトルシーンクラス
*/
#include "pch.h"
#include "QuestSelectScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "../Factory/Factory.h"
#include "../Sound/Sound.h"
#include "../Data/GameData.h"
#include "Libraries/MyLib/Math.h"
#include "Effects/Particle.h"
#include "Game/UI/SceneUIManager/QuestSelectSceneUIManager.h"

#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include "../Stage/Sea/Sea.h"
#include "../TitleObject/TitleEnemy.h"
#include "Libraries/MyLib/SkySphere.h"


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
QuestSelectScene::QuestSelectScene()
	:
	m_totalSeconds{},
	m_isChangeScene{},
	m_selectIndex{},
	m_shakePower{ 1.0f }
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
QuestSelectScene::~QuestSelectScene()
{
	// do nothing.
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void QuestSelectScene::Initialize()
{
	// オブジェクトを生成する
	CreateObjects();
	// 射影行列を生成する
	CreateProjection();
	// シーン変更フラグを初期化する
	m_isChangeScene = false;
	// 音楽の変更
	Sound::ChangeBGM(Sound::BGM_TYPE::TITLE);
}

//---------------------------------------------------------
// オブジェクトの生成
//---------------------------------------------------------
void QuestSelectScene::CreateObjects()
{
	m_camera = Factory::CreateCamera();
	m_floor = Factory::CreateFloor();
	m_sea = Factory::CreateSea();
	m_skySphere = Factory::CreateSkySphere();

	m_enemy = std::make_unique<TitleEnemy>();
	m_enemy->Initialize();
	m_skySphere->LoadSkySphereModel();

	// タイトルシーンのカメラステートを設定
	m_camera->ChangeState(CameraState::Title);

	m_particle = Factory::CreateParticle();

	m_uiManager = std::make_unique<QuestSelectSceneUIManager>();
	m_uiManager->Initialize();
}

//---------------------------------------------------------
// 射影行列の生成
//---------------------------------------------------------
void QuestSelectScene::CreateProjection()
{
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// 射影行列を作成する
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(40.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void QuestSelectScene::Update(float elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// オブジェクトの更新
	UpdateObject(elapsedTime);
	// パーティクルの更新
	m_particle->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	// ステージ選択
	SelectStage(kbTracker.get());
}

//---------------------------------------------------------
// オブジェクトの更新
//---------------------------------------------------------
void QuestSelectScene::UpdateObject(const float elapsedTime)
{
	DirectX::SimpleMath::Vector3 zeroV = DirectX::SimpleMath::Vector3::Zero;

	// オブジェクトの更新
	m_camera->Shake(elapsedTime);
	m_camera->Update(zeroV, zeroV, elapsedTime);
	m_enemy->Update(elapsedTime);

	// UIの更新
	m_uiManager->Update(elapsedTime);
}

//---------------------------------------------------------
// ステージ選択
//---------------------------------------------------------
void QuestSelectScene::SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard)
{
	// シーン変更中は処理しない
	if (m_isChangeScene) return;

	// スペースキーが押されたら
	if (keyboard->pressed.Space)
	{
		// シーン変更フラグを立てるのと音楽を鳴らす
		m_isChangeScene = true;
		Sound::PlaySE(Sound::SE_TYPE::SYSTEM_OK);
	}

	// ステージ選択
	if (keyboard->pressed.Up)	m_selectIndex = Math::Clamp(m_selectIndex - 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);
	if (keyboard->pressed.Down)	m_selectIndex = Math::Clamp(m_selectIndex + 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);

	// ゲームデータにステージを設定
	auto gameData = GameData::GetInstance();
	gameData->SetSelectStage(m_selectIndex);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void QuestSelectScene::Render()
{
	// ビュー行列を取得する
	auto view = m_camera->GetViewMatrix();
	// 床の描画
	m_floor->Render(view, m_projection);
	// 海の描画
	m_sea->Render(view, m_projection);
	// 敵の描画
	m_enemy->Render(view, m_projection);
	// 天球の描画
	m_skySphere->DrawSkySphere(view, m_projection);
	// パーティクルの描画
	m_particle->CreateBillboard(m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Zero, m_camera->GetUpVector());
	m_particle->Render(view, m_projection);
	// UIの描画
	m_uiManager->Render();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void QuestSelectScene::Finalize()
{
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID QuestSelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}