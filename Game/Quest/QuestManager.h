// ---------------------------------------------------
// ステージクリアのクエストを管理するクラス
// ---------------------------------------------------

#pragma once
#include "pch.h"

class PlayScene;
class IQuestChecker;

class QuestManager
{
// ---------------
// 固定値
// ---------------
public:
	// ステージの最大数
	static constexpr int MAX_STAGE = 3;

	// ステージごとのクエストの最大数
	static constexpr int MAX_QUEST_1 = 5;
	static constexpr int MAX_QUEST_2 = 1;
	static constexpr int MAX_QUEST_3 = 1;

	// クールタイム
	static constexpr float COOL_TIME = 1.0f;

// ---------------
// 公開関数
// ---------------
public:
	// コンストラクタ
	QuestManager(PlayScene* playScene);
	// デストラクタ
	~QuestManager();
	// クエストの初期化
	void InitializeQuest();
	// 更新処理
	void Update(float elapsedTime);
	// クエストの描画
	void DrawQuest();
	// クエストの終了
	void Finalize();


// ---------------
// 内部関数
// ---------------
private:
	// クエストの更新
	void UpdateQuest();
	// エフェクトの更新処理
	void UpdateEffect(float elapsedTime);


	// クエストリストの作成 1st
	void CreateQuestList_1st();
	// クエストリストの作成 2nd
	void CreateQuestList_2nd();
	// クエストリストの作成 3rd
	void CreateQuestList_3rd();
	// クエストデータの消去
	void ClearQuestData();

// ---------------
// メンバ変数
// ---------------
private:

	// 現在のクエスト番号
	int m_currentQuestNo;

	// プレイシーン
	PlayScene* m_playScene;

	// クエストのリスト
	std::vector<IQuestChecker*> m_questList;

	// 経過時間
	float m_totalTime;

	// クエストのクリアが可能か
	bool m_canClear;
};
