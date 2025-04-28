// ---------------------------------------------------------
//
// 名前：Sign.h
// 説明：看板クラス
// 作者：池田桜輔
//
// ---------------------------------------------------------
#pragma once

// インクルード
#include "pch.h"


/// <summary>
/// 看板クラス
/// </summary>
class Sign
{
	// -------------------------------
	// 定数
	// -------------------------------
public:
	// 看板のスケール
	static constexpr float SIGN_SCALE = 0.15f;

	// 看板の回転
	static constexpr float SIGN_ROTATE = DirectX::XMConvertToRadians(150.0f);

	// 看板の位置
	static constexpr DirectX::SimpleMath::Vector3 SIGN_POSITION = DirectX::SimpleMath::Vector3(-35.0f, 0.0f, 50.0f);

	// -------------------------------
	// メンバ関数（公開）
	// -------------------------------
public:
	// コンストラクタ
	Sign();
	// デストラクタ
	~Sign();
	// 看板の描画
	void DrawSign(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection
	);

	// -------------------------------
	// メンバ変数
	// -------------------------------
private:
	// 看板のモデル
	std::unique_ptr<DirectX::Model> m_signModel;

};
