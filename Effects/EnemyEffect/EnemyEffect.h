#pragma once
#include "pch.h"

class CustomShader;

class EnemyEffect
{
	// ----------------------------
	// 定数や構造体
	// ----------------------------
public:
	// 死亡時のエフェクトの時間
	static constexpr float DEAD_TIME = 2.5f;
	// ダメージ時のエフェクトの時間
	static constexpr float DAMAGE_TIME = 0.5f;
	// 死亡時のシェーダーパス
	static constexpr wchar_t DEAD_PS_PATH[] = L"Resources/cso/DeadEffectPS.cso";
	static constexpr wchar_t DEAD_VS_PATH[] = L"Resources/cso/DeadEffectVS.cso";
	// ダメージのシェーダーパス
	static constexpr wchar_t DAMAGE_PS_PATH[] = L"Resources/cso/DamageEffectPS.cso";
	static constexpr wchar_t DAMAGE_VS_PATH[] = L"Resources/cso/DamageEffectVS.cso";
	// 頂点入力レイアウト
	std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	
	// 定数バッファ
	struct ConstBuffer
	{
		float Time; // 時間
		DirectX::SimpleMath::Vector3 Padding;
	};

	enum class ENEMY_EFFECT
	{
		NONE,
		DAMAGE,
		DEAD
	};

	// ----------------------------
	// アクセサ
	// ----------------------------
public:
	// どのエフェクトを適用するか
	void SetEffect(ENEMY_EFFECT effect);

	// ----------------------------
	// メンバ関数（公開）
	// ----------------------------
public:
	// コンストラクタ
	EnemyEffect();
	// デストラクタ
	~EnemyEffect();
	// 更新処理
	void Update(float elapsedTime);
	// 体パーツの描画
	void DrawWithEffect(
		DirectX::Model* model,
		const DirectX::SimpleMath::Matrix world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);


	// ----------------------------
	// メンバ関数（非公開）
	// ----------------------------
private:
	// シェーダーの生成
	void CreateShader();
	// 定数バッファの作成
	void CreateConstBuffer();
	// 定数バッファの更新処理
	void UpdateConstBuffer(ConstBuffer* cb);


	// ----------------------------
	// メンバ変数
	// ----------------------------
private:
	// 経過時間
	float m_totalTime;
	// 死亡時のシェーダー
	std::unique_ptr<CustomShader> m_deadShader;
	// ダメージ時のシェーダー
	std::unique_ptr<CustomShader> m_damageShader;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	// 現在のエフェクト
	ENEMY_EFFECT m_currentEffect;
};