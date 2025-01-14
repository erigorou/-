// ----------------------------------
// 「内容」敵のインターフェース
// 「持主」ゴブリン　ボス
// 「日付」12/18
// 「作成者」池田
// ----------------------------------
#pragma once
#include "pch.h"
#include "IObject.h"
#include "Game/Data/HPSystem.h"

class IEnemy : public IObject
{
public:
	// デストラクタ
	virtual ~IEnemy() = default;
	// 初期化処理
	virtual void Initialize() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) = 0;
	// 終了処理
	virtual void Finalize() = 0;


	// 当たったときにtrueを設定する
	virtual void HitAction(InterSectData data) = 0;
	// 位置を取得する
	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;

	// HPSystemを渡す
	virtual HPSystem* GetHPSystem() = 0;
};
