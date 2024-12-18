#pragma once
#include "pch.h"

class CustomShader;

class EnemyDamageEffect
{
// ** 固定値 **
private:
	// モデルにエフェクトを掛けておく時間
	static constexpr float TOTAL_TIME = 0.25f;

	// シェーダーのファイルパス
	static const wchar_t* PS_PATH;

	// 頂点入力レイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		float Time; // 時間 (シェーダー用) // ★
		float Padding[3]; // 16バイトアライメント // ★
	};


// ** 公開関数 **
public:
	// コンストラクタ
	EnemyDamageEffect();
	// デストラクタ
	~EnemyDamageEffect();

	void Update(float elapsedTime);

	// 体パーツの描画
	void DrawWithDamageEffect(
		DirectX::Model* model,
		const DirectX::SimpleMath::Matrix world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	//ダメージを受けたことを通達する関数
	void IsDamaged();

// ** 非公開関数 **
private:
	// シェーダーの生成
	void CreateShader();
	// 定数バッファの作成
	void CreateConstBuffer();


// ** メンバ変数 **
private:
	// 経過時間
	float m_totalTime;
	// ダメージエフェクトを付与するのか
	bool m_isDamaged;

	// デバイスリソース
	ID3D11Device1* m_device;
	// シェーダー
	std::unique_ptr<CustomShader> m_damageShader;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
};