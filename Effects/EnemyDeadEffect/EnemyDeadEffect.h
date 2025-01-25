#pragma once
#include "pch.h"

class CustomShader;

class EnemyDeadEffect
{
// ** 固定値 **
private:
	// モデルにエフェクトを掛けておく時間
	static constexpr float TOTAL_TIME = 2.5f;

	// シェーダーのファイルパス
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/DeadEffectPS.cso";
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/DeadEffectVS.cso";


	// 頂点入力レイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// 定数バッファ
	struct ConstBuffer
	{
		float Time; // 時間
		DirectX::SimpleMath::Vector3 Padding;
	};



// ** 公開関数 **
public:
	// コンストラクタ
	EnemyDeadEffect();
	// デストラクタ
	~EnemyDeadEffect();

	void Update(float elapsedTime);

	// 体パーツの描画
	void DrawWithDeadEffect(
		DirectX::Model* model,
		const DirectX::SimpleMath::Matrix world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	void IsDead();

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
	// シェーダー
	std::unique_ptr<CustomShader> m_deadShader;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	// 死亡フラグ
	bool m_isDead;
};