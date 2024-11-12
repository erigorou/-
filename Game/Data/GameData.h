#pragma once


class GameData
{
	// 列挙型 -------------------
public:
	enum class BATTLE_RESULT : unsigned int
	{
		WIN,
		LOSE,
		NONE
	};


// アクセサ関数 ------------
public:

	// 静的関数===========================================
	// GameDataクラスのインスタンスを取得する
	static GameData* const GetInstance();


	// 戦闘結果
	void			SetBattleResult(BATTLE_RESULT result)	{ m_battleResult = result;	}
	BATTLE_RESULT	GetBattleResult() const					{ return m_battleResult;	}

	// 画面のフェード量
	void	SetFadeValue(float value) { m_fadeValue = value; }
	float	GetFadeValue()		const { return m_fadeValue; }

	// 経過時間
	void	SetElapsedTime(float time) { m_elapsedTime = time; }
	float	GetElapsedTime()	const { return m_elapsedTime; }


// インスタンス化の為のprivate	(多重生成を防ぐため)
private:
	// コンストラクタ
	GameData();

// 公開関数 -----------------
public:
	// デストラクタ
	~GameData() = default;

	// 初期化処理
	void Initialize();


// メンバ変数 --------------
private:

	static std::unique_ptr<GameData> s_data;	// インスタンス

	BATTLE_RESULT m_battleResult; // 戦闘結果

	float m_fadeValue;	// 画面のフェード量

	float m_elapsedTime;	// 経過時間

};