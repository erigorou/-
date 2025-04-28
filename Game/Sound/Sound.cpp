// ---------------------------------------------------------
// 
// 名前:    Sound.cpp
// 内容:    サウンド管理クラス
//          FMODを使用してサウンドを再生する
// 作成:    池田桜輔
// 
// ---------------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/Sound/Sound.h"
#include "Libraries/FMOD/inc/fmod.hpp"
#include <fstream>
#include "nlohmann/json.hpp"
#include "Game/Data/GameData.h"

std::unique_ptr<Sound> Sound::s_sound = nullptr;

// ---------------------------------------------------------
/// <summary>
/// インスタンスを返す関数
/// </summary>
/// <returns>Soundのインスタンス</returns>
// ---------------------------------------------------------
Sound* const Sound::GetInstance()
{
    // インスタンスが生成されていない場合
    if (s_sound == nullptr)
    {
        // 生成する
        s_sound.reset(new Sound());
    }

    // インスタンスを返す
    return s_sound.get();
}

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
Sound::Sound()
    : m_system(nullptr),
    m_soundBGM(nullptr),
    m_channelBGM(nullptr),
    m_channelSE(nullptr),
    m_bgmVolume(DEFAULT_BGM_VOLUME),
    m_seVolume(DEFAULT_SE_VOLUME),
    m_fadeValue(FADE_VALUE)
{
    // FMODのシステムを初期化
    InitializeFMOD();
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
Sound::~Sound()
{
    FinalizeFMOD();
}

// ---------------------------------------------------------
/// <summary>
/// 初期化関数（コンストラクタで呼ばれる）
    /// </summary>
/// <param name="filePath">ロードするBGMファイルのパス</param>
// ---------------------------------------------------------
void Sound::InitializeFMOD()
{
    // システムをインスタンス化する
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    assert(result == FMOD_OK);

    // システムを初期化する
    result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
    assert(result == FMOD_OK);

    // JSONからBGMリストを生成する
    LoadBGMFromJson();

    // JSONからSEリストを生成する
    LoadSEFromJson();
}

// ---------------------------------------------------------
/// <summary>
/// JSONからBGM用の音声データを取得する
/// </summary>
// ---------------------------------------------------------
void Sound::LoadBGMFromJson()
{
    // JSONファイルを開く
	std::ifstream ifs(BGM_JSON_PATH);
    // ファイルを開けない場合はエラーを出す
    if (!ifs)
    {
        MessageBoxA(nullptr, "BGMのJsonファイルが見つかりません" , "エラー", MB_OK);
        return; // 処理を終了
    }

    // 仮の受付先を持つ
    FMOD::Sound* sound;
    FMOD_RESULT result;

	// Jsonファイルを読み込む
	nlohmann::json json = nlohmann::json::parse(ifs);

	// Jsonデータを順に処理
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		// キーと値を取得
		std::string key = it.key();
		std::string pathStr = it.value();

        // BGMをロードする
        result = m_system->createSound(pathStr.c_str(), FMOD_DEFAULT, nullptr, &sound);
        if (result != FMOD_OK)
        {
            std::string errorMsg = "BGMファイルの読み込みに失敗しました。\nパス: ";
            errorMsg += pathStr;
            MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_ICONERROR | MB_OK);
            assert(false); // ここで止める
        }

        // ロードしたBGMを登録する
        m_bgmData[key] = sound;
	}

    m_soundBGM = m_bgmData["TitleBGM"];
}


// ---------------------------------------------------------
/// <summary>
/// JSONからSE用の音声データを取得する
/// </summary>
// ---------------------------------------------------------
void Sound::LoadSEFromJson()
{
    // JSONファイルを開く
    std::ifstream ifs(SE_JSON_PATH);
    // ファイルを開けない場合はエラーを出す
    if (!ifs)
    {
        MessageBoxA(nullptr, "BGMのJsonファイルが見つかりません", "エラー", MB_OK);
        return; // 処理を終了
    }

    // 仮の受付先を持つ
    FMOD::Sound* sound;
    FMOD_RESULT result;

    // Jsonファイルを読み込む
    nlohmann::json json = nlohmann::json::parse(ifs);

    // Jsonデータを順に処理
    for (auto it = json.begin(); it != json.end(); ++it)
    {
        // キーと値を取得
        std::string key = it.key();
        std::string pathStr = it.value();

        // BGMをロードする
        result = m_system->createSound(pathStr.c_str(), FMOD_DEFAULT, nullptr, &sound);
        if (result != FMOD_OK)
        {
            std::string errorMsg = "SEファイルの読み込みに失敗しました。\nパス: ";
            errorMsg += pathStr;
            MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_ICONERROR | MB_OK);
            assert(false); // ここで止める
        }

        // ロードしたBGMを登録する
        m_seData[key] = sound;
    }
}


