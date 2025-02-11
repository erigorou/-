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
    m_bgmList(),
    m_seList(),
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

    // BGMリストを生成する
    CreateBGMList();

    // SEリストを生成する
    CreateSEList();
}

// ---------------------------------------------------------
/// <summary>
/// BGMリストを生成する
/// </summary>
// ---------------------------------------------------------
void Sound::CreateBGMList()
{
    LoadBGM(BGM_TYPE::TITLE, TITLE_BGM_PATH);
    LoadBGM(BGM_TYPE::PLAY, PLAY_BGM_PATH);
    LoadBGM(BGM_TYPE::WIN, WIN_BGM_PATH);
    LoadBGM(BGM_TYPE::LOSE, LOSE_BGM_PATH);

    // 最初のBGMをセットする
    m_soundBGM = m_bgmList[static_cast<size_t>(BGM_TYPE::TITLE)];
}

// ---------------------------------------------------------
/// <summary>
/// SEリストを生成する
/// </summary>
// ---------------------------------------------------------
void Sound::CreateSEList()
{
    LoadSE(SE_TYPE::SYSTEM_OK, SYSTEM_OK_PATH);
    LoadSE(SE_TYPE::PLAYER_ATTACK, PLAYER_ATTACK_SE_PATH);
    LoadSE(SE_TYPE::PLAYER_ATTACK2, PLAYER_ATTACK2_SE_PATH);
    LoadSE(SE_TYPE::PLAYER_DAMAGED, PLAYER_DAMAGED_SE_PATH);
    LoadSE(SE_TYPE::BOSS_MOVE, ENEMY_MOVE_SE_PATH);
    LoadSE(SE_TYPE::BOSS_ATTACK, ENEMY_ATTACK_SE_PATH);
    LoadSE(SE_TYPE::BOSS_SWEEP, ENEMY_SWEEP_SE_PATH);
}

// ---------------------------------------------------------
/// <summary>
/// BGMファイルを読み込む
/// </summary>
/// <param name="type">BGMタイプ</param>
/// <param name="filePath">BGMファイルのパス</param>
// ---------------------------------------------------------
void Sound::LoadBGM(BGM_TYPE type, const char* filePath)
{
    FMOD::Sound* sound;
    FMOD_RESULT result;

    // 仮のサウンドを形成する
    result = m_system->createSound(filePath, FMOD_LOOP_NORMAL, nullptr, &sound);
    assert(result == FMOD_OK);

    // BGMリストに追加
    if (m_bgmList.size() != static_cast<size_t>(type))
        assert(!"BGMを入れる場所とenumが合いません。");

    m_bgmList.push_back(sound);
}

// ---------------------------------------------------------
/// <summary>
/// SEファイルを読み込む
/// </summary>
/// <param name="type">SEタイプ</param>
/// <param name="filePath">SEファイルのパス</param>
// ---------------------------------------------------------
void Sound::LoadSE(SE_TYPE type, const char* filePath)
{
    FMOD::Sound* sound;
    FMOD_RESULT result;

    // 仮のサウンドを形成する
    result = m_system->createSound(filePath, FMOD_DEFAULT, nullptr, &sound);
    assert(result == FMOD_OK);

    // SEリストに追加
    if (m_seList.size() != static_cast<size_t>(type))
        assert(!"SEを入れる場所とenumが合いません。");

    m_seList.push_back(sound);
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
void Sound::ChangeBGM(Sound::BGM_TYPE type)
{
    // すでに再生中のBGMを停止する
    if (s_sound->m_channelBGM != nullptr)
    {
        s_sound->m_channelBGM->stop();
    }

    // BGMを変更する
    s_sound->m_soundBGM = s_sound->m_bgmList[static_cast<size_t>(type)];

    // 新しいBGMを再生する
    FMOD_RESULT result = s_sound->m_system->playSound(s_sound->m_soundBGM, nullptr, false, &s_sound->m_channelBGM);
    if (result != FMOD_OK)
    {
        std::string errorMsg = "BGM再生に失敗しました。\nBGM_TYPE: " + std::to_string(static_cast<int>(type));
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
/// <param name="type">再生するSEの種類</param>
// ---------------------------------------------------------
void Sound::PlaySE(Sound::SE_TYPE type)
{
    // 再生するSEを取得
    FMOD::Sound* seSound = s_sound->m_seList[static_cast<size_t>(type)];

    // SEを再生
    FMOD_RESULT result = s_sound->m_system->playSound(seSound, nullptr, false, &s_sound->m_channelSE);
    if (result != FMOD_OK)
    {
        std::string errorMsg = "SE再生に失敗しました。\nBGM_TYPE: " + std::to_string(static_cast<int>(type));
        MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_OK | MB_ICONERROR);
        return;
    }

    // 音量調整を行う
    s_sound->SetSEVolume(1.0f);
}
