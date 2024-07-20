// ステージの境界壁
#pragma once

class Wall
{
public: // 固定値や受け渡し用関数
	static const float WALL_SCALE;		// 大きさ（固定値）

	// コンストラクタ
	Wall();
	// デストラクタ
	~Wall();
	// 初期化
	void Initialize();
	// 描画処理
	void Render(
		 ID3D11DeviceContext* context
		,DirectX::CommonStates* states
		,const DirectX::SimpleMath::Matrix& view
		,const DirectX::SimpleMath::Matrix& projection
	);
	// 終了処理
	void Finalize();

	// ワールド行列更新処理
	void UpdateWorldMatrix();

private:
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;

};
