//--------------------------------------------------------------------------------------
//	File: Particle.h
//
//	�p�[�e�B�N���N���X
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Fade.h"
#include "Libraries/MyLib/BinaryFile.h"
#include "Game/Scene/SceneManager.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/Data/GameData.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

///	<summary>
///	�C���v�b�g���C�A�E�g
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const wchar_t* Fade::TEXTURE_PATH	= L"Resources/Textures/Load.png";
const wchar_t* Fade::VS_PATH		= L"Resources/Shaders/Fade/FadeVS.cso";
const wchar_t* Fade::GS_PATH		= L"Resources/Shaders/Fade/FadeGS.cso";
const wchar_t* Fade::PS_PATH		= L"Resources/Shaders/Fade/FadePS.cso";

const wchar_t* Fade::STENCIL_TEX_PATH = L"Resources/Textures/mask.png";
const wchar_t* Fade::MASK_VS_PATH = L"Resources/Shaders/Mask/MaskVS.cso";
const wchar_t* Fade::MASK_GS_PATH = L"Resources/Shaders/Mask/MaskGS.cso";
const wchar_t* Fade::MASK_PS_PATH = L"Resources/Shaders/Mask/MaskPS.cso";


/// <summary>
/// �R���X�g���N�^
/// </summary>
Fade::Fade(SceneManager* scene)
	: m_scene(scene)
	, m_totalTime(0.0f)
	, m_isFade(false)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Fade::~Fade()
{
}


/// <summary>
/// �������֐�
/// </summary>
void Fade::Initialize()
{
	// �R�������\�[�X�̎擾�i�C���X�^���X�j
	CommonResources* commonResources = CommonResources::GetInstance();
	// �f�o�C�X���\�[�X�̎擾
	m_pDR = commonResources->GetDeviceResources();
	auto device = m_pDR->GetD3DDevice();

	m_customShader->LoadTexture(device, TEXTURE_PATH, m_texture);
	// �V�F�[�_�[�̐���
	CreateShader();
	// �v���~�e�B�u�o�b�`�̐���
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());
	// �R�����X�e�[�g�̐���
	m_states = std::make_unique<DirectX::CommonStates>(m_pDR->GetD3DDevice());
	// �^�����摜�̓ǂݍ���
	m_customShader->LoadTexture(device, STENCIL_TEX_PATH, m_stencilImage);
}


/// <summary>
/// �V�F�[�_�[�쐬�����p�֐�
/// </summary>
void Fade::CreateShader()
{
	// �f�o�C�X�̎擾
	ID3D11Device* device = m_pDR->GetD3DDevice();

	// �J�X�^���V�F�[�_�[�̏�����
	m_customShader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			INPUT_LAYOUT
		);

	// �}�X�N�p�J�X�^���V�F�[�_�[�̏�����
	m_maskShader = std::make_unique<CustomShader>
		(
			device,
			MASK_VS_PATH,
			MASK_PS_PATH,
			MASK_GS_PATH,
			INPUT_LAYOUT
		);

	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= sizeof(ConstBuffer);
	bd.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags	= 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

/// <summary>
/// �t�F�[�h�C���̊J�n����
/// </summary>
/// <param name="type"></param>
void Fade::StartFadeIn()
{
	m_isFade = true;
	m_totalTime = 0;
	m_fadeType = FadeType::FADE_IN;
}


void Fade::StartFadeOut()
{
	m_isFade = true;
	m_totalTime = FADE_TIME;
	m_fadeType = FadeType::FADE_OUT;
}

void Fade::FadeStop()
{
	m_isFade = false;
	m_fadeType = FadeType::FADE_NONE;
}



void Fade::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime;

	// �t�F�[�h���łȂ��ꍇ�͉������Ȃ�
	if ( ! m_isFade) return;

	// �^�C�}�[�̃J�E���g
	CountTimer();
}


void Fade::CountTimer()
{
	switch (m_fadeType)
	{
	case FadeType::FADE_IN:		FadeIn();	break;
	case FadeType::FADE_OUT:	FadeOut();	break;
	
	default:								break;
	}
}



/// <summary>
/// �t�F�[�h�C���̏���
/// </summary>
void Fade::FadeIn()
{
	// ���Ԃ��v�Z����( 0 �� 1 )
	m_totalTime = std::min(m_totalTime + m_elapsedTime, FADE_TIME);

	if (m_totalTime >= FADE_TIME)
	{
		m_totalTime = FADE_TIME;
		m_isFade = false;
	}
	else if (m_totalTime >= CHANGE_SCENE_TIME)
	{
		// �V�[���ύX��������
		m_scene->SetCanChangeScene();
	}
}


/// <summary>
/// �t�F�[�h�A�E�g����
/// </summary>
void Fade::FadeOut()
{
	// ���Ԃ��v�Z����( 1 �� 0 )
	m_totalTime = std::max(m_totalTime - m_elapsedTime, 0.0f);

	if (m_totalTime <= 0.0f)
	{
		m_totalTime = 0.0f;
		m_isFade = false;
		// �V�[���ύX��������
		m_scene->SetCanChangeScene();
	}
}

