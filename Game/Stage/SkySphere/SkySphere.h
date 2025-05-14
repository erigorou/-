#pragma once

#include "pch.h"
#include "Interface/IRenderable.h"

class SkySphere : public IRenderable
{
public:
	// -----------------------------
	// 固定値
	// -----------------------------
	static const float SKYSPHERE_SCALE;

public:
	// -------------------------
	// メンバ関数(公開)
	// -------------------------

	// コンストラクタ
	SkySphere();
	// デストラクタ
	~SkySphere();

	// 更新処理
	void Update();

	// モデルを取得する
	void LoadSkySphereModel();

	// モデルの描画
	void DrawSkySphere(
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection
	);

	// 描画コマンド登録
	void RecordRenderCommands(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		ID3D11DeviceContext* deferredContext) override;

	// ---------------------------
	// メンバ変数
	// ---------------------------
private:
	// SkyeSphereのモデル
	std::unique_ptr<DirectX::Model> m_skySphereModel;
};
