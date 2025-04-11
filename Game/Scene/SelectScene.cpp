// --------------------------------------------------
// 
// 名前:	SelectScene.cpp
// 内容:	セレクトシーンを描画するクラス
// 作成:	池田桜輔
// 
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "SelectScene.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Game/UI/UserInterface.h"
#include "../Sound/Sound.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"
#include "Game/Messenger/KeyboardMessenger.h"
#include "Libraries/MyLib/Math.h"
#include "Game/UI/Action/NormalAction.h"
#include "Game/Factory/Factory.h"
#include "Game/Camera/Camera.h"
// ステージ関連
#include "Game/Stage/Sea/Sea.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Stage/Floor/Floor.h"
#include "Game/Stage/SkySphere/SkySphere.h"

// ----------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
//-----------------------------------------------
SelectScene::SelectScene()
	: 
	m_spriteBatch(),
	m_isChangeScene(),
	m_selectIndex()
{
}

// ----------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ----------------------------------------------
SelectScene::~SelectScene()
{
	Finalize();
}

// ----------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ----------------------------------------------
void SelectScene::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// キーボードの登録
	RegisterKeyboard();
	// SelectSceneのUI追加
	CreateUI();
	// ステージの作成
	CreateStage();
}

// ----------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// -----------------------------------------------
void SelectScene::Update(float elapsedTime)
{
	// カメラの更新
	m_camera->Update(CAMERA_POSITION, CAMERA_DIRECTION, elapsedTime);

	// UIの更新
	for (auto& ui : m_uiList)
	{
		ui->Update(elapsedTime);
	}
}

// ----------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ----------------------------------------------
void SelectScene::Render()
{
	// ステージの描画
	RenderStage();

	// コモンステートを取得する
	auto states = CommonResources::GetInstance()->GetCommonStates();
	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
	// スプライトバッチの終わり
	m_spriteBatch->End();
	// UIの描画
	for (auto& ui : m_uiList)
	{
		ui->Render();
	}
}

// ----------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ----------------------------------------------
void SelectScene::Finalize()
{
	// UIの削除
	m_uiList.clear();
	// スプライトバッチの削除
	m_spriteBatch.reset();
	// キーボードの登録解除
	KeyboardMessenger::Clear();
}

// -------------------------------------------------
/// <summary>
/// SelectScene内のUIを登録する
/// </summary>
// -------------------------------------------------
void SelectScene::CreateUI()
{
	//// 背景の追加
	//AddUserInterface(
	//	"SelectBack",
	//	BACKGROUND_POSITION,
	//	BACKGROUND_SIZE,
	//	ANCHOR::TOP_LEFT,
	//	new NormalAction()
	//);

	// EspaceUIの追加
	AddUserInterface(
		"SelectEscape",
		ESPACE_POSITION,
		ESPACE_SIZE,
		ANCHOR::MIDDLE_CENTER,
		new NormalAction()
	);

	// SpaceUIの追加
	AddUserInterface(
		"SelectSpace",
		SPACE_POSITION,
		SPACE_SIZE,
		ANCHOR::MIDDLE_CENTER,
		new NormalAction()
	);

	// ステージ選択UI（大見出し）の追加
	AddUserInterface(
		"StageSelect",
		LOGO_POSITION,
		LOGO_SIZE,
		ANCHOR::TOP_LEFT,
		new NormalAction()
	);

	// チュートリアルUIの追加
	AddUserInterface(
		"SelectTutorial",
		TUTORIAL_POSITION,
		SELECT_STAGE_UI_SIZE,
		ANCHOR::MIDDLE_LEFT,
		new NormalAction()
	);

	// ボス戦UIの追加
	AddUserInterface(
		"SelectBoss",
		BOSS_POSITION,
		SELECT_STAGE_UI_SIZE,
		ANCHOR::MIDDLE_LEFT,
		new NormalAction()
	);
}

// -----------------------------------------------
/// <summary>
/// ステージを作成する
/// </summary>
// -----------------------------------------------
void SelectScene::CreateStage()
{
	// 衝突判定の作成
	m_collisionManager = Factory::CreateCollisionManager();
	// カメラの作成
	m_camera = Factory::CreateCamera();

	// カメラを作成する
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };
	// 射影行列を作成する
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(FOV),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		NEAR_Z, FAR_Z);

	// カメラのステート変更
	CameraState playState = CameraState::Select;
	m_camera->ChangeState(&playState);

	// 海の作成
	m_sea = Factory::CreateSea();
	// 壁の作成
	m_wall = Factory::CreateWall();
	// 地面の作成
	m_floor = Factory::CreateFloor();
	// スカイボックスの作成
	m_skySphere = Factory::CreateSkySphere();
}

