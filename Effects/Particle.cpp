//--------------------------------------------------------------------------------------
// File: Particle.cpp
//
// �p�[�e�B�N���N���X
// �@�����@�V�[���O���t
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Libraries/MyLib/BinaryFile.h"
#include "Effects/Particle.h"
#include "Effects/Header/DustTrailParticle.h"
#include "Effects/Header/SwordTrialParticle.h"

#include <random>



// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


// �R���X�g���N�^
Particle::Particle()
	:m_pDR(nullptr)
	,m_timerDustTrail(0.0f)
{
}


// �f�X�g���N�^
Particle::~Particle()
{
}


/// <summary>
/// �e�N�X�`�����\�[�X�̓ǂݍ��݊֐�
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
void Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}


/// <summary>
/// �����֐�
/// </summary>
void Particle::Create()
{
	// ComonResources���擾
	CommonResources* resources = CommonResources::GetInstance();
	// DeviceResources���擾
	m_pDR = resources->GetDeviceResources();
	// 1D3D11Device���擾
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	�V�F�[�_�[�̍쐬
	CreateShader();

	//	�摜�̓ǂݍ��݁i�Q���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/dust.png");
	LoadTexture(L"Resources/Textures/dust.png");

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(resources->GetDeviceResources()->GetD3DDeviceContext());
	//  �X�^�b�c�̍쐬
	m_states = std::make_unique<DirectX::CommonStates>(device);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTimer">�t���[���Ԃ̕b��</param>
/// <param name="playerPosition">�v���C���[�̍��W</param>
/// <param name="playerVelocity">�v���C���[�̑��x</param>
void Particle::Update(float elapsedTimer, const DirectX::SimpleMath::Vector3 playerPosition, DirectX::SimpleMath::Vector3 playerVelocity)
{
	// �ʒu�Ƒ��x���L�^����
	m_playerPosition = playerPosition;
	m_playerVelocity = playerVelocity;

	// �C�e���[�^���擾���āA�폜�Ώۂ̗v�f���폜
	m_dustTrail.remove_if([&elapsedTimer](DustTrialParticle& particle)
		{
			// �q�N���X����false�ŏ���
			return !particle.Update(elapsedTimer);
		}
	);

	// �C�e���[�^���擾���āA�폜�Ώۂ̗v�f���폜
	m_swordTrial.remove_if([&elapsedTimer](SwordTrialParticle& particle)
		{
			// �q�N���X����false�ŏ���
			return !particle.Update(elapsedTimer);
		}
	);
}


/// <summary>
/// �O�Ղ����ǂ�y���̐������s��
/// </summary>
/// <param name="elapsedTimer">�t���[���Ԃ̕b��</param>
void Particle::CreateTrailDust(float elapsedTimer)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// �p�[�e�B�N��(�P��)�𐶐�
	DustTrialParticle dTP(
		0.5f,																		//	��������(s)
		Vector3(m_playerPosition.x, m_playerPosition.y, m_playerPosition.z),		//	����W
		Vector3(-m_playerVelocity.x, 0.75f, -m_playerVelocity.z),					//	���x
		Vector3(1.0f, 0.1f, 1.0f),													//	�����x
		Vector3(2.0f, 2.0f, 2.0f), SimpleMath::Vector3(4.0f, 4.0f, 4.0f),			//	�����X�P�[���A�ŏI�X�P�[��
		Color(0.7f, 0.7f, 0.7f, 1.0f), SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.0f)	//	�����J���[�A�ŏI�J���[
	);

	// �z��ɒǉ�
	m_dustTrail.push_back(dTP);
	// �^�C�}�[�����Z�b�g
	m_timerDustTrail = 0.0f;
}



/// <summary>
/// ���������̎��ɏo��y���𐶐�����
/// </summary>
/// <param name="center">���������̈ʒu</param>
void Particle::CreateSlamDust(DirectX::SimpleMath::Vector3 center)
{
	using namespace DirectX;

	for (int i = 0; i < 25; i++)
	{
		// ���S�ȃ����_�����n�[�h�E�F�A�I�ɐ������邽�߂̃N���X�̕ϐ�
		std::random_device seed;
		// ��L�̊��S�ȃ����_���͓��삪�x�����߁Aseed�l�̌���݂̂Ɏg�p����
		// ���udefault_random_engine�v��using�Łumt19937�v�ƂȂ��Ă���
		std::default_random_engine engine(seed());
		// �Ƃ΂��ė~���������_���͈̔͂�Distribution�ɔC����B�����0�`2PI
		std::uniform_real_distribution<> dist(0, XM_2PI);
		float range = 5.0f;
		// �����_���Ȋp�x�𐶐�����
		float randAngle = static_cast<float>(dist(engine));

		std::uniform_real_distribution<> dist2(0.5f, 2.0f);
		float Yspeed = static_cast<float>(dist2(engine));
		float XZspeed = static_cast<float>(dist2(engine));

		// ���S����̃x�N�g���𐶐�
		SimpleMath::Vector3 vectorFromCenter = center + SimpleMath::Vector3(range * cosf(randAngle), 0.0f, range * sinf(randAngle)) - center;
		// �x�N�g���̒������擾�i�����j
		float distanceFromCenter = vectorFromCenter.Length();
		// ���S����̃x�N�g���𐳋K�����ĕ�����ێ�
		SimpleMath::Vector3 normalizedVectorFromCenter = vectorFromCenter / distanceFromCenter;
		// �x�N�g�����O���ɍL���邽�߂̃X�P�[����K�p
		float scaleFactor = 1.0f + (distanceFromCenter / range); // range�͉~�̔��a
		SimpleMath::Vector3 adjustedVelocity = normalizedVectorFromCenter * scaleFactor;
		// ���x�x�N�g���𐶐�
		SimpleMath::Vector3 velocity = -adjustedVelocity;

		// ���������_�X�g�̍��W���擾����
		SimpleMath::Vector3 dustPosition = center + SimpleMath::Vector3(range * cosf(randAngle), 0.0f, range * sinf(randAngle));    // ����W

		// �p�[�e�B�N������
		DustTrialParticle pB(
			1.0f,																				// ��������(s)
			dustPosition,																		// ����W
			SimpleMath::Vector3{ -velocity.x * XZspeed, Yspeed, -velocity.z * XZspeed } * 2,	// ���x
			SimpleMath::Vector3(0.1f, 0.1f, 0.1f),												// �����x
			SimpleMath::Vector3::One, SimpleMath::Vector3{ 10.0f, 25.0f, 10.0f },				// �����X�P�[���A�ŏI�X�P�[��
			SimpleMath::Color(1.f, 1.f, 1.f, 1.f),												// �����J���[
			SimpleMath::Color(1.f, 1.f, 1.f, 0.f)												// �ŏI�J���[
		);

		m_dustTrail.push_back(pB);
	}
}



/// <summary>
/// �����~�����Ƃ��̎c�����o��
/// </summary>
/// <param name="ver"></param>
void Particle::CreateSwordTrial(DirectX::VertexPositionTexture ver[4])
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// SwordTrialParticle�𐶐�
	SwordTrialParticle sTP(
		ver,															//	���_���
		0.4f,															//	��������(s)
		Color(1.0f, 1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 0.0f)	//	�����J���[�A�ŏI�J���[
	);

	// �z��ɒǉ�
	m_swordTrial.push_back(sTP);
}





