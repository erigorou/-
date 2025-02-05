#pragma once
#ifndef GOBLIN_IDLING
#define GOBLIN_IDLING

#include "pch.h"
#include "Interface/IState.h"
class Goblin;

class GoblinIdling : public IState
{
	// -----------0-
	// 固定値
	// -------------
public:
	static constexpr float MIN_COOLTIME = 1.0f;
	static constexpr float MAX_COOLTIME = 3.0f;

	// -----------0-
	// 公開関数
	// -------------
public:
	// コンストラクタ
	GoblinIdling(Goblin* goblin);
	// デストラクタ
	~GoblinIdling() override;
	// 初期化する
	void Initialize() override;
	// 事前更新する
	void PreUpdate() override;
	// 更新する
	void Update(const float& elapsedTime) override;
	// 事後更新する
	void PostUpdate() override;
	// 後処理を行う
	void Finalize() override;
	// プレイヤーを探す
	void SearchPlayer();


private:
	Goblin* m_goblin;	// 親
	float m_TotalTime;	// 経過時間

	float m_coolTime;	// クールタイム

	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 角度
	float m_angle;
	// 回転行列
	DirectX::SimpleMath::Matrix m_rotMatrix;
};

#endif