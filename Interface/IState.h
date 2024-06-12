// -------------------------------
//
// ステートパターンの基礎
// プレイヤーと敵
//
// -------------------------------


#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED


class IState
{
public:
	virtual ~IState() = default;
	// 初期化する
	virtual void Initialize() = 0;
	// 事前更新する
	virtual void PreUpdate() = 0;
	// 更新する
	virtual void Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos) = 0;
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

#endif