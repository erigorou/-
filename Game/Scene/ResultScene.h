/*
	@file	ResultScene.h
	@brief	リザルトシーンクラス
*/
#pragma once
#include "IScene.h"

#include "../Data/GameData.h"

// 前方宣言
class CommonResources;
class GameData;
class WinResult;
class LoseResult;

namespace mylib
{
	class DebugCamera;
}

class ResultScene final : public IScene
{
	// 固定値
public:

	// 共通関数
public:
	ResultScene();
	~ResultScene() override;

	void Initialize() override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;

	// メンバ関数
private:
	void CreateTextures();	// テクスチャの作成
	void CreateObjects();	// オブジェクトの作成

	// メンバ変数
private:
	CommonResources* m_commonResources;								// 共通リソース
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;			// スプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	DirectX::SimpleMath::Vector2 m_texCenter;						// テクスチャの中心座標

	// リザルトシーンステート
	std::unique_ptr<WinResult>	m_winResult;	// 勝利リザルト
	std::unique_ptr<LoseResult> m_loseResult;	// 敗北リザルト

	bool m_isChangeScene;	// シーン遷移フラグ

	GameData* m_gameData;	// ゲームデータ（シーンで渡すやつ）

	GameData::BATTLE_RESULT m_result;	// 戦闘結果

	IScene* m_currentState;	// 現在のステート
};
