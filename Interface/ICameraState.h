#pragma once

#include "pch.h"

class ICameraState
{
public:
    virtual	~ICameraState()
    {}  // デストラクタ
	
    virtual void PreUpdate() = 0;		// ステート変更（in）

    virtual void Update(
        const DirectX::SimpleMath::Vector3& playerPos,
        const DirectX::SimpleMath::Vector3& enemyPos,
        const DirectX::SimpleMath::Matrix& rotate,
        float elapsedTime
    
    ) = 0;
	virtual void PostUpdate		() = 0;		// ステート変更（out)
};
