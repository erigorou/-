/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
*/
#pragma once

// 前方宣言
class CommonResources;

namespace mylib
{
	class DebugCamera;
}


class IScene
{
public:
	// シーンＩＤ
	enum class SceneID : unsigned int
	{
		NONE,
		TITLE,
		PLAY,
		RESULT
	};


public:
	virtual ~IScene() = default;

	virtual void Initialize() = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;

	virtual SceneID GetNextSceneID() const = 0;
};
