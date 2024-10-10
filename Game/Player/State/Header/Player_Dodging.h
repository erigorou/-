#pragma once
#ifndef PLAYER_DODGING_DEFINED
#define PLAYER_DODGING_DEFINED
// -----------------------------
//
// プレイヤーの待機状態
//
// -----------------------------


#pragma once
#include "pch.h"
#include "Interface/IState.h"

class Player;
class Enemy;

class PlayerDodging : public IState
{

private:
	/////////////////////////固定値///////////////////////////////
	static const float DODGING_TIME;		// 回避全体の時間
	static const float DODGE_FUNCTION;		// 回避時の摩擦
	static const int   TRUNCATION_DIGIT;	// 少数〇〇桁以下削除

	static constexpr float DODGING_SPEED = 3.0f;	// 回避時の速度

public:
	// デストラクタ
	~PlayerDodging() override;
	// 初期化する
	void Initialize(DirectX::Model* model) override;
	// 事前更新する
	void PreUpdate() override;
	// 更新処理する
	void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) override;
	// 事後更新する
	void PostUpdate() override;
	// 描画する
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) override;
	// 終了処理
	void Finalize() override;

	// コンストラクタ
	PlayerDodging(Player* player);

private:
	// 総時間
	float m_totalSeconds;
	// 終了時間
	float m_finishTime;

	// 回避方向を取得するための向き
	DirectX::SimpleMath::Vector3 m_direction;
	// プレイヤーの移動速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// プレイヤー
	Player* m_player;
	// モデル
	DirectX::Model* m_model;

};

#endif  // PLAYER_DODGING_DEFINED