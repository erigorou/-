// ---------------------------
// *
// * FMODを使用してBGMを流す
// *
// ---------------------------

#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Sound/BGM_Player.h"
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"


/// <summary>
/// コンストラクタ
/// </summary>
BGM_Player::BGM_Player()
	: m_system(nullptr)
	, m_soundBGM(nullptr)
	, m_channelBGM(nullptr)
{
	InitializeFMOD(BGM_FILE_PATH);
}


/// <summary>
/// デストラクタ
/// </summary>
BGM_Player::~BGM_Player()
{
	FinalizeFMOD();
}


/// <summary>
/// FMODのシステムを初期化し、指定されたファイルパスのBGMをロードする。
/// </summary>
/// <param name="filePath">ロードするBGMファイルのパス</param>
void BGM_Player::InitializeFMOD(const char* filePath)
{
    // システムをインスタンス化する
    FMOD_RESULT result = FMOD::System_Create(&m_system);
    assert(result == FMOD_OK);

    // システムを初期化する
    result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
    assert(result == FMOD_OK);

    // BGMをロードする
    result = m_system->createSound(filePath, FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
    assert(result == FMOD_OK);

}

/// <summary>
/// BGMの再生を開始し、FMODのシステムを更新する。
/// </summary>
void BGM_Player::Update()
{
    FMOD_RESULT result;

    // 二重再生しない
    if (m_channelBGM == nullptr)
    {
        result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
        assert(result == FMOD_OK);

        // 音量調整を行う
        SetVolume(0.05f);
    }

    // FMODのシステムを更新する
    result = m_system->update();
    assert(result == FMOD_OK);
}

/// <summary>
/// FMODのシステムとサウンドオブジェクトを解放する。
/// </summary>
void BGM_Player::FinalizeFMOD()
{
    // Sound用のオブジェクトを解放する
    m_soundBGM->release();
    m_system->release();
}

/// <summary>
/// 再生中のBGMの音量を設定する。
/// </summary>
/// <param name="volume">設定する音量（0.0から1.0の範囲）</param>
void BGM_Player::SetVolume(float volume)
{
    if (m_channelBGM != nullptr)
    {
        m_channelBGM->setVolume(volume);
    }
}
