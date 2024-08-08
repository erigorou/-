// ----------------------------------------------
//
// @パーティクルのインターフェースクラス
// @ Ikeda Osuke
//
// ---------------------------------------------

#pragma once
#include "pch.h"

class IParticle
{
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsedTime">フレーム毎にかかった時間</param>
	/// <returns>生存しているか</returns>
	virtual bool Update(float elapsedTime) = 0;
};