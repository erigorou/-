
#include "pch.h"
#include "../Header/OperateUI.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "CommonStates.h"


// -----------------------------------
// �R���X�g���N�^
// -----------------------------------
OperateUI::OperateUI(const wchar_t* texturePath)
	: m_texturePath(texturePath)
	, m_pDR(nullptr)
	, m_customShader(nullptr)
	, m_CBuffer(nullptr)
	, m_states(nullptr)
	, m_batch(nullptr)
	, m_texture()
	, m_elapsedTime(0.01667f * 10.0f)
	, m_totalTime(0.0f)
	, m_downKey(false)
{
	m_pDR = CommonResources::GetInstance()->GetDeviceResources();
}


// -----------------------------------
// �f�X�g���N�^
// -----------------------------------
OperateUI::~OperateUI()
{
}


// -----------------------------------
// ����������
// -----------------------------------
void OperateUI::Initialize()
{
	ID3D11Device* device = m_pDR->GetD3DDevice();

	// �V�F�[�_�[�̐���
	m_customShader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			InputElements
		);

	// �v���~�e�B�u�o�b�`�̐���
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>
		(
			m_pDR->GetD3DDeviceContext()
		);

	// �R�����X�e�[�g�̐���
	m_states = std::make_unique<DirectX::CommonStates>(device);

	// �e�N�X�`���̓ǂݍ���
	CustomShader::LoadTexture(device, m_texturePath, m_texture);

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"�R���X�^���g�o�b�t�@�̐����Ɏ��s���܂���.", NULL, MB_OK);
	}
}


void OperateUI::Render()
{
	using namespace DirectX;
	ID3D11DeviceContext* context = m_pDR->GetD3DDeviceContext();

	if (m_downKey)
	{
		m_totalTime -= m_elapsedTime;

		if (m_totalTime <= 0.0f)
			m_downKey = false;
	}


	// ���_���
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(
		SimpleMath::Vector3::Zero,
		SimpleMath::Vector4::One,
		XMFLOAT2(0.0f, 0.0f)
		)
	};

	// �o�b�t�@�̍쐬
	ConstBuffer cbuff;
	cbuff.matWorld = SimpleMath::Matrix::Identity;
	cbuff.matView = SimpleMath::Matrix::Identity;
	cbuff.matProj = SimpleMath::Matrix::Identity;
	cbuff.diffuse = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	cbuff.easing = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	cbuff.time = SimpleMath::Vector4(m_totalTime, 0.0f, 0.0f, 0.0f);

	// �R���X�^���g�o�b�t�@�̐ݒ�
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	// �V�F�[�_�[�̊J�n
	m_customShader->BeginSharder(context);

	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	context->RSSetState(m_states->CullNone());

	// �e�N�X�`���̐ݒ�
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	context->IASetInputLayout(m_customShader->GetInputLayout());

	// �`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	m_customShader->EndSharder(context);
}


void OperateUI::Finalize()
{
	m_customShader.reset();
	m_batch.reset();
	m_states.reset();
	m_texture.clear();
	m_CBuffer.Reset();
}



void OperateUI::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}


void OperateUI::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	m_downKey = true;
	m_totalTime = 1.0f;

	UNREFERENCED_PARAMETER(key);
}



void OperateUI::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}



DirectX::SimpleMath::Vector3 OperateUI::GetPosition()
{
	return DirectX::SimpleMath::Vector3::Zero;
}