// ---------------------------------------------------------
/// <summary>
/// Sound再生の更新処理
/// </summary>
// ---------------------------------------------------------
void Sound::Update()
{
    FMOD_RESULT result;

    // フェードの値を取得
    s_sound->m_fadeValue = GameData::GetInstance()->GetFadeValue();

    // 二重再生しない
    if (s_sound->m_channelBGM == nullptr)
    {
        result = s_sound->m_system->playSound(s_sound->m_soundBGM, nullptr, false, &s_sound->m_channelBGM);
        assert(result == FMOD_OK && "BGM 再生失敗");
    }

    s_sound->SetBGMVolume(s_sound->m_fadeValue);

    // FMODのシステムを更新
    result = s_sound->m_system->update();
    assert(result == FMOD_OK);
}

// ---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void Sound::FinalizeFMOD()
{
    // Sound用のオブジェクトを解放する
    m_soundBGM->release();
    m_system->release();
}

// ---------------------------------------------------------
/// <summary>
/// BGMの音量調整
/// </summary>
/// <param name="volume">音量（0～1）</param>
// ---------------------------------------------------------
void Sound::SetBGMVolume(float volume)
{
    if (m_channelBGM != nullptr)
    {
        m_channelBGM->setVolume(volume);
    }
}

// ---------------------------------------------------------
/// <summary>
/// SEの音量調整
/// </summary>
/// <param name="volume">音量（0～1）</param>
// ---------------------------------------------------------
void Sound::SetSEVolume(float volume)
{
    if (m_channelSE != nullptr)
    {
        m_channelSE->setVolume(volume);
    }
}

// ---------------------------------------------------------
/// <summary>
/// BGMを変更する
/// </summary>
/// <param name="type">変更するBGMタイプ</param>
// ---------------------------------------------------------
void Sound::ChangeBGM(const std::string key)
{
    // すでに再生中のBGMを停止する
    if (s_sound->m_channelBGM != nullptr)
    {
        s_sound->m_channelBGM->stop();
    }

    // 再生するBGMを探索する
    auto it = s_sound->m_bgmData.find(key);
    if (it == s_sound->m_bgmData.end())
    {
        std::string errorMsg = "BGMが見つかりません。\nキー: " + key;
        MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_OK | MB_ICONERROR);
        return;
    }

    // BGMを取得
    FMOD::Sound* bgmSound = it->second;

    // BGMを再生
    FMOD_RESULT result = s_sound->m_system->playSound(bgmSound, nullptr, false, &s_sound->m_channelBGM);
    if (result != FMOD_OK)
    {
        std::string errorMsg = "BGM再生に失敗しました。\nキー: " + key;
        MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_OK | MB_ICONERROR);
        return;
    }

    // 音量調整を行う
    s_sound->SetBGMVolume(1.0f);
}

// ---------------------------------------------------------
/// <summary>
/// SEを再生する
/// </summary>
/// <param name="key">再生するもの</param>
// ---------------------------------------------------------
void Sound::PlaySE(const std::string key)
{
    // 再生するSEを探索する
	auto it = s_sound->m_seData.find(key);
	if (it == s_sound->m_seData.end())
	{
		std::string errorMsg = "SEが見つかりません。\nキー: " + key;
		MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_OK | MB_ICONERROR);
		return;
	}

	// SEを取得
    FMOD::Sound* seSound = it->second;

    // SEを再生
    FMOD_RESULT result = s_sound->m_system->playSound(seSound, nullptr, false, &s_sound->m_channelSE);
    if (result != FMOD_OK)
    {
        std::string errorMsg = "SE再生に失敗しました。\nキー: " + key;
        MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_OK | MB_ICONERROR);
        return;
    }

    // 音量調整を行う
    s_sound->SetSEVolume(1.0f);
}
