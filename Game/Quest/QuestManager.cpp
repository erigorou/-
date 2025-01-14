// ---------------------------------------------
// クエストマネージャークラスの実装
// ---------------------------------------------

#include "pch.h"
#include "QuestManager.h"
#include "../Scene/PlayScene.h"
#include "QuestRenderer/QuestRenderer.h"
#include "Tutorial/Tutorial.h"

#include "Interface/IQuestChecker.h"
#include "QuestList/QuestPlayerMove.h"
#include "QuestList/QuestPlayerAttack.h"
#include "QuestList/QuestPlayerCombo.h"
#include "QuestList/QuestPlayerAvoid.h"

#include "TutorialList/TutorialStart.h"
#include "TutorialList/TutorialEnd.h"

#include "Libraries/MyLib/DebugString.h"


// -----------------------------
// コンストラクタ
// -----------------------------
QuestManager::QuestManager(PlayScene* playScene)
	: m_playScene(playScene)
	, m_currentQuestNo()
	, m_totalTime()
{
}


// -----------------------------
// デストラクタ
// -----------------------------
QuestManager::~QuestManager()
{
}


// -----------------------------
// クエストの初期化
// -----------------------------
void QuestManager::InitializeQuest()
{
	// クエストリストの作成
	CreateQuestList_2nd();

	if (m_questList.size() <= 0) return;

	// クエスト内容の描画オブジェクト
	m_renderer = std::make_unique<QuestRenderer>(this);
	// 初期化処理
	m_renderer->Initialize(m_textureList[0]);
}

// -----------------------------
// 更新処理
// -----------------------------
void QuestManager::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

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
}


// -----------------------------
// クエストの更新
// -----------------------------
void QuestManager::UpdateQuest()
{
	if (m_renderer == nullptr	) return;	// クエストがない場合は終了
	if (m_totalTime < DELAY_TIME) return;	// 最初の遅延時間内なら終了

	if (!(m_currentQuestNo < m_questList.size() - 1))	return;	// クエストが最後まで行っている場合は終了

	// クエストのクリアを描画オブジェクトに通知
	m_renderer->IsClear(m_questList[m_currentQuestNo]->ExecuteChecker(m_playScene));
}


// -----------------------------
// クエストを一つ更新する
// -----------------------------
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
		// クエストクリア

	}
}



// -----------------------------
// クエストの描画
// -----------------------------
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


// -----------------------------
// クエストマネージャーの終了
// -----------------------------
void QuestManager::Finalize()
{
}


// -----------------------------------
// クエストリストの作成 ステージ１
// -----------------------------------
void QuestManager::CreateQuestList_1st()
{
	// クエストデータの消去
	ClearQuestData();

	// クエストの作成
	m_questList.push_back(new TutorialStart		());
	m_questList.push_back(new QuestPlayerMove	());
	m_questList.push_back(new QuestPlayerAttack	());
	m_questList.push_back(new QuestPlayerCombo	());
	m_questList.push_back(new QuestPlayerAvoid	());
	m_questList.push_back(new TutorialEnd		());

	// クエストの中のテクスチャを読み込む
	AddQuestTexture();

	// チュートリアルの作成
	CreateTutorial();
}


// -----------------------------------
// クエストリストの作成 ステージ２
// -----------------------------------

void QuestManager::CreateQuestList_2nd()
{
	// クエストデータの消去
	ClearQuestData();
}


// -----------------------------------
// クエストリストの作成 ステージ３
// -----------------------------------
void QuestManager::CreateQuestList_3rd()
{
	// クエストデータの消去
	ClearQuestData();
}

// -----------------------------------
// クエストのテクスチャを読み込む
// -----------------------------------
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


// -----------------------------------
// チュートリアルのテクスチャを読み込む
// -----------------------------------
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


// -----------------------------------
// チュートリアルの作成
// -----------------------------------
void QuestManager::CreateTutorial()
{
	// チュートリアルのテクスチャを読み込む
	AddTutorialTexture();

	// チュートリアルの作成
	m_tutorial = std::make_unique<Tutorial>(this);
	m_tutorial->Initialize(m_tutorialTextureList[0]);
}



// -----------------------------------
// クエストデータの消去
// -----------------------------------
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
