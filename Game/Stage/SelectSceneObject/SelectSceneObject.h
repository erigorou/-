// ---------------------------------------------
// 
// 名前		:SelectSceneObject.h
// 内容		:セレクトシーンで表示するオブジェクト
// 作成		:池田桜輔
// 
// ---------------------------------------------

#pragma once
// インクルード
#include "pch.h"

class SelectSceneObject
{
	// -----------------------------
	// 定数
	// -----------------------------
public:
	// モデルの回転速度
	static constexpr float ROTATE_SPEED = 0.5f;

	// モデルの位置
	static constexpr DirectX::SimpleMath::Vector3 MODEL_POSITION = DirectX::SimpleMath::Vector3(
		20.0f,
		0.0f,
		70.0f
	);

	// -----------------------------
	// メンバ関数（公開）
	// -----------------------------
public:
	// コンストラクタ
	SelectSceneObject();
	// デストラクタ
	~SelectSceneObject();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj);
	// 終了処理
	void Finalize();

	// -----------------------------
	// メンバ関数(非公開)
	// -----------------------------
private:
	// モデルの受け取り
	void LoadModel();
	// ワールド行列の計算
	void CalcWorldMatrix();

	// -----------------------------
	// メンバ変数
	// -----------------------------
private:
	// モデルの配列
	std::vector<DirectX::Model*> m_models;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 回転
	DirectX::SimpleMath::Vector3 m_rotation;

	// 今のステージ番号
	int m_stageNo;
};