// -----------------------------------------------
/// <summary>
/// ステージの描画
/// </summary>
// ------------------------------------------------
void SelectScene::RenderStage()
{
	// カメラの必要情報を取得
	auto view = m_camera->GetViewMatrix();

	// 海の描画
//	m_sea->Render(view, m_projection);
	// 壁の描画
//	m_wall->Render(view, m_projection);
	// 地面の描画
//	m_floor->Render(view, m_projection);
	// スカイボックスの描画
//	m_skySphere->DrawSkySphere(view, m_projection);
}


// -----------------------------------------------
/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns>次のシーンID</returns>
// -----------------------------------------------
IScene::SceneID SelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

// -----------------------------------------------
/// <summary>
/// Escapeが押された場合のシーンIDを取得する
/// </summary>
/// <returns>変更後のシーンID</returns>
// ---------------------------------
IScene::SceneID SelectScene::GetPrevSceneID() const
{
	// シーン変更がない場合
	return IScene::SceneID::TITLE;
}


// ----------------------------------------------
/// <summary>
/// 使用するキーボードを登録する
/// </summary>
// -----------------------------------------------
void SelectScene::RegisterKeyboard()
{
	// キー入力の登録
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Up, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Down, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Space, this, KeyboardMessenger::KeyPressType::PRESSED);
}


// ----------------------------------------------
/// <summary>
/// キー入力（押された瞬間）
/// </summary>
/// <param name="key">キー</param>
// -----------------------------------------------
void SelectScene::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// シーン変更中は処理しない
	if (m_isChangeScene) return;

	// スペースキーが押されたら
	if (key == DirectX::Keyboard::Keys::Space){
		OnKeyDownSpace();
	}
	// 上キーが押されたら
	if (key == DirectX::Keyboard::Keys::Up){
		OnKeyDownArrowUp();
	}
	// 下キーが押されたら
	if (key == DirectX::Keyboard::Keys::Down){
		OnKeyDownArrowDown();
	}
}

// ----------------------------------------------
/// <summary>
/// キー入力（押されている間）
/// </summary>
/// <param name="key">キー</param>
// -----------------------------------------------
void SelectScene::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ----------------------------------------------
/// <summary>
// 上キーが押されたときの処理
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownArrowUp()
{
	// 選択インデックスを減らす
	m_selectIndex = Math::Clamp(m_selectIndex - 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);

	// ゲームデータにステージを設定
	auto gameData = GameData::GetInstance();
	gameData->SetSelectStage(m_selectIndex);
}

// ----------------------------------------------
/// <summary>
// 下キーが押されたときの処理
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownArrowDown()
{
	// 選択インデックスを増やす
	m_selectIndex = Math::Clamp(m_selectIndex + 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);

	// ゲームデータにステージを設定
	auto gameData = GameData::GetInstance();
	gameData->SetSelectStage(m_selectIndex);
}

// ----------------------------------------------
/// <summary>
// スペースキーが押されたときの処理
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownSpace()
{
	// スペースキーが押されたときの処理
	m_isChangeScene = true;
}

// ---------------------------------------------------------
/// <summary>
/// ユーザーインターフェースの追加
/// </summary>
/// <param name="textureName">テクスチャ名</param>
/// <param name="position">位置</param>
/// <param name="scale">スケール</param>
/// <param name="anchor">アンカー</param>
/// <param name="action">アクション</param>
// ---------------------------------------------------------
void SelectScene::AddUserInterface(
	const std::string& textureName,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	const ANCHOR& anchor,
	IAction* action
)
{
	// テクスチャの取得
	auto texture = GameResources::GetInstance()->GetTexture(textureName);
	// ユーザーインターフェースの生成
	auto ui = std::make_unique<UserInterface>();
	// ユーザーインターフェースの作成
	ui->Create(texture, position, scale, anchor, action);
	// ユーザーインターフェースの登録
	m_uiList.push_back(std::move(ui));
}
