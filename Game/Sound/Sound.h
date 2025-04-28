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
	// 固定値
	// -----------------------------
public:

	// BGMのJSONファイルパス
	static constexpr const char* BGM_JSON_PATH = "Resources/Jsons/BGMData.json";
	// SEのJSONファイルパス
	static constexpr const char* SE_JSON_PATH = "Resources/Jsons/SEData.json";

	// 初期のBGM音量
	static constexpr float DEFAULT_BGM_VOLUME = 1.0f;
	// 初期のSE音量
	static constexpr float DEFAULT_SE_VOLUME = 1.0f;
	// フェード量
	static constexpr float FADE_VALUE = 1.0f;

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
	static void ChangeBGM(const std::string key);
	// SEの再生
	static void PlaySE(const std::string key);

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

	// JSONからBGMの音声データを読み込む
	void LoadBGMFromJson();
	// JSONからSEの音声データを読み込む
	void LoadSEFromJson();


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


	// BGM用の音声データ群
	std::unordered_map<std::string, FMOD::Sound*> m_bgmData;
	// SE用の音声データ群
	std::unordered_map<std::string, FMOD::Sound*> m_seData;

	// BGMの音量
	float m_bgmVolume;
	// SEの音量
	float m_seVolume;
	// 画面のフェード量(フェードに応じてBGMも行う)
	float m_fadeValue;
};
