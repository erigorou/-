// ---------------------------------------------
// クエストマネージャークラスの実装
// ---------------------------------------------

#include "pch.h"
#include "QuestManager.h"
#include "../Scene/PlayScene.h"

#include "Interface/IQuestChecker.h"
#include "QuestList/QuestPlayerMove.h"
#include "QuestList/QuestPlayerAttack.h"
#include "QuestList/QuestPlayerCombo.h"
#include "QuestList/QuestPlayerAvoid.h"


// -----------------------------
// コンストラクタ
// -----------------------------
QuestManager::QuestManager(PlayScene* playScene)
	: m_playScene(playScene)
	, m_currentQuestNo(0)	// ★
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
	CreateQuestList_1st();
}

// -----------------------------
// 更新処理
// -----------------------------
void QuestManager::Update(float elapsedTime)
{
	// クエストの更新
	UpdateQuest();
}


// -----------------------------
// クエストの更新
// -----------------------------
void QuestManager::UpdateQuest()
{
	// クエストの更新
	if (m_currentQuestNo < m_questList.size())
	{
		// クエストの実行
		auto quest = m_questList[m_currentQuestNo];
		if (quest->ExecuteChecker(m_playScene))
		{
			// 次のクエストへ
			m_currentQuestNo++;
		}
	}
}


// -----------------------------
// エフェクトの更新処理
// -----------------------------
void QuestManager::UpdateEffect(float elapsedTime)
{
}



// -----------------------------
// クエストの描画
// -----------------------------
void QuestManager::DrawQuest()
{

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
	m_questList.push_back(new QuestPlayerMove());
	m_questList.push_back(new QuestPlayerAttack());
	m_questList.push_back(new QuestPlayerCombo());
	m_questList.push_back(new QuestPlayerAvoid());
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
// クエストデータの消去
// -----------------------------------
void QuestManager::ClearQuestData()
{
	// クエストリストを全て削除する
	for (auto quest : m_questList)
	{
		delete quest;
	}

	// クエストリストをクリア
	m_questList.clear();
}
