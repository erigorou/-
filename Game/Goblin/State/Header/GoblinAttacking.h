#pragma once
#ifndef GOBLIN_ATTACKING
#define GOBLIN_ATTACKING

#include "pch.h"
#include "Interface/IState.h"

#include "../../Goblin.h"

class Goblin::GoblinAttacking : public IState
{
public:
	// コンストラクタ
	GoblinAttacking(Goblin* goblin);
	// デストラクタ
	~GoblinAttacking() override;
	// 初期化する
	void Initialize(DirectX::Model* model) override;
	// 事前更新する
	void PreUpdate() override;
	// 更新する
	void Update(const float& elapsedTime) override;
	// 事後更新する
	void PostUpdate() override;
	// 描画する
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;

	// 後処理を行う
	void Finalize() override;

private:
	Goblin* m_goblin;	// 親

	DirectX::Model* m_model;
	float m_totalTime;
};



#endif	// GOBLIN_ATTACKING