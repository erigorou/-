/*
	@file	ResultScene.h
	@brief	リザルトシーンクラス
*/
#pragma once
#include "IScene.h"

// 前方宣言
class CommonResources;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class ResultScene final : public IScene
{
private:
	CommonResources* m_commonResources;								// 共通リソース	
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;			// スプライトバッチ
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;				// スプライトフォント
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	DirectX::SimpleMath::Vector2 m_texCenter;						// テクスチャの中心座標


	bool m_isChangeScene;	// シーン遷移フラグ


public:
    ResultScene();
    ~ResultScene() override;

    void Initialize() override;
    void Update(float elapsedTime)override;
    void Render() override;
    void Finalize() override;

    SceneID GetNextSceneID() const;
};
