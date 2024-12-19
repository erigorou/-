// ----------------------------------
// 「内容」敵のインターフェース
// 「持主」ゴブリン　ボス
// 「日付」12/18
// 「作成者」池田
// ----------------------------------

#include "pch.h"
#include "IObject.h"

class IEnemy : public IObject
{
	// デストラクタ
	virtual ~IEnemy() = default;
	// 初期化処理
	virtual void Initialize(DirectX::Model* model) = 0;
	// 更新処理
	virtual void Update(const float& elapsedTime) = 0;
	// 描画処理
	virtual void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) = 0;
	// 終了処理
	virtual void Finalize() = 0;
};
