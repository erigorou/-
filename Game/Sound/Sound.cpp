// ---------------------------
// *
// * FMODを使用してBGMを流す
// *
// ---------------------------

#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Sound/Sound.h"
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"

std::unique_ptr<Sound> Sound::s_bgmPlayer = nullptr;


// ------------------------------------------------
// インスタンスを返す関数
// ------------------------------------------------
Sound* const Sound::GetInstance()
{
	// インスタンスが生成されていない場合
	if (s_bgmPlayer == nullptr)
	{
		// 生成する
		s_bgmPlayer.reset(new Sound());
	}
	// インスタンスを返す
	return s_bgmPlayer.get();
}


// ------------------------------------------------
// コンストラクタ
// ------------------------------------------------
Sound::Sound()
    : m_system(nullptr)
    , m_soundBGM(nullptr)
    , m_channelBGM(nullptr)
    , m_channelSE(nullptr)
    , m_bgmList()
    , m_seList()
{
	InitializeFMOD();
}

// ------------------------------------------------
// デストラクタ
// ------------------------------------------------
Sound::~Sound()
{
	FinalizeFMOD();
}


// ------------------------------------------------
// 初期化関数(コンストラクタで読んでる)
// ------------------------------------------------
/// <param name="filePath">ロードするBGMファイルのパス</param>
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

// ------------------------------------------------
// リストを生成する
// ------------------------------------------------
void Sound::CreateBGMList()
{
    LoadBGM(BGM_TYPE::TITLE,    TITLE_BGM_PATH  );
	LoadBGM(BGM_TYPE::PLAY,     PLAY_BGM_PATH   );
	LoadBGM(BGM_TYPE::WIN,      WIN_BGM_PATH    );
	LoadBGM(BGM_TYPE::LOSE,     LOSE_BGM_PATH   );

    // 最初のBGMをセットする
	m_soundBGM = m_bgmList[(size_t)BGM_TYPE::TITLE];
}


// ------------------------------------------------
// リストを生成する
// ------------------------------------------------
void Sound::CreateSEList()
{

}


// ------------------------------------------------
// BGMファイルの読み込み
// ------------------------------------------------
void Sound::LoadBGM(BGM_TYPE type, const char* filePath)
{
    // 仮のサウンドを形成する
    FMOD::Sound* sound;

    // 仮サウンドに登録をする
	FMOD_RESULT result;
	result = m_system->createSound(filePath, FMOD_LOOP_NORMAL, nullptr, &sound);
	assert(result == FMOD_OK);

    if (m_bgmList.size() != (size_t)type)
        assert(! "BGMを入れる場所とenumが合いません！！！！！");

    // リストに登録する
	m_bgmList.push_back(sound);
}

// ------------------------------------------------
// BGMファイルの読み込み
// ------------------------------------------------



// ------------------------------------------------
// BGM再生Update
// ------------------------------------------------
void Sound::Update()
{
    FMOD_RESULT result;

    // 二重再生しない
    if (s_bgmPlayer->m_channelBGM == nullptr)
    {
        result = s_bgmPlayer->m_system->playSound(s_bgmPlayer->m_soundBGM, nullptr, false, &s_bgmPlayer->m_channelBGM);
        assert(result == FMOD_OK && "BGM 再生失敗！");

        // 音量調整を行う
        s_bgmPlayer->SetBGMVolume(0.2f);
    }

    // FMODのシステムを更新する
    result = s_bgmPlayer->m_system->update();
    assert(result == FMOD_OK);
}


// ------------------------------------------------
// 終了処理
// ------------------------------------------------
void Sound::FinalizeFMOD()
{
    // Sound用のオブジェクトを解放する
    m_soundBGM->release();
    m_system->release();
}


// ------------------------------------------------
// BGMの音量調整機構
// ------------------------------------------------
// 音量　＝　MAX 1 : MIN 0
// -----------------------------------------------
void Sound::SetBGMVolume(float volume)
{
    if (m_channelBGM != nullptr)
    {
        m_channelBGM->setVolume(volume);
    }
}


// ------------------------------------------------
// BGMの音量調整機構
// ------------------------------------------------
// 音量　＝　MAX 1 : MIN 0
// -----------------------------------------------
void Sound::SetSEVolume(float volume)
{
    if (m_channelSE != nullptr)
    {
        m_channelSE->setVolume(volume);
    }
}
