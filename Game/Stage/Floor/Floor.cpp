// -----------------------------------
// ���v���~�e�B�u�ɂ��n�ʂ̕`��
// -----------------------------------

#include "pch.h"
#include "Floor.h"

// -------------------------------------------------------------------^
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="device">�f�o�C�X�̃|�C���^</param>
/// // ---------------------------------------------------------------^
Floor::Floor(ID3D11Device1* device)
{
	using namespace DirectX;

	//	�G�t�F�N�g�̍쐬
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	//	���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	//	���ʃX�e�[�g�̍쐬
	m_states = std::make_unique<CommonStates>(device);

	//	�e�N�X�`���̃��[�h
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/floor.png",
		nullptr,
		m_texture.GetAddressOf()
	);
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
/// ���̕`����s��
/// </summary>
/// <param name="context">�f�o�C�X�R���e�L�X�g</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
// ---------------------------------------------
void Floor::Render(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// �v���~�e�B�u�o�b�`�̍쐬
	m_Batch =
		std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	// ���_���i�|���S���̒��_�j
	VertexPositionTexture vertex[4] =
	{
		// ���W					�摜��UV���W�F���ʂ�0�Őݒ�.�������ꍇ�͌J��Ԃ�
		VertexPositionTexture(Vector3(100.0f, 0.0f, 100.0f),	Vector2(-1.0f, -1.0f)),
		VertexPositionTexture(Vector3(-100.0f, 0.0f, 100.0f),	Vector2(1.0f, -1.0f)),
		VertexPositionTexture(Vector3(-100.0f, 0.0f, -100.0f),	Vector2(1.0f,  1.0f)),
		VertexPositionTexture(Vector3(100.0f,0.0f, -100.0f),	Vector2(-1.0f, 1.0f)),
	};

	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// �J�����O�͍�����i�����v���j
	context->RSSetState(m_states->CullNone());

	// �s�����̂ݕ`�悷��ݒ�
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->SetTexture(m_texture.Get());
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	// ������������`��
	m_Batch->Begin();
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}


// -----------------------------
/// <summary>
/// �I������
/// </summary>
// -----------------------------
void Floor::Finalize()
{
}