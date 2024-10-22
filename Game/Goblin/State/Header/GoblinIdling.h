#pragma once
#ifndef GOBLIN_IDLING
#define GOBLIN_IDLING

#include "pch.h"
#include "../../Goblin.h"


class Goblin::GoblinIdling : public IState
{

// 共通
public:

	GoblinIdling(Goblin* goblin);

	~GoblinIdling() override;

	// 初期化する
	void Initialize(DirectX::Model* model) override;

	// 事前更新する
	void PreUpdate() override;

	// 更新する
	void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) override;

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


// メンバ
private:





	// 固定値



	Goblin* m_goblin;	// 親
};


#endif