// ---------------------------------------------
// 名前:	QuestManager.cpp
// 内容:	クエストマネージャークラス
//			クエストの管理を行う
// 制作:	池田桜輔
// ---------------------------------------------
// インクルード
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "QuestManager.h"
#include "../Scene/PlayScene.h"
#include "../Data/GameData.h"
#include "QuestRenderer/QuestRenderer.h"
#include "Tutorial/Tutorial.h"
#include "Interface/IQuestChecker.h"
#include "QuestList/QuestChangeTarget.h"
#include "QuestList/QuestPlayerMove.h"
#include "QuestList/QuestPlayerAttack.h"
#include "QuestList/QuestPlayerCombo.h"
#include "QuestList/QuestPlayerAvoid.h"
#include "QuestList/TutorialClear.h"
#include "TutorialList/TutorialStart.h"
#include "TutorialList/TutorialEnd.h"
#include "TutorialList/TutorialAttackStart.h"
#include "TutorialList/TutorialDodgStart.h"

// ---------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------
QuestManager::QuestManager(PlayScene* playScene)
	: m_playScene(playScene)
	, m_elapsedTime()
	, m_currentQuestNo()
	, m_totalTime()
	, m_clearTime()
	, m_canClear(false)
{
}

// ---------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------
QuestManager::~QuestManager()
{
}

// ---------------------------------------------
/// <summary>
/// クエストの初期化
/// </summary>
// ---------------------------------------------
void QuestManager::InitializeQuest()
{
	// クエストデータに登録された番号を取得
	int questIndex = GameData::GetInstance()->GetSelectStage();

	// クエストリストの作成
	CreateQuest(questIndex);

	if (m_questList.size() <= 0) return;

	// クエスト内容の描画オブジェクト
	m_renderer = std::make_unique<QuestRenderer>(this);
	// 初期化処理
	m_renderer->Initialize(m_textureList[0]);
}

// ---------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void QuestManager::Update(float elapsedTime)
{
	// 経過時間の記録
	m_totalTime += elapsedTime;
	m_elapsedTime = elapsedTime;

	// クエストの更新
	UpdateQuest();

	// チュートリアルの更新
	if (m_tutorial != nullptr)
	{
		m_tutorial->Update(elapsedTime);
	}

	if (m_renderer != nullptr)
	{
		// クエスト描画の更新
		m_renderer->Update(elapsedTime);
	}

	// クエストのクリア判定
	IsQuestClear();
}

// ---------------------------------------------
/// <summary>
/// クエストの更新処理
/// </summary>
// ---------------------------------------------
void QuestManager::UpdateQuest()
{
	if (m_renderer == nullptr) return;	// クエストがない場合は終了
	if (m_totalTime < DELAY_TIME) return;	// 最初の遅延時間内なら終了

	if ((m_currentQuestNo < m_questList.size()))
	{
		// クエストのクリアを描画オブジェクトに通知
		m_renderer->IsClear(m_questList[m_currentQuestNo]->ExecuteChecker(m_playScene));
	}
}

// ---------------------------------------------
/// <summary>
/// クエストを一つ更新する
/// </summary>
// ---------------------------------------------
void QuestManager::ChangeNextQuest()
{
	// クエストを１つ進める
	m_currentQuestNo++;

	// クエストが最後まで行っていない場合
	if (m_currentQuestNo < m_questList.size())
	{
		// クエストの描画の更新
		m_renderer->ChangeTexture(m_textureList[m_currentQuestNo]);

		// チュートリアルの描画の更新
		if (m_tutorial != nullptr)
		{
			m_tutorial->ChangeTexture(m_tutorialTextureList[m_currentQuestNo]);
		}
	}
	else
	{
		m_canClear = true;
	}
}

// ---------------------------------------------
/// <summary>
/// クエストの描画
/// </summary>
// ---------------------------------------------
void QuestManager::DrawQuest()
{
	if (m_renderer != nullptr)
	{
		// クエストの描画
		m_renderer->Draw();
	}

	// チュートリアルの描画
	if (m_tutorial != nullptr)
	{
		m_tutorial->Draw();
	}
}

// ---------------------------------------------
/// <summary>
/// クエストマネージャーの終了処理
/// </summary>
// ---------------------------------------------
void QuestManager::Finalize()
{
}

// ---------------------------------------------
/// <summary>
/// クエストの作成
/// </summary>
/// <param name="questIndex">クエストのインデックス</param>
// ---------------------------------------------
void QuestManager::CreateQuest(int questIndex)
{
	switch (questIndex)
	{
	case 0:
		CreateQuestList_1st();
		break;

	case 1:
		CreateQuestList_2nd();
		break;

	case 2:
		CreateQuestList_3rd();
		break;

	default:
		MessageBox(nullptr, L"クエストの作成に失敗しました", L"エラー", MB_OK);
		break;
	}
}

