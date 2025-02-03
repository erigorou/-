// -------------------------------------------------------------------
//
// 動くものの後ろに付くパーティクルの動き
//
// -------------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IParticle.h"

//SimpleMath::Vector2(0.0f, 0.0f)), // 左上
//SimpleMath::Vector2(1.0f, 0.0f)), // 右上
//SimpleMath::Vector2(1.0f, 1.0f)), // 右下
//SimpleMath::Vector2(0.0f, 1.0f)), // 左下

class SwordTrailParticle : public IParticle
{
	// ----------------------------
	// アクセサ
	// ----------------------------
public:
	const float GetLife() { return m_life; }			// 表示時間
	const float GetStartLife() { return m_startLife; }		// 開始時間
	const DirectX::SimpleMath::Color GetNowColor() { return m_nowColor; }		// 現在の色
	const DirectX::SimpleMath::Color GetStartColor() { return m_startColor; }		// 開始時色
	const DirectX::SimpleMath::Color GetEndColor() { return m_endColor; }		// 終了時色

	void GetVertices(DirectX::VertexPositionColorTexture* vertices)
	{
		// 各頂点の位置、色、テクスチャ座標を設定する
		vertices[0] = DirectX::VertexPositionColorTexture(m_vertices[0].position, DirectX::SimpleMath::Vector4(1, 1, 1, 1), m_vertices[0].textureCoordinate);
		vertices[1] = DirectX::VertexPositionColorTexture(m_vertices[1].position, DirectX::SimpleMath::Vector4(1, 1, 1, 1), m_vertices[1].textureCoordinate);
		vertices[2] = DirectX::VertexPositionColorTexture(m_vertices[2].position, DirectX::SimpleMath::Vector4(1, 1, 1, 1), m_vertices[2].textureCoordinate);
		vertices[3] = DirectX::VertexPositionColorTexture(m_vertices[3].position, DirectX::SimpleMath::Vector4(1, 1, 1, 1), m_vertices[3].textureCoordinate);
	}

	// ----------------------------
	// メンバ関数（公開）
	// ----------------------------
public:
	// コンストラクタ
	SwordTrailParticle(
		DirectX::VertexPositionTexture vertex[],											// 頂点情報
		float life,																			// 生存時間
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor			// 初期カラーと最終カラー
	);
	// デストラクタ
	~SwordTrailParticle();
	// 更新処理
	bool Update(float elapsedTime);

	// ----------------------------
	// メンバ変数
	// ----------------------------
private:
	// 描画に必要な４頂点の座標
	DirectX::VertexPositionTexture m_vertices[4];
	//	生存時間
	float m_life;
	float m_startLife;
	//	カラー
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;
};