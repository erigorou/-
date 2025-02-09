/*
	@file	LoseResultScene.h
	@brief	リザルトシーンクラス
*/
#pragma once
#include "../IScene.h"

// 前方宣言
class CommonResources;

class LoseResult final : public IScene
{
public:
	static const wchar_t* TEXTURE_PATH;	// テクスチャのパス

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;			// スプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	DirectX::SimpleMath::Vector2 m_texCenter;						// テクスチャの中心座標

	bool m_isChangeScene;	// シーン遷移フラグ

public:
	LoseResult();
	~LoseResult() override;

	void Initialize() override;
	void Update(float elapsedTime)override;
	void Render() override;
	void Finalize() override;

	SceneID GetNextSceneID() const;
};
