// ------------------------------------------------------------------------------
// 
// 名前:	PlayScene
// 内容:	プレイシーンのオブジェクトを生成し、更新、描画を行う
// 制作:	池田桜輔
// 
// ------------------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "PlayScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/Factory/Factory.h"
#include "Game/Sound/Sound.h"
#include "Game/HitStop/HitStop.h"
#include "Libraries/MyLib/Collision/CollisionManager.h"
#include "Game/Data/GameData.h"
#include "Game/Messenger/KeyboardMessenger.h"
#include "Game/Quest/QuestManager.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/EnemyManager/EnemyManager.h"
#include "Game/Player/Player.h"
#include "Game/Boss/Boss.h"
#include "Game/Stage/Floor/Floor.h"
#include "Game/Stage/Sea/Sea.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Stage/SkySphere/SkySphere.h"
#include "Game/UI/!PlaySceneUIManager/PlaySceneUIManager.h"
#include "Effects/Particle.h"
#include "Game/Camera/Camera.h"

// ------------------------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ------------------------------------------------------------------------------
PlayScene::PlayScene()
	: 
	m_projection{},
	m_isChangeScene{ false },
	m_isScreenShot{ false },
	m_smoothDeltaTime{}
{
	GameData::GetInstance()->SetBattleResult(GameData::BATTLE_RESULT::NONE);
}

// ------------------------------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ------------------------------------------------------------------------------
PlayScene::~PlayScene()
{
}

// ------------------------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ------------------------------------------------------------------------------
void PlayScene::Initialize()
{
	// デバッグカメラを作成する
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };

	// 射影行列を作成する
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(FOV),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		NEAR_Z, FAR_Z);

	// オブジェクトの生成
	CreateObjects();

	// イベントを登録する (シーン終了)
	EventMessenger::Attach(EventList::EndPlayScene, std::bind(&PlayScene::ChangeResultScene, this));
	// イベントを登録する (スクリーンショット)
	EventMessenger::Attach(EventList::TakeCapture, std::bind(&PlayScene::TakeCapture, this));
}

// ------------------------------------------------------------------------------
/// <summary>
/// オブジェクトの生成
/// </summary>
// ------------------------------------------------------------------------------
void PlayScene::CreateObjects()
{
	// 火っとストップを取得
	m_hitStop = HitStop::GetInstance();
	// パーティクルを生成
	m_collisionManager = Factory::CreateCollisionManager();
	// カメラを生成
	m_camera = Factory::CreateCamera();
	// パーティクルを生成
	m_particles = Factory::CreateParticle();
	// 天球を生成
	m_skySphere = Factory::CreateSkySphere();
	// 床を生成
	m_floor = Factory::CreateFloor();
	// 海を生成
	m_sea = Factory::CreateSea();
	// 壁を生成
	m_wall = Factory::CreateWall();
	// プレイヤーを生成
	m_player = Factory::CreatePlayer();

	// 敵マネージャー
	m_enemyManager = Factory::CreateEnemyManager();
	// クエストマネージャー
	m_questManager = Factory::CreateQuestManager(this);

	// UIマネージャーを生成
	m_uiManager = Factory::CreateUIManager(this);

	// カメラの状態を変更
	CameraState state = CameraState::Play;
	EventMessenger::Execute(EventList::ChangeCamera, &state);

	// BGM変更
	Sound::GetInstance()->ChangeBGM(Sound::BGM_TYPE::PLAY);
}

// ------------------------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ------------------------------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	//// キーボードの更新処理
	//UpdateKeyboard();
	// オブジェクトの更新処理
	UpdateObjects(elapsedTime);
}

// ------------------------------------------------------------------------------
/// <summary>
/// オブジェクトの更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ------------------------------------------------------------------------------
void PlayScene::UpdateObjects(float elapsedTime)
{
	// ヒットストップの更新
	m_hitStop->Update(elapsedTime);
	// ヒットストップの残り時間を取得
	float smoothDeltaTime = m_hitStop->GetSmoothDeltaTime();

	// UIマネジャーの更新
	m_uiManager->Update(elapsedTime);
	// プレイヤーの更新
	m_player->Update(smoothDeltaTime);
	// 敵マネージャーの更新
	m_enemyManager->Update(smoothDeltaTime);
	// クエストマネージャーの更新
	m_questManager->Update(elapsedTime);
	// カメラの更新
	UpdateCamera(elapsedTime);

	// パーティクルの更新
	m_particles->Update(elapsedTime, m_player->GetPosition(), m_player->GetVelocity());

	// 衝突判定の更新処理
	m_collisionManager->Update();
	// ゲームオーバー判定
	GameOverChacker();
}

// ------------------------------------------------------------------------------
/// <summary>
/// プレイシーンのオブジェクトの描画処理
/// </summary>
// ------------------------------------------------------------------------------
void PlayScene::Render()
{
	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& view = m_camera->GetViewMatrix();
	// 当たり判定を描画
	m_collisionManager->Render(view, m_projection);
	// 空の描画
	m_skySphere->DrawSkySphere(view, m_projection);
	// 地面の描画
	m_floor->Render(view, m_projection);
	// 海の描画
	m_sea->Render(view, m_projection);
	// 壁の描画
	m_wall->Render(view, m_projection);
	// プレイヤーの描画
	m_player->Render(view, m_projection);
	// 敵（複数）の描画
	m_enemyManager->Render(view, m_projection);

	if (m_isScreenShot)
	{
		// スクリーンショットを取る
		TakeScreenShot();
	}

	// パーティクルの描画
	DrawParticle(view, m_projection);
	// クエストの描画
	m_questManager->DrawQuest();
	// UIの描画
	m_uiManager->Render();
}

