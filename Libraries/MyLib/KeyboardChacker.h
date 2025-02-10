// ---------------------------------------------------------
// 名前; KeyboardChacker.h
// 
// 内容: キーボード入力のチェックを行うクラス
// 制作: 池田桜輔
//
// ---------------------------------------------------------

#pragma once
#include "pch.h"

/// <summary>
/// キーボード入力のチェックを行うクラス
/// </summary>
class KeyboardChacker
{
	// -------------------
	// メンバ関数
	// -------------------
public:
	/// <summary>
	/// 矢印キーが入力されたかどうかを判定する
	/// </summary>
	/// <param name="key">押されたキー</param>
	/// <returns>押されているか</returns>
	static bool IsInputArrowKey(DirectX::Keyboard::Keys key)
	{
		return
			key == DirectX::Keyboard::Keys::Up ||
			key == DirectX::Keyboard::Keys::Down ||
			key == DirectX::Keyboard::Keys::Left ||
			key == DirectX::Keyboard::Keys::Right;
	}
};