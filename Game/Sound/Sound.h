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
public:
	// 列挙型-------------------
	enum class BGM_TYPE : unsigned int
	{
		TITLE,
		PLAY,
		WIN,
		LOSE
	};

	enum class SE_TYPE : unsigned int
	{
		ATTACK,
		SWEEP
	};


	// 固定値
	static constexpr const char* TITLE_BGM_PATH	= "Resources/Sounds/BGM/Play.mp3"	;
	static constexpr const char* PLAY_BGM_PATH	= "Resources/Sounds/BGM/Debug.mp3"	;
	static constexpr const char* WIN_BGM_PATH	= "Resources/Sounds/BGM/Play.mp3"	;
	static constexpr const char* LOSE_BGM_PATH	= "Resources/Sounds/BGM/Play.mp3"	;

	static constexpr const char* ATTACK_SE_PATH = "Resources/Sounds/SE/Attack.mp3"	;
	static constexpr const char* SWEEP_SE_PATH	= "Resources/Sounds/SE/Sweep.mp3"	;
// アクセサ関数
public:

	// BGM再生クラスのインスタンスを取得する
	static Sound* const GetInstance();

	// 音量を調整する関数
	void SetBGMVolume	(float volume);
	void SetSEVolume	(float volume);


	static void ChangeBGM(Sound::BGM_TYPE type);
	static void PlaySE(Sound::SE_TYPE type);

	// デストラクタ
	~Sound();

	// 更新処理
	static void Update();

	// SOUNDで使用したオブジェクトを解放
	void FinalizeFMOD();

private:
	// コンストラクタ
	Sound();
	// FMODのシステムの初期化
	void InitializeFMOD();
	// BGMリストの生成
	void CreateBGMList();
	// SEリストの生成
	void CreateSEList();

	//　BGM読み込み
	void LoadBGM	(BGM_TYPE	type, const char* filePath);
	void LoadSE		(SE_TYPE	type, const char* filePath);

private:
	static std::unique_ptr<Sound> s_sound;	// インスタンス

	FMOD::System*	m_system;		// FMODのシステム
	FMOD::Sound*	m_soundBGM;		// BGM用の音声データ
	FMOD::Channel*	m_channelBGM;	// BGMを再生するチャンネル
	FMOD::Channel*	m_channelSE;	// SEを再生するチャンネル

	std::vector<FMOD::Sound*> m_bgmList;	// BGM用の音声データリスト
	std::vector<FMOD::Sound*> m_seList;		// SE用の音声データリスト

	float m_bgmVolume;	// BGMの音量
	float m_seVolume;	// SEの音量

	float m_fadeValue;	// 画面のフェード量(フェードに応じてBGMも行う。)
};