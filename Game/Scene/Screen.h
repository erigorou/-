// ---------------------------------------------
//
// 名前：Screen.h
// 内容：画面のサイズを定義するクラス
// 作成：池田桜輔
//
// ---------------------------------------------

#pragma once
#include "pch.h"

class Screen
{
	// ------------------------------
	// 固定値
	// ------------------------------
public:
	// 画面の左
	static constexpr float LEFT = 0.0f;
	// 画面の右
	static constexpr float RIGHT = 1280.0f;
	// 画面の上
	static constexpr float TOP = 0.0f;
	// 画面の下
	static constexpr float BOTTOM = 720.0f;
	// 画面の中心
	static constexpr DirectX::SimpleMath::Vector2 CENTER = DirectX::SimpleMath::Vector2(640.0f, 360.0f);

	// 画面のサイズ(横)
	static constexpr float WIDTH = RIGHT - LEFT;
	// 画面のサイズ(縦)
	static constexpr float HEIGHT = BOTTOM - TOP;
};