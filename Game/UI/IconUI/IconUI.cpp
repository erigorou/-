//--------------------------------------------------------------------------------------
//
// ���O:	IconUI.cpp
// ���e:	�A�C�R��UI�̃N���X
//			�摜�̕`��Ȃǂ��ȒP�ɍs����
// �쐬:	�r�c����
//
//-------------------------------------------------------------------------------------
// �C���N���[�h
#include "pch.h"
#include "IconUI.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "DeviceResources.h"


// -------------------------------------------------------
// �萔
// -------------------------------------------------------

// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> IconUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// -------------------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// -------------------------------------------------------
IconUI::IconUI()
	:
	m_windowHeight{}
	, m_windowWidth{}
	, m_textureHeight{}
	, m_textureWidth{}
	, m_texture(nullptr)
	, m_scale(DirectX::SimpleMath::Vector2::One)
	, m_position(DirectX::SimpleMath::Vector2::Zero)
	, m_anchor(ANCHOR::TOP_LEFT)
{
}

// -------------------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// -------------------------------------------------------
IconUI::~IconUI()
{
}

// -------------------------------------------------------
/// <summary>
/// ��������
/// </summary>
/// <param name="position">���W</param>
/// <param name="scale">�g��{��</param>
/// <param name="anchor">�A���J�[�|�C���g</param>
// -------------------------------------------------------
void IconUI::Create(
	DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, ANCHOR anchor)
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	���W�A�g��{���A�A���J�[�|�C���g�̐ݒ�
	m_position = position;
	m_baseScale = m_scale = scale;
	m_anchor = anchor;

	//	�V�F�[�_�[�̍쐬
	CreateShader();

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	m_states = std::make_unique<DirectX::CommonStates>(device);
}


// -------------------------------------------------------
/// <summary>
/// Shader�쐬�����������������֐�
/// </summary>
// -------------------------------------------------------
void IconUI::CreateShader()
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�̐���
	m_customShader = std::make_unique<CustomShader>(
		device,
		VS_PATH,
		PS_PATH,
		GS_PATH,
		INPUT_LAYOUT
	);

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

// -------------------------------------------------------
/// <summary>
/// �`��֐�
/// </summary>
// -------------------------------------------------------
void IconUI::Render()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// ���_���
	// Position.xy	:�g�k�p�X�P�[��
	// Position.z	:�A���J�[�^�C�v(0�`8)�̐����Ŏw��
	// Color.xy�@	:�A���J�[���W(�s�N�Z���w��:1280 �~720)
	// Color.zw		:�摜�T�C�Y
	// Tex.xy		:�E�B���h�E�T�C�Y�i�o�b�t�@�������B������͖��g�p�j
	DirectX::VertexPositionColorTexture vertex[1] = {
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor)),
			DirectX::SimpleMath::Vector4(m_position.x, m_position.y,static_cast<float>(m_textureWidth),static_cast<float>(m_textureHeight)),
			DirectX::SimpleMath::Vector2(static_cast<float>(m_windowWidth),
			static_cast<float>(m_windowHeight)))
	};
	//	��������L�̐ݒ�l�ɂ́AWorld��View�Ȃǂ�3D��Ԃ���ϊ����邽�߂̌v�Z����؂��Ȃ����߁A
	//	�X�N���[�����W�Ƃ��ĕ`�悳���

	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);

	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	�������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	//	�J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	// �V�F�[�_�[�̓o�^
	m_customShader->BeginSharder(context);

	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//	�|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//	�V�F�[�_�[�̏I������
	m_customShader->EndSharder(context);
}