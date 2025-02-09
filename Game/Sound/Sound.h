// --------------------------------------------------
// 名前:	Sound.h
// 内容:	サウンド管理クラス
// 作成:	池田桜輔
// --------------------------------------------------
#pragma once

// FMOD用の前方宣言
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class Sound
{
	// -----------------------------
	// 列挙型
	// -----------------------------
public:
	// BGMの種類
	enum class BGM_TYPE : unsigned int
	{
		TITLE,
		PLAY,
		WIN,
		LOSE
	};

	// SEの種類
	enum class SE_TYPE : unsigned int
	{
		SYSTEM_OK,
		PLAYER_ATTACK,
		PLAYER_ATTACK2,
		PLAYER_DAMAGED,
		BOSS_MOVE,
		BOSS_ATTACK,
		BOSS_SWEEP
	};

	// -----------------------------
	// 固定値
	// -----------------------------
public:
	// タイトルBGM
	static constexpr const char* TITLE_BGM_PATH = "Resources/Sounds/BGM/Title.mp3";
	// プレイBGM
	static constexpr const char* PLAY_BGM_PATH = "Resources/Sounds/BGM/Play.mp3";
	// 勝利BGM
	static constexpr const char* WIN_BGM_PATH = "Resources/Sounds/BGM/Win.mp3";
	// 敗北BGM
	static constexpr const char* LOSE_BGM_PATH = "Resources/Sounds/BGM/Lose.mp3";

	// 選択SE
	static constexpr const char* SYSTEM_OK_PATH = "Resources/Sounds/SE/System/OK.mp3";
	// プレイヤーの攻撃SE
	static constexpr const char* PLAYER_ATTACK_SE_PATH = "Resources/Sounds/SE/Player/Attack.mp3";
	// プレイヤーの攻撃SE
	static constexpr const char* PLAYER_ATTACK2_SE_PATH = "Resources/Sounds/SE/Player/Attack2.mp3";
	// プレイヤーのダメージSE
	static constexpr const char* PLAYER_DAMAGED_SE_PATH = "Resources/Sounds/SE/Player/DamagedSE.mp3";
	// ボスの移動SE
	static constexpr const char* ENEMY_MOVE_SE_PATH = "Resources/Sounds/SE/Enemy/Move.mp3";
	// ボスの攻撃SE
	static constexpr const char* ENEMY_ATTACK_SE_PATH = "Resources/Sounds/SE/Enemy/Attack.mp3";
	// ボスの掃討SE
	static constexpr const char* ENEMY_SWEEP_SE_PATH = "Resources/Sounds/SE/Enemy/Sweep.mp3";

	// -----------------------------
	// アクセサ
	// -----------------------------
public:
	// BGM再生クラスのインスタンスを取得する
	static Sound* const GetInstance();

	// BGMの音量を調整する関数
	void SetBGMVolume(float volume);
	// SEの音量を調整する関数
	void SetSEVolume(float volume);

	// BGMの変更
	static void ChangeBGM(BGM_TYPE type);
	// SEの再生
	static void PlaySE(SE_TYPE type);

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// デストラクタ
	~Sound();
	// 更新処理
	static void Update();
	// SOUNDで使用したオブジェクトを解放
	void FinalizeFMOD();

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// コンストラクタ
	Sound();
	// FMODのシステムの初期化
	void InitializeFMOD();
	// BGMリストの生成
	void CreateBGMList();
	// SEリストの生成
	void CreateSEList();
	// BGMの読み込み
	void LoadBGM(BGM_TYPE type, const char* filePath);
	// SEの読み込み
	void LoadSE(SE_TYPE type, const char* filePath);

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// インスタンス
	static std::unique_ptr<Sound> s_sound;

	// FMODのシステム
	FMOD::System* m_system;
	// BGM用の音声データ
	FMOD::Sound* m_soundBGM;
	// BGMを再生するチャンネル
	FMOD::Channel* m_channelBGM;
	// SEを再生するチャンネル
	FMOD::Channel* m_channelSE;

	// BGM用の音声データリスト
	std::vector<FMOD::Sound*> m_bgmList;
	// SE用の音声データリスト
	std::vector<FMOD::Sound*> m_seList;

	// BGMの音量
	float m_bgmVolume;
	// SEの音量
	float m_seVolume;
	// 画面のフェード量(フェードに応じてBGMも行う)
	float m_fadeValue;
};
