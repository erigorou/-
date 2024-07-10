#pragma once

// FMOD用の前方宣言
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class BGM_Player
{
public:
	// FMODのシステムの初期化と音声データのロード
	void InitializeFMOD(const char* filePath);

	// 更新処理
	void Update();

	// SOUNDで使用したオブジェクトを解放
	void FinalizeFMOD();
private:
	FMOD::System* m_system;	// FMODのシステム
	FMOD::Sound* m_soundBGM;	// BGM用の音声データ
	FMOD::Channel* m_channelBGM;// BGMを再生するチャンネル
};