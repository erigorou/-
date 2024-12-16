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


/// <summary>
/// �R���X�g���N�^
/// </summary>
Fade::Fade(SceneManager* scene)
	: m_scene(scene)
	, m_totalTime(FADE_TIME)
	, m_isFade(false)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Fade::~Fade()
{
	// do nothing.
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
	m_totalTime = 0.0f;
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



void Fade::Render()
{
	using namespace DirectX;

	// �C�[�W���O�Ŏg�p����ׂ̕ϐ�	// 1�ɋ߂��Ɩ��邢
	float t = 0.0f;

	t = std::max(0.0f, 1.0f - (m_totalTime / FADE_TIME));
	t = std::max(0.0001f, Easing::easeInCubic(t));

	GameData::GetInstance()->SetFadeValue(t);

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

	//	�V�F�[�_�̊J�n
	m_customShader->BeginSharder(context);

	// �|���S���ŕ`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	//	�V�F�[�_�[�̉���
	m_customShader->EndSharder(context);


#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("t = %f" , t);
#endif // !_DEBUG

}