/// <summary>
/// �V�F�[�_�[�𐶐�����
/// </summary>
void Particle::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSDataDust = BinaryFile::LoadFile(L"Resources/Shaders/Dust/DustVS.cso");
	BinaryFile GSDataDust = BinaryFile::LoadFile(L"Resources/Shaders/Dust/DustGS.cso");
	BinaryFile PSDataDust = BinaryFile::LoadFile(L"Resources/Shaders/Dust/DustPS.cso");

	BinaryFile VSDataSword = BinaryFile::LoadFile(L"Resources/Shaders/Sword/SwordTrialVS.cso");
	BinaryFile PSDataSword = BinaryFile::LoadFile(L"Resources/Shaders/Sword/SwordTrialPS.cso");

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSDataDust.GetData(), VSDataDust.GetSize(),
		m_inputLayout.ReleaseAndGetAddressOf());

	//device->CreateInputLayout(&INPUT_LAYOUT[1],
	//	static_cast<UINT>(INPUT_LAYOUT.size()),
	//	VSDataSword.GetData(), VSDataSword.GetSize(),
	//	m_inputLayout.ReleaseAndGetAddressOf());


	// ���_�V�F�[�_�[�A�W�I���g���V�F�[�_�[�A�s�N�Z���V�F�[�_�[�̐���
	if (FAILED(device->CreateVertexShader(VSDataDust.GetData(), VSDataDust.GetSize(), NULL, m_vertexShaderDust.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreateVS (Dust) Failed.", NULL, MB_OK);	return; }
	if (FAILED(device->CreateGeometryShader(GSDataDust.GetData(), GSDataDust.GetSize(), NULL, m_geometryShaderDust.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreateGS (Dust) Failed.", NULL, MB_OK);	return; }
	if (FAILED(device->CreatePixelShader(PSDataDust.GetData(), PSDataDust.GetSize(), NULL, m_pixelShaderDust.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreatePS (Dust) Failed.", NULL, MB_OK);	return; }

	// Sword�p�̃V�F�[�_�[�̐���
	if (FAILED(device->CreateVertexShader(VSDataSword.GetData(), VSDataSword.GetSize(), NULL, m_vertexShaderSword.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreateVS (Sword) Failed.", NULL, MB_OK);return; }
	if (FAILED(device->CreatePixelShader(PSDataSword.GetData(), PSDataSword.GetSize(), NULL, m_pixelShaderSword.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreatePS (Sword) Failed.", NULL, MB_OK);return; }



	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}


/// <summary>
/// Render���ʏ��������SwordTrialParticle��DustParticle�̕`��Ăяo��
/// </summary>
/// <param name="states"></param>
/// <param name="view"></param>
/// <param name="proj"></param>
void Particle::Render(DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();												// �J�����̕����𐳋K��

	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };			//	�T���v���[�X�e�[�g�̐ݒ�
	context->PSSetSamplers(0, 1, sampler);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();	//  �������`��w��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		//	�������菈��
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);		//	�[�x�o�b�t�@�͂Ȃ�
	context->RSSetState(m_states->CullNone());						//	�J�����O�Ȃ�

	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	for (int i = 0; i < m_texture.size(); i++)	{ context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf()); }

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	DrawSwordParticle(view, proj, cameraDir);	// ���̎c���̕`��
	DrawDustParticle(view, proj, cameraDir);	// �y���̕`��
}



/// <summary>
/// ���̎c���p�[�e�B�N���̕`��
/// �W�I���g���V�F�[�_�[��r���{�[�h���g�p������4���_���g�p����Quad��`�悵�܂��B
/// </summary>
/// <param name="view"></param>
/// <param name="proj"></param>
/// <param name="cameraDir"></param>
void Particle::DrawSwordParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 cameraDir)
{
	using namespace DirectX;

	// ���̃p�[�e�B�N���̂��߂̃R���X�^���g�o�b�t�@�̍쐬�ƍX�V
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = SimpleMath::Matrix::Identity; // ���̃p�[�e�B�N���ł̓r���{�[�h��K�p���Ȃ�
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	context->VSSetShader(m_vertexShaderSword.Get(), nullptr,0);	// ���_�V�F�[�_�[��ݒ�
	context->GSSetShader(nullptr, nullptr, 0);					// �W�I���g���V�F�[�_�[�͎g�p���Ȃ�
	context->PSSetShader(m_pixelShaderSword.Get(), nullptr, 0);	// �s�N�Z���V�F�[�_�[��ݒ�

	// ���̎c���p�[�e�B�N����`��
	m_batch->Begin();

	float t = 0.0f;
	for (auto& sTP : m_swordTrial)
	{
		// ���̃p�[�e�B�N����4�̒��_������
		DirectX::VertexPositionColorTexture ver[4];
		sTP.GetVertices(ver);
		
		float value1 = 1 - pow(1 - t, 2);	// �C�[�W���O
		t += 1.0f / m_swordTrial.size();
		float value2 = 1 - pow(1 - t, 2);	// �C�[�W���O	// t �� 1�ɋ߂Â��قǐF�������Ȃ�

		ver[1].color = DirectX::SimpleMath::Color(1, 0, 0, value1);
		ver[2].color = DirectX::SimpleMath::Color(1, 0, 0, value1);

		ver[0].color = DirectX::SimpleMath::Color(1, 0, 0, value2);
		ver[3].color = DirectX::SimpleMath::Color(1, 0, 0, value2);

		m_batch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	}
	m_batch->End();

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}



/// <summary>
/// �y���p�[�e�B�N���̕`��
/// �W�I���g���V�F�[�_�[�ƃr���{�[�h��K�p���ĕ`�悵�܂��B
/// </summary>
/// <param name="view"></param>
/// <param name="proj"></param>
/// <param name="cameraDir"></param>
void Particle::DrawDustParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 cameraDir)
{
	using namespace DirectX;

	// �y���p�[�e�B�N���̂��߂̃R���X�^���g�o�b�t�@�̍쐬�ƍX�V
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose(); // �y���p�[�e�B�N���ł̓r���{�[�h��K�p����
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	context->VSSetShader(m_vertexShaderDust  .Get(), nullptr, 0);
	context->GSSetShader(m_geometryShaderDust.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShaderDust   .Get(), nullptr, 0);

	// �y���p�[�e�B�N���̒��_���X�g���N���A
	m_dustVertices.clear();
	for (DustTrialParticle& li : m_dustTrail)
	{
		// �J�����̕����Ɍ����Ȃ��p�[�e�B�N���̓X�L�b�v
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f)
			continue;

		// �p�[�e�B�N���̌��݂̏���ݒ�
		VertexPositionColorTexture vPCT;
		vPCT.position = XMFLOAT3(li.GetPosition());
		vPCT.color = XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_dustVertices.push_back(vPCT);
	}

	// �p�[�e�B�N�������݂���ꍇ�A�`����s��
	if (!m_dustVertices.empty())
	{
		// �J�����Ƃ̋����Ɋ�Â��ăp�[�e�B�N�����\�[�g
		std::sort(m_dustVertices.begin(), m_dustVertices.end(),
			[&](VertexPositionColorTexture lhs, VertexPositionColorTexture rhs)
			{
				return cameraDir.Dot(lhs.position - m_cameraPosition) > cameraDir.Dot(rhs.position - m_cameraPosition);
			});

		// �p�[�e�B�N���̕`��
		m_batch->Begin();
		m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_dustVertices[0], m_dustVertices.size());
		m_batch->End();
	}

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}




/// <summary>
/// �r���{�[�h�쐬�֐�
/// </summary>
/// <param name="target">�J�����^�[�Q�b�g�i�����_�j</param>
/// <param name="eye">�J�����A�C�i�J�������W�j</param>
/// <param name="up">������x�N�g���i��{��Y�݂̂P�̃x�N�g���j</param>
void Particle::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	using namespace DirectX;

	// �p�[�e�B�N�����J�����Ɍ����悤�Ƀr���{�[�h�s����쐬����
	m_billboard =
		SimpleMath::Matrix::CreateBillboard
		(SimpleMath::Vector3::Zero, eye - target, up);

	// �r���{�[�h�𐅕�����ѐ����ɔ��]�������]�s����쐬����
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1; // x���𔽓]
	rot._33 = -1; // z���𔽓]

	// �J�����̈ʒu�ƃ^�[�Q�b�g��ۑ�����i�����I�Ɏg�p���邽�߁j
	m_cameraPosition = eye;
	m_cameraTarget = target;

	// ��]�s����r���{�[�h�s��ɓK�p����
	m_billboard = rot * m_billboard;
}