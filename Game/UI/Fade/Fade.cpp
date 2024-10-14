//--------------------------------------------------------------------------------------
//	File: Particle.h
//
//	パーティクルクラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Fade.h"

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const wchar_t* Fade::TEXTURE_PATH = L"Assets/Textures/fade.png";


Fade::Fade()
	: 
	m_totalTime(0.0f)

{
}