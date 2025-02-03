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
#include "Game/Data/GameData.h"

std::unique_ptr<Sound> Sound::s_sound = nullptr;

// ------------------------------------------------
// インスタンスを返す関数
// ------------------------------------------------
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
	, m_bgmVolume(1.0f)
	, m_seVolume(1.0f)
	, m_fadeValue(1.0f)
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
	LoadBGM(BGM_TYPE::TITLE, TITLE_BGM_PATH);
	LoadBGM(BGM_TYPE::PLAY, PLAY_BGM_PATH);
	LoadBGM(BGM_TYPE::WIN, WIN_BGM_PATH);
	LoadBGM(BGM_TYPE::LOSE, LOSE_BGM_PATH);

	// 最初のBGMをセットする
	m_soundBGM = m_bgmList[(size_t)BGM_TYPE::TITLE];
}

// ------------------------------------------------
// リストを生成する
// ------------------------------------------------
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
		assert(!"BGMを入れる場所とenumが合いません！！！！！");

	// リストに登録する
	m_bgmList.push_back(sound);
}

// ------------------------------------------------
// SEファイルの読みこみ
// ------------------------------------------------
void Sound::LoadSE(SE_TYPE type, const char* filePath)
{
	// 仮のサウンドを形成する
	FMOD::Sound* sound;

	// 仮サウンドに登録をする
	FMOD_RESULT result;
	result = m_system->createSound(filePath, FMOD_DEFAULT, nullptr, &sound);
	assert(result == FMOD_OK);

	if (m_seList.size() != (size_t)type)
		assert(!"SEを入れる場所とenumが合いません！！！！！");

	// リストに登録する
	m_seList.push_back(sound);
}

// ------------------------------------------------
// Sound再生Update
// ------------------------------------------------
void Sound::Update()
{
	FMOD_RESULT result;

	// フェードの値を取得
	s_sound->m_fadeValue = GameData::GetInstance()->GetFadeValue();

	// 二重再生しない
	if (s_sound->m_channelBGM == nullptr)
	{
		result = s_sound->m_system->playSound(s_sound->m_soundBGM, nullptr, false, &s_sound->m_channelBGM);
		assert(result == FMOD_OK && "BGM 再生失敗！");
	}

	s_sound->SetBGMVolume(s_sound->m_fadeValue);

	// FMODのシステムを更新する
	result = s_sound->m_system->update();
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
// SEの音量調整機構
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

// ------------------------------------------------
// BGMの変更
// ------------------------------------------------
// どのBGMを流すか
// -----------------------------------------------
void Sound::ChangeBGM(Sound::BGM_TYPE type)
{
	// すでに再生中のBGMを停止する
	if (s_sound->m_channelBGM != nullptr)
		s_sound->m_channelBGM->stop();

	// BGMを変更する
	s_sound->m_soundBGM = s_sound->m_bgmList[(size_t)type];

	// 新しいBGMを再生する
	FMOD_RESULT result = s_sound->m_system->playSound(s_sound->m_soundBGM, nullptr, false, &s_sound->m_channelBGM);
	assert(result == FMOD_OK && "BGM 再生失敗！");

	// 音量調整を行う
	s_sound->SetBGMVolume(1.0f);
}

// ------------------------------------------------
// SEの再生
// ------------------------------------------------
// 引数: type - 再生するSEの種類
// -----------------------------------------------
void Sound::PlaySE(Sound::SE_TYPE type)
{
	// 再生するSEを取得
	FMOD::Sound* seSound = s_sound->m_seList[(size_t)type];

	// SEを再生
	FMOD_RESULT result = s_sound->m_system->playSound(seSound, nullptr, false, &s_sound->m_channelSE);
	assert(result == FMOD_OK && "SE 再生失敗！");

	// 音量調整を行う（任意）
	s_sound->SetSEVolume(1.0f);  // ここで任意の音量を設定できます
}