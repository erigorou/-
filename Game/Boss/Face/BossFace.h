// ----------------------------------------------------------------------
//
// 名前: BossFace.h
// 内容: ボスの顔クラス
// 作成: 池田桜輔
// ----------------------------------------------------------------------

// インクルード
#pragma once
#include "pch.h"
#include "Interface/IRenderable.h"

class BossFace : public IRenderable
{
	// ----------------------------
	// 定数
	// ----------------------------
public:
	// 待機顔
	static const int IDLING_FACE = 0;
	// 攻撃顔
	static const int ATTACKING_FACE = 1;


	// ----------------------------
	// アクセサ
	// ----------------------------
public:
	// 表示レイヤーを取得する
	UINT GetLayer() const override { return static_cast<UINT>(Layer::Object); }

	// ワールド行列を設定する
	void SetWorldMatrix(const DirectX::SimpleMath::Matrix& world) { m_world = world; }

	// 顔を変更
	void ChangeFace(const int type);

	// ---------------------------
	// メンバ関数（公開）
	// ---------------------------
public:
	// コンストラクタ
	BossFace();
	// デストラクタ
	~BossFace() override;

	// 描画コマンドを記録する
	void RecordRenderCommands(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		ID3D11DeviceContext* deferredContext) override;

	// ---------------------------
	// メンバ変数（非公開）
	// ---------------------------
private:
	// 現在の顔
	DirectX::Model* m_model;
	// 待機顔
	DirectX::Model* m_IdlingFace;
	// 攻撃顔
	DirectX::Model* m_AttackingFace;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
};