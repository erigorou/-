#pragma once

#ifndef I_RESULT_SCENE_DEFINED
#define I_RESULT_SCENE_DEFINED

class IResultScene
{
public:
	// デストラクタ
	virtual ~IResultScene() = default;
	// 初期化処理
	virtual void Initialize() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render() = 0;
};

#endif // I_SCENE_DEFINED