// ------------------------------------------------------------------------------
/// <summary>
/// パーティクルの描画
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
// ------------------------------------------------------------------------------
void PlayScene::DrawParticle(const DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix projection)
{
	// ビルボード行列の計算
	m_particles->CreateBillboard(m_camera->GetTargetPosition(), m_camera->GetEyePosition(), DirectX::SimpleMath::Vector3::Up);
	// パーティクルの描画
	m_particles->Render(view, projection);
}

// ------------------------------------------------------------------------------
/// <summary>
/// 終了関数
/// </summary>
// ------------------------------------------------------------------------------
void PlayScene::Finalize()
{
}

// ------------------------------------------------------------------------------
/// <summary>
/// カメラの更新処理
/// 
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ------------------------------------------------------------------------------
void PlayScene::UpdateCamera(float elapsedTime)
{
	// 敵ターゲットの座標を取得
	DirectX::SimpleMath::Vector3 targetPos = 
		*(DirectX::SimpleMath::Vector3*)EventMessenger::ExecuteGetter(GetterList::GetTargetPosition);

	// カメラの更新
	m_camera->Update(m_player->GetPosition(),targetPos, elapsedTime);
}

// ------------------------------------------------------------------------------
/// <summary>
/// ボスのポインタを取得
/// </summary>
/// <returns>ボスポインタ</returns>
// ------------------------------------------------------------------------------
Boss* PlayScene::GetBoss()
{
	return m_enemyManager->GetBossEnemy();
}


// ------------------------------------------------------------------------------
/// <summary>
/// 次のシーンのIDを取得
/// </summary>
/// <returns>シーン</returns>
// ------------------------------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		switch (GameData::GetInstance()->GetBattleResult())
		{
			// 勝利したばあいはリザルトに遷移
		case GameData::BATTLE_RESULT::WIN:
			return IScene::SceneID::RESULT;
			break;

			// 敗北した場合はそのままタイトルに遷移
		case GameData::BATTLE_RESULT::LOSE:
			return IScene::SceneID::TITLE;
			break;
		}
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

// ------------------------------------------------------------------------------
/// <summary>
/// リザルトに遷移する
/// </summary>
// ------------------------------------------------------------------------------
void PlayScene::ChangeResultScene()
{
	// 勝敗を登録する
	auto data = GameData::GetInstance();
	data->SetBattleResult(GameData::BATTLE_RESULT::WIN);

	// シーン遷移フラグを立てる
	m_isChangeScene = true;
}

// ------------------------------------------------------------------------------
/// <summary>
/// ゲームオーバーを判定する
/// </summary>
// ------------------------------------------------------------------------------
void PlayScene::GameOverChacker()
{
	// プレイヤーが死亡
	if (m_player->GetPlayerHP()->GetHP() <= 0)
	{
		// 勝敗を登録する
		auto data = GameData::GetInstance();
		data->SetBattleResult(GameData::BATTLE_RESULT::LOSE);

		// シーン遷移フラグを立てる
		m_isChangeScene = true;
	}
}


// ------------------------------------------------------------------------------
/// <summary>
/// スクリーンショットを取る(テクスチャとして保存する)
/// </summary>
// ------------------------------------------------------------------------------
void PlayScene::TakeScreenShot()
{
	// すでにスクリーンショットを取っている場合は何もしない
	if (m_captureSRV != nullptr) return;

	// デバイスリソースの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources();
	ID3D11Device* d3dDevice = device->GetD3DDevice();
	ID3D11DeviceContext* context = device->GetD3DDeviceContext();

	// 現在のレンダーターゲットビューを取得
	ID3D11RenderTargetView* currentRTV = device->GetRenderTargetView();
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	currentRTV->GetResource(&backBuffer);

	// バックバッファテクスチャを取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferTex;
	backBuffer.As(&backBufferTex);

	// バックバッファのテクスチャ設定を取得
	D3D11_TEXTURE2D_DESC desc = {};
	backBufferTex->GetDesc(&desc);

	// GPU 用の DEFAULT テクスチャを作成
	desc.Usage = D3D11_USAGE_DEFAULT; // GPU で使用可能にする
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // シェーダーリソースとしてバインド
	desc.CPUAccessFlags = 0; // CPU からのアクセスは不要
	Microsoft::WRL::ComPtr<ID3D11Texture2D> gpuTexture;
	HRESULT hr = d3dDevice->CreateTexture2D(&desc, nullptr, &gpuTexture);
	if (FAILED(hr)) return;

	// CPU 読み取り用の STAGING テクスチャを作成
	desc.Usage = D3D11_USAGE_STAGING; // CPU 読み取り専用
	desc.BindFlags = 0; // バインド不要
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU で読み取り可能に設定
	Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
	hr = d3dDevice->CreateTexture2D(&desc, nullptr, &stagingTexture);
	if (FAILED(hr)) return;

	// GPU のレンダーターゲットから STAGING テクスチャにデータをコピー
	context->CopyResource(stagingTexture.Get(), backBufferTex.Get());

	// STAGING テクスチャから DEFAULT テクスチャに再コピー
	context->CopyResource(gpuTexture.Get(), stagingTexture.Get());

	// シェーダーリソースビューを作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format; // フォーマットを設定
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; // 2D テクスチャとして設定
	srvDesc.Texture2D.MipLevels = 1; // ミップマップレベル数を指定
	srvDesc.Texture2D.MostDetailedMip = 0; // 最初のミップマップレベルを指定

	hr = d3dDevice->CreateShaderResourceView(gpuTexture.Get(), &srvDesc, &m_captureSRV);
	if (FAILED(hr)) return;

	// スクリーンショットを保存
	GameData::GetInstance()->SetScreenShot(m_captureSRV);
}
