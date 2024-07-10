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

void BGM_Player::InitializeFMOD(const char* filePath)
{
	// システムをインスタンス化する
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// システムを初期化する
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// BGMをロードする
	//result = m_system->createSound("Resources/Sounds/maou_bgm_8bit29.ogg", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	result = m_system->createSound(filePath, FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);
}

void BGM_Player::Update()
{
	FMOD_RESULT result;

	// 二重再生しない
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
	}

	// FMODのシステムを更新する
	result = m_system->update();
	assert(result == FMOD_OK);
}


void BGM_Player::FinalizeFMOD()
{
	// Sound用のオブジェクトを解放する
	m_soundBGM->release();
	m_system->release();
}