void Fade::DrawStencilImage()
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* context = m_pDR->GetD3DDeviceContext();

	//	�`��ɂ��Ă̐ݒ���s��
	D3D11_TEXTURE2D_DESC texDesc;
	m_pDR->GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_pDR->GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_capture.ReleaseAndGetAddressOf());

	//	�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//	�����_�[�^�[�Q�b�g�r���[�̐���
	m_pDR->GetD3DDevice()->CreateRenderTargetView(m_capture.Get(), &rtvDesc, &m_captureRTV);

	//	�V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//	�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�r���[���擾�i��Ō��ɖ߂����߁j
	ID3D11RenderTargetView* defaultRTV = m_pDR->GetRenderTargetView();
	ID3D11DepthStencilView* pDSV = m_pDR->GetDepthStencilView();

	// �w�i�F�̎擾(����)
	float backColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// �����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���̐ݒ�
	context->OMSetRenderTargets(1, m_captureRTV.GetAddressOf(), pDSV);
	context->ClearRenderTargetView(m_captureRTV.Get(), backColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ---�I�u�W�F�N�g�̕`��--- ��

	m_maskShader->BeginSharder(context);

	// �t�F�[�h�̎��Ԃ� 0 ~ 1�Ő��K��������
	float t = m_totalTime / FADE_TIME;

	// �t�F�[�h�̃T�C�Y���v�Z
	float size = CalcrateFadeValue(t);

	DirectX::SimpleMath::Matrix world;
	world = DirectX::SimpleMath::Matrix::CreateScale(size, size, 1.0f);

	// �V�F�[�_�[�ɓn�����̔}�̂��쐬����
	ConstBuffer cbuff;

	// �n���f�[�^��o�^����
	cbuff.matView = DirectX::SimpleMath::Matrix::Identity;						// �X�N���[�����W�ŕ`�悷�邽�ߕs�v
	cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;						// �X�N���[�����W�ŕ`�悷�邽�ߕs�v	
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4::One;							// �e�N�X�`���̐F
	cbuff.time = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);				// �t�F�[�h�̎��ԁi���v�f�̂ݎg�p�j

	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* ccb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, ccb);
	context->GSSetConstantBuffers(0, 1, ccb);
	context->PSSetConstantBuffers(0, 1, ccb);

	//	�`�悷��I�u�W�F�N�g�̏���ݒ�
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	for (int i = 0; i < m_texture.size(); i++)
		context->PSSetShaderResources(i, 1, m_stencilImage[i].GetAddressOf());

	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();


	//	�`�悵����ʂ�captureSRV�ɕۑ�����
	HRESULT hr = m_pDR->GetD3DDevice()->CreateShaderResourceView
	(
		m_capture.Get(), &srvDesc, m_captureSRV.ReleaseAndGetAddressOf()
	);

	m_maskShader->EndSharder(context);

	//	�ێ����Ă������f�t�H���g�ݒ�ɖ߂��āA��ʕ`�悪����ɏo����悤�ɂ��Ă���
	backColor[0] = 0.3f;
	backColor[1] = 0.3f;
	backColor[2] = 0.3f;
	backColor[3] = 0.0f;
	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);
}



void Fade::Render()
{
	using namespace DirectX;

	// �^�����摜�̕`��
	DrawStencilImage();

	float t = 0.0f;
	t = std::max(0.0f, 1.0f - (m_totalTime / FADE_TIME));
	t = std::max(0.0001f, Easing::easeInCubic(t));
	GameData::GetInstance()->SetFadeValue(1 - t);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();

	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(SimpleMath::Vector3(0.0f, 0.0f, 0.0f),SimpleMath::Vector4::One, SimpleMath::Vector2(0.0f, 0.0f)),
	};

	// �V�F�[�_�[�ɓn�����̔}�̂��쐬����
	ConstBuffer cbuff;

	// �n���f�[�^��o�^����
	cbuff.matView = SimpleMath::Matrix::Identity;						// �X�N���[�����W�ŕ`�悷�邽�ߕs�v
	cbuff.matProj = SimpleMath::Matrix::Identity;						// �X�N���[�����W�ŕ`�悷�邽�ߕs�v	
	cbuff.matWorld = SimpleMath::Matrix::Identity;						// �X�N���[�����W�ŕ`�悷�邽�ߕs�v	
	cbuff.Diffuse = SimpleMath::Vector4::One;							// �e�N�X�`���̐F
	cbuff.time = SimpleMath::Vector4(t, 0.0f, 0.0f, 0.0f);				// �t�F�[�h�̎��ԁi���v�f�̂ݎg�p�j

	//	�V�F�[�_�̊J�n
	m_customShader->BeginSharder(context);

	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* ccb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, ccb);
	context->GSSetConstantBuffers(0, 1, ccb);
	context->PSSetConstantBuffers(0, 1, ccb);

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	//	�������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	//	�J�����O�͐��ʂ̂ݍs��
	context->RSSetState(m_states->CullCounterClockwise());


	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	for (int i = 0; i < m_texture.size(); i++)
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());

	context->PSSetShaderResources(m_texture.size(), 1, m_captureSRV.GetAddressOf());


	// �|���S���ŕ`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	//	�V�F�[�_�[�̉���
	m_customShader->EndSharder(context);


#ifdef _DEBUG
#endif // !_DEBUG

}



// ----------------------------------
// �t�F�[�h�̒l���v�Z����
// ----------------------------------
float Fade::CalcrateFadeValue(float t)
{
	if (FadeType::FADE_IN == m_fadeType)
	{
		return Easing::easeInOutExpo(t) * 6;
	}
	else if (FadeType::FADE_OUT == m_fadeType)
	{
		return Easing::easeInOutExpo(t) * 6;
	}
}