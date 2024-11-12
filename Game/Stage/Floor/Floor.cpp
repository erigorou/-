// -----------------------------------
// ���v���~�e�B�u�ɂ��~�`�̒n�ʂ̕`�� + �g�̕`��
// -----------------------------------

#include "pch.h"
#include "Floor.h"
#include "Game/CommonResources.h"
#include "Game/Data/GameData.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"


// ------------
// �Œ�l
// ------------
const wchar_t* Floor::VS_PATH = L"Resources/Shaders/Voronoi/Voronoi_VS.cso";
const wchar_t* Floor::PS_PATH = L"Resources/Shaders/Voronoi/Voronoi_PS.cso";
const wchar_t* Floor::GS_PATH = L"Resources/Shaders/Voronoi/Voronoi_GS.cso";



// -------------------------------------------------------------------^
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// // ---------------------------------------------------------------^
Floor::Floor()
	: m_totalTime(0.0f)
{
	using namespace DirectX;
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// �G�t�F�N�g�̍쐬
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	// VoronoiShader�̐���
	m_voronoiShader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			InputElements
		);

	// �v���~�e�B�u�o�b�`�̍쐬
	m_Batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	// ���ʃX�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(device);

	// �e�N�X�`���̓ǂݍ���
	CustomShader::LoadTexture(device, L"Resources/Textures/Floor.png", m_texture);

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"�R���X�^���g�o�b�t�@�̐����Ɏ��s���܂���.", NULL, MB_OK);
	}
}


// ----------------------^
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ----------------------^
Floor::~Floor()
{
}


// -------------------------------------------
/// <summary>
/// �~�̒��_�𐶐�����
/// </summary>
/// <param name="vertices">���_�z��</param>
/// <param name="radius">�~�̔��a</param>
/// <param name="segments">�~�̕�����</param>
/// <param name="radius">�~�̔��a</param>
/// -------------------------------------------
void Floor::GenerateCircleVertices(DirectX::VertexPositionTexture* vertices, float radius, int segments)
{
	for (int i = 0; i < segments; ++i)
	{
		float angle = (2.0f * DirectX::XM_PI / segments) * i;
		vertices[i].position = DirectX::SimpleMath::Vector3(radius * cosf(angle), 0.0f, radius * sinf(angle));
		vertices[i].textureCoordinate = DirectX::SimpleMath::Vector2(cosf(angle) * 0.5f + 0.5f, sinf(angle) * 0.5f + 0.5f);
	}
}

// ---------------------------------------------
/// <summary>
/// ���̕`����s��
/// </summary>
/// <param name="context">�f�o�C�X�R���e�L�X�g</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
// ---------------------------------------------
void Floor::Render(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// ���ʂ̕`������Ȃ�
	context->RSSetState(m_states->CullCounterClockwise());

	// �s�����̂ݕ`�悷��ݒ�
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_texture[0].Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	DrawFloor();
	DrawSea();
}


// -----------------------------
/// <summary>
/// ���̕`��
/// </summary>
/// -----------------------------
void Floor::DrawFloor()
{
	// �n�ʕ`��p�̒��_���
	std::vector<DirectX::VertexPositionTexture> vertices(SEGMENTS);
	GenerateCircleVertices(vertices.data(), RADIUS, SEGMENTS);

	// ������������`��
	m_Batch->Begin();

	// �n�ʂ̉~�`�`��
	for (int i = 0; i < SEGMENTS; ++i)
	{
		m_Batch->DrawTriangle(vertices[i], vertices[(i + 1) % SEGMENTS], vertices[0]);
	}

	m_Batch->End();
}


// -----------------------------
/// <summary>
/// �C�̕`��
/// </summary>
/// -----------------------------
void Floor::DrawSea()
{
	using namespace DirectX;
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// ���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(
		SimpleMath::Vector3::Zero,
		SimpleMath::Vector4::One
		)
	};

	// �o�ߎ��Ԃ��擾
	m_totalTime += GameData::GetInstance()->GetElapsedTime();

	ConstBuffer cbuff;
	cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;
	cbuff.matView = DirectX::SimpleMath::Matrix::Identity;
	cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	cbuff.time = DirectX::SimpleMath::Vector4(m_totalTime, 0.0f, 0.0f, 0.0f);

	// �R���X�^���g�o�b�t�@�̐ݒ�
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

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

	m_voronoiShader->BeginSharder(context);

	context->IASetInputLayout(m_voronoiShader->GetInputLayout());
	
	m_Batch->Begin();
	m_Batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_Batch->End();

	m_voronoiShader->EndSharder(context);
}


// -----------------------------
/// <summary>
/// �I������
/// </summary>
// -----------------------------
void Floor::Finalize()
{
}
