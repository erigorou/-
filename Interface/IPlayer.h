#pragma once
#ifndef IPLAYER
#define IPLAYER

#include "pch.h"
#include "Interface/IState.h"

class IPlayer : public IState
{
public:
	// デストラクタ
	virtual ~IPlayer() = default;
	// 初期化する
	virtual void Initialize(DirectX::Model* model) = 0;
	// 事前更新する
	virtual void PreUpdate() = 0;
	// 更新する
	virtual void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) = 0;
	// キー入力を通知
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
	// 事後更新する
	virtual void PostUpdate() = 0;
	// 描画する
	virtual void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) = 0;
	// 後処理を行う
	virtual void Finalize() = 0;
};

#endif // !1

