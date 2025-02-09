// ------------------------------------------------
// 名前:	GameData.h
// 内容:	ゲームデータを渡すためのクラス
//			主にシーン間でのデータの受け渡しに使用する
// 作成;	池田桜輔
// ------------------------------------------------
#pragma once

/// <summary>
/// ゲームデータを渡すためのクラス
/// </summary>
class GameData
{
	// ------------------------------
	// 構造体
	// ------------------------------
public:
	enum class BATTLE_RESULT : unsigned int
	{
		WIN,
		LOSE,
		NONE
	};

	// ------------------------------
	// アクセサ
	// ------------------------------
public:
	// GameDataクラスのインスタンスを取得する
	static GameData* const GetInstance();

	// 戦闘結果の設定
	void SetBattleResult(BATTLE_RESULT result) { m_battleResult = result; }
	// 戦闘結果の取得
	BATTLE_RESULT GetBattleResult() const { return m_battleResult; }
	// 画面のフェード量の設定
	void SetFadeValue(float value) { m_fadeValue = value; }
	// 画面のフェード量の取得	
	float GetFadeValue() const { return m_fadeValue; }
	// 経過時間
	void SetElapsedTime(float time) { m_elapsedTime = time; }
	// 経過時間の取得
	float GetElapsedTime() const { return m_elapsedTime; }
	// 選択したステージのインデックスの設定
	void SetSelectStage(int stageIndex) { m_questIndex = stageIndex; }
	// 選択したステージのインデックスの取得
	int GetSelectStage() const { return m_questIndex; }

	// ------------------------------
	// メンバ関数(公開)
	// ------------------------------
public:
	// コンストラクタ
	GameData();
	// デストラクタ
	~GameData() = default;

	// ------------------------------
	// メンバ変数
	// ------------------------------
private:
	// インスタンス
	static std::unique_ptr<GameData> s_data;
	// 戦闘結果
	BATTLE_RESULT m_battleResult;
	// 画面のフェード量
	float m_fadeValue;
	// 経過時間
	float m_elapsedTime;
	// クエストのインデックス
	int m_questIndex;
};