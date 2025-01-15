// ---------------------------------------------------
// ステージクリアのクエストを管理するクラス
// ---------------------------------------------------

#pragma once
#include "pch.h"

class PlayScene;		// プレイシーン
class QuestRenderer;	// クエスト内容の描画
class Tutorial;			// チュートリアル
class IQuestChecker;	// クエストのチェッカー

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
	// 最初の遅延
	static constexpr float DELAY_TIME = 1.0f;
	// クリアの遅延
	static constexpr float CLEAR_DELAY_TIME = 2.0f;



// ---------------
// アクセサ
// ---------------
public:
	// 現在のクエスト番号を取得
	int GetCurrentQuestNo() const { return m_currentQuestNo; }

	// チュートリアルを取得
	Tutorial* GetTutorial() { return m_tutorial.get(); }



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

	// クエストを一つ更新する
	void ChangeNextQuest();


// ---------------
// 内部関数
// ---------------
private:
	// クエストの更新
	void UpdateQuest();

	// クエストの作成
	void CreateQuest(int questIndex);

	// クエストリストの作成 1st
	void CreateQuestList_1st();
	// クエストリストの作成 2nd
	void CreateQuestList_2nd();
	// クエストリストの作成 3rd
	void CreateQuestList_3rd();

	// クエストのテクスチャを読み込む
	void AddQuestTexture();
	// チュートリアルのテクスチャを読み込む
	void AddTutorialTexture();

	// チュートリアルの作成
	void CreateTutorial();

	// クエストデータの消去
	void ClearQuestData();

	// クエストのクリアを確認
	void IsQuestClear();


// ---------------
// メンバ変数
// ---------------
private:

	// 現在のクエスト番号
	int m_currentQuestNo;

	// プレイシーン
	PlayScene* m_playScene;

	std::vector<IQuestChecker*>										m_questList;	// クエストのリスト
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	m_textureList;	// テクスチャのリスト
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	m_tutorialTextureList;	// チュートリアルのリスト


	// クエストの描画
	std::unique_ptr<QuestRenderer> m_renderer;

	// チュートリアルの描画
	std::unique_ptr<Tutorial> m_tutorial;

	// 経過時間
	float m_totalTime;
	// クリアしてからの時間
	float m_clearTime;
	// 経過時間
	float m_elapsedTime;

	// クエストのクリアが可能か
	bool m_canClear;
};
