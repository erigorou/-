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

	// GameDataクラスのインスタンスを取得する
	static GameData* const GetInstance();

	// 戦闘結果
	void			SetBattleResult(BATTLE_RESULT result)	{ m_battleResult = result;	}
	BATTLE_RESULT	GetBattleResult() const					{ return m_battleResult;	}




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

};