// ---------------------------------------------
/// <summary>
/// クエストリストの作成 ステージ１
/// </summary>
// ---------------------------------------------
void QuestManager::CreateQuestList_1st()
{
	// クエストデータの消去
	ClearQuestData();

	// クエストの作成
	m_questList.push_back(new TutorialStart());
	m_questList.push_back(new QuestChangeTarget());
	m_questList.push_back(new QuestPlayerMove());
	m_questList.push_back(new TutorialAttackStart());
	m_questList.push_back(new QuestPlayerAttack());
	m_questList.push_back(new QuestPlayerCombo());
	m_questList.push_back(new TutorialDodgStart());
	m_questList.push_back(new QuestPlayerAvoid());
	m_questList.push_back(new TutorialEnd());

	// クエストの中のテクスチャを読み込む
	AddQuestTexture();

	// チュートリアルの作成
	CreateTutorial();
}

// ---------------------------------------------
/// <summary>
/// クエストリストの作成 ステージ２
/// </summary>
// ---------------------------------------------
void QuestManager::CreateQuestList_2nd()
{
	// クエストデータの消去
	ClearQuestData();

	// クエストの作成
	m_questList.push_back(new TutorialClear());

	// クエストの中のテクスチャを読み込む
	AddQuestTexture();
}

// ---------------------------------------------
/// <summary>
/// クエストリストの作成 ステージ３
/// </summary>
// ---------------------------------------------
void QuestManager::CreateQuestList_3rd()
{
	// クエストデータの消去
	ClearQuestData();
}

// ---------------------------------------------
/// <summary>
/// クエストのテクスチャを読み込む
/// </summary>
// ---------------------------------------------
void QuestManager::AddQuestTexture()
{
	// 設定したクエストから全て抜き出す
	for (auto quest : m_questList)
	{
		if (quest->GetTexturePath() == nullptr)
		{
			// テクスチャリストにnullptrを追加(何も入れないが要素は増やす)
			m_textureList.push_back(nullptr);
			continue;
		}

		// テクスチャの読み込み
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		DX::ThrowIfFailed(
			DirectX::CreateWICTextureFromFile(
				CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice(),
				quest->GetTexturePath(),
				nullptr,
				texture.ReleaseAndGetAddressOf()
			)
		);

		// テクスチャリストに追加
		m_textureList.push_back(texture);
	}
}

// ---------------------------------------------
/// <summary>
/// チュートリアルのテクスチャを読み込む
/// </summary>
// ---------------------------------------------
void QuestManager::AddTutorialTexture()
{
	for (auto tutorial : m_questList)
	{
		if (tutorial->GetTutorialTexturePath() == nullptr)
		{
			// テクスチャリストにnullptrを追加(何も入れないが要素は増やす)
			m_tutorialTextureList.push_back(nullptr);
			continue;
		}

		// チュートリアルのテクスチャを読み込む
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		DX::ThrowIfFailed(
			DirectX::CreateWICTextureFromFile(
				CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice(),
				tutorial->GetTutorialTexturePath(),
				nullptr,
				texture.ReleaseAndGetAddressOf()
			)
		);

		// テクスチャリストに追加
		m_tutorialTextureList.push_back(texture);
	}
}

// ---------------------------------------------
/// <summary>
/// チュートリアルの作成
/// </summary>
// ---------------------------------------------
void QuestManager::CreateTutorial()
{
	// チュートリアルのテクスチャを読み込む
	AddTutorialTexture();

	// チュートリアルの作成
	m_tutorial = std::make_unique<Tutorial>(this);
	m_tutorial->Initialize(m_tutorialTextureList[0]);
}

// ---------------------------------------------
/// <summary>
/// クエストデータの消去
/// </summary>
// ---------------------------------------------
void QuestManager::ClearQuestData()
{
	// クエストリストを全て削除する
	for (auto quest : m_questList)
	{
		delete quest;
	}

	// 画像リストをすべて削除する
	for (auto texture : m_textureList)
	{
		texture.Reset();
	}

	// クエストリストとテクスチャリストをクリアする
	m_questList.clear();
	m_textureList.clear();
}

// ---------------------------------------------
/// <summary>
/// クエストのクリア判定
/// </summary>
// ---------------------------------------------
void QuestManager::IsQuestClear()
{
	// ゲームクリアが可能な場合
	if (m_canClear)
	{
		// 一定時間経過後にゲームクリア
		m_clearTime += m_elapsedTime;

		if (m_clearTime > CLEAR_DELAY_TIME)
		{
			// クリアシーンに遷移
			m_playScene->ChangeResultScene();
		}
	}
}
