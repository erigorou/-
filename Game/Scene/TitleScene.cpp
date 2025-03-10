// --------------------------------------------------
// 
// 名前:	TitleScene.h
// 内容:	タイトルシーンを描画するクラス
// 作成:	池田桜輔
// 
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "TitleScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "../Factory/Factory.h"
#include "../Sound/Sound.h"
#include "../Data/GameData.h"
#include "Libraries/MyLib/Math.h"
#include "Effects/Particle.h"
#include "Game/UI/SceneUIManager/QuestSelectSceneUIManager.h"
#include "Game/Messenger/EventMessenger.h"
#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include "../Stage/Sea/Sea.h"
#include "../TitleObject/TitleEnemy.h"
#include "../Stage/SkySphere/SkySphere.h"

#include "Game/Messenger/KeyboardMessenger.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
TitleScene::TitleScene()
	:
	m_totalSeconds{},
	m_isChangeScene{},
	m_selectIndex{},
	m_shakePower{ SHAKE_POWER }
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
TitleScene::~TitleScene()
{
	Finalize();
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
void TitleScene::Initialize()
{
	// キー入力を登録
	AttachKeyInput();
	// オブジェクトを生成する
	CreateObjects();
	// 射影行列を生成する
	CreateProjection();
	// シーン変更フラグを初期化する
	m_isChangeScene = false;
	// 音楽の変更
	Sound::ChangeBGM(Sound::BGM_TYPE::TITLE);
}

// ---------------------------------------------------------
/// <summary>
/// オブジェクト生成処理
/// </summary>
// ---------------------------------------------------------
void TitleScene::CreateObjects()
{
	// カメラの生成
	m_camera = Factory::CreateCamera();
	// 床の生成
	m_floor = Factory::CreateFloor();
	// 海の生成
	m_sea = Factory::CreateSea();
	// 天球の生成
	m_skySphere = Factory::CreateSkySphere();

	// 敵の生成
	m_enemy = std::make_unique<TitleEnemy>();
	m_enemy->Initialize();

	// パーティクルの生成
	m_particle = Factory::CreateParticle();

	// カメラの状態を変更
	CameraState state = CameraState::Title;
	EventMessenger::Execute(EventList::ChangeCamera, &state);

	// UIの生成
	m_uiManager = std::make_unique<QuestSelectSceneUIManager>();
	m_uiManager->Initialize();
}


void TitleScene::AttachKeyInput()
{
	KeyboardMessenger::Attach(DirectX::Keyboard::Up, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Down, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Space, this, KeyboardMessenger::KeyPressType::PRESSED);
}

// ---------------------------------------------------------
/// <summary>
/// 射影行列生成処理
/// </summary>
// ---------------------------------------------------------
void TitleScene::CreateProjection()
{
	// ウィンドウのサイズを取得する
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };

	// 射影行列を作成する
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(FOV),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		NEAR_Z, FAR_Z
	);
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void TitleScene::Update(float elapsedTime)
{
	// 秒数を加算する
	m_totalSeconds += elapsedTime;

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = CommonResources::GetInstance()->GetInputManager()->GetKeyboardTracker();
	// オブジェクトの更新
	UpdateObject(elapsedTime);
	// パーティクルの更新
	m_particle->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	// ステージ選択
	SelectStage(kbTracker.get());
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void TitleScene::UpdateObject(const float elapsedTime)
{
	// ゼロベクトル
	DirectX::SimpleMath::Vector3 zeroV = DirectX::SimpleMath::Vector3::Zero;

	// オブジェクトの更新
	m_camera->Shake(elapsedTime);
	m_camera->Update(zeroV, zeroV, elapsedTime);
	m_enemy->Update(elapsedTime);

	// UIの更新
	m_uiManager->Update(elapsedTime);
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void TitleScene::SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard)
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

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void TitleScene::Render()
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

// ---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void TitleScene::Finalize()
{
	// オブジェクトの終了処理
	m_camera.reset();
	m_floor.reset();
	m_sea.reset();
	m_enemy.reset();
	m_skySphere.reset();
	m_particle.reset();
	m_uiManager.reset();
}

// ---------------------------------------------------------
/// <summary>
/// 次のシーンID取得処理
/// </summary>
/// <returns>次のシーンID</returns>
// ---------------------------------------------------------
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

void TitleScene::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
}

void TitleScene::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
}
