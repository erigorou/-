// -------------------------------------------------------------- //
//
//      Ç©Ç®ÅB
//
// -------------------------------------------------------------- //

#pragma once
#include "pch.h"

class IFace
{
public:
	virtual ~IFace() = default;

	virtual void DrawFace(
		const DirectX::SimpleMath::Matrix mat,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	) = 0;
};
