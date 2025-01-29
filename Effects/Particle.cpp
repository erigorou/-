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
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Game/Messenger/EventMessenger.h"

#include "Effects/Header/DustTrailParticle.h"
#include "Effects/Header/SwordTrailParticle.h"

#include <random>



// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// ---------------------------
// �R���X�g���N�^
// ---------------------------
Particle::Particle()
	:
	m_pDR(nullptr),
	m_timerDustTrail(0.0f)
{
	// �C�x���g��o�^
	EventMessenger::Attach("CreateTrailDust", std::bind(&Particle::CreateTrailDust, this));
	EventMessenger::Attach("CreateBashDust", std::bind(&Particle::CreateBashDust, this, std::placeholders::_1));
	EventMessenger::Attach("CreateSwordTrail", std::bind(&Particle::CreateSwordTrail, this, std::placeholders::_1));
}


// ---------------------------
// �f�X�g���N�^
// ---------------------------
Particle::~Particle()
{
}


// ---------------------------
// �e�N�X�`�����\�[�X�̓ǂݍ��݊֐�
// ---------------------------
void Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}


// ---------------------------
// �����֐�
// ---------------------------
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
	LoadTexture(TEXTURE_PATH);

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(resources->GetDeviceResources()->GetD3DDeviceContext());
	//  �X�^�b�c�̍쐬
	m_states = std::make_unique<DirectX::CommonStates>(device);
}

// ---------------------------
// �X�V����
// ---------------------------
void Particle::Update(float elapsedTimer, const DirectX::SimpleMath::Vector3 playerPosition, DirectX::SimpleMath::Vector3 playerVelocity)
{
	// �ʒu�Ƒ��x���L�^����
	m_playerPosition = playerPosition;
	m_playerVelocity = playerVelocity;

	// �C�e���[�^���擾���āA�폜�Ώۂ̗v�f���폜
	m_dustTrail.remove_if([&elapsedTimer](DustTrailParticle& particle)
		{
			// �q�N���X����false�ŏ���
			return !particle.Update(elapsedTimer);
		}
	);

	// �C�e���[�^���擾���āA�폜�Ώۂ̗v�f���폜
	m_swordTrail.remove_if([&elapsedTimer](SwordTrailParticle& particle)
		{
			// �q�N���X����false�ŏ���
			return !particle.Update(elapsedTimer);
		}
	);
}


// ---------------------------
// �O�Ղ����ǂ�y���̐������s��
// ---------------------------
void Particle::CreateTrailDust()
{
	// �p�[�e�B�N��(�P��)�𐶐�
	DustTrailParticle dTP(
		0.5f,																									//	��������(s)
		DirectX::SimpleMath::Vector3(m_playerPosition.x, m_playerPosition.y, m_playerPosition.z),				//	����W
		DirectX::SimpleMath::Vector3(-m_playerVelocity.x, 0.75f, -m_playerVelocity.z),							//	���x
		DirectX::SimpleMath::Vector3(1.0f, 0.1f, 1.0f),															//	�����x
		DirectX::SimpleMath::Vector3(4.0f, 4.0f, 4.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),			//	�����X�P�[���A�ŏI�X�P�[��
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f), DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.0f)	//	�����J���[�A�ŏI�J���[
	);

	// �z��ɒǉ�
	m_dustTrail.push_back(dTP);
	// �^�C�}�[�����Z�b�g
	m_timerDustTrail = 0.0f;
}



// ---------------------------
// ���������̎��ɏo��y���𐶐�����
// ---------------------------
void Particle::CreateBashDust(void* center)
{
	// ���S���W���擾
	DirectX::SimpleMath::Vector3 centerPos = *static_cast<DirectX::SimpleMath::Vector3*>(center);

	// 25����
	for (int i = 0; i < MAX_SMASH_ATTACK_DUST; i++)
	{
		// ���S�ȃ����_�����n�[�h�E�F�A�I�ɐ������邽�߂̃N���X�̕ϐ�
		std::random_device seed;
		// ��L�̊��S�ȃ����_���͓��삪�x�����߁Aseed�l�̌���݂̂Ɏg�p����
		// ���udefault_random_engine�v��using�Łumt19937�v�ƂȂ��Ă���
		std::default_random_engine engine(seed());
		// �Ƃ΂��ė~���������_���͈̔͂�Distribution�ɔC����B�����0�`2PI
		std::uniform_real_distribution<> dist(0, DirectX::XM_2PI);
		// �����_���Ȋp�x�𐶐�����
		float randAngle = static_cast<float>(dist(engine));

		// ���x�̃����_������
		std::uniform_real_distribution<> dist2(MIN_SMASH_DUST_SPEED_Y, MAX_SMASH_DUST_SPEED_Y);
		float Yspeed = static_cast<float>(dist2(engine) / 2.5f);
		float XZspeed = static_cast<float>(dist2(engine));

		// ���S����̃x�N�g���𐶐�
		DirectX::SimpleMath::Vector3 vectorFromCenter = centerPos + DirectX::SimpleMath::Vector3
		(
			SMASH_DUST_RADIUS * cosf(randAngle), 
			0.0f, 
			SMASH_DUST_RADIUS * sinf(randAngle)
		) - centerPos;

		// �x�N�g���̒������擾�i�����j
		float distanceFromCenter = vectorFromCenter.Length();
		// ���S����̃x�N�g���𐳋K�����ĕ�����ێ�
		DirectX::SimpleMath::Vector3 normalizedVectorFromCenter = vectorFromCenter / distanceFromCenter;
		// �x�N�g�����O���ɍL���邽�߂̃X�P�[����K�p
		float scaleFactor = 2.0f + (distanceFromCenter / SMASH_DUST_RADIUS);
		DirectX::SimpleMath::Vector3 adjustedVelocity = normalizedVectorFromCenter * scaleFactor;
		// ���x�x�N�g���𐶐�
		DirectX::SimpleMath::Vector3 velocity = -adjustedVelocity;

		// ���������_�X�g�̍��W���擾����
		DirectX::SimpleMath::Vector3 dustPosition = centerPos + DirectX::SimpleMath::Vector3
		(
			SMASH_DUST_RADIUS * cosf(randAngle), 
			0.0f, 
			SMASH_DUST_RADIUS * sinf(randAngle)
		);

		// �p�[�e�B�N������
		DustTrailParticle pB(
			0.9f,																						// ��������(s)
			dustPosition,																				// ����W
			DirectX::SimpleMath::Vector3{ -velocity.x * XZspeed, Yspeed , -velocity.z * XZspeed } *2,	// ���x
			DirectX::SimpleMath::Vector3(0.1f, 0.1f, 0.1f),												// �����x
			DirectX::SimpleMath::Vector3::One, DirectX::SimpleMath::Vector3{ 10.0f, 25.0f, 10.0f },				// �����X�P�[���A�ŏI�X�P�[��
			DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 1.f),												// �����J���[
			DirectX::SimpleMath::Color(1.f, 1.f, 1.f, -1.f)												// �ŏI�J���[
		);

		m_dustTrail.push_back(pB);
	}
}



// ---------------------------
// �����~�����Ƃ��̎c�����o��
// ---------------------------
void Particle::CreateSwordTrail(void* ver)
{
	// void* ��K�؂Ȍ^�ɃL���X�g
	DirectX::VertexPositionTexture* vertices = static_cast<DirectX::VertexPositionTexture*>(ver);

	// SwordTrailParticle�𐶐�
	SwordTrailParticle sTP(
		vertices,//	���_���
		0.2f,//	��������(s)
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),	// �����J���[
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.0f)	// �ŏI�J���[
	);

	// �z��ɒǉ�
	m_swordTrail.push_back(sTP);
}





// ---------------------------
// �V�F�[�_�[�𐶐�����
// ---------------------------
void Particle::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	m_swordShader = std::make_unique<CustomShader>
		(
			device, 
			SWORD_VS,
			SWORD_PS,
			nullptr,
			InputElements
		);

	m_dustShader = std::make_unique<CustomShader>
		(
			device,
			DUST_VS,
			DUST_PS,
			DUST_GS,
			InputElements
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


// -------------------------------------------------------------------
// Render���ʏ��������SwordTrailParticle��DustParticle�̕`��Ăяo��
// -------------------------------------------------------------------
void Particle::Render(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj
)
{
	using namespace DirectX;
	CommonResources* resources = CommonResources::GetInstance();
	auto states = resources->GetCommonStates();

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();											// �J�����̕����𐳋K��

	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };		//	�T���v���[�X�e�[�g�̐ݒ�
	context->PSSetSamplers(0, 1, sampler);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();	//  �������`��w��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		//	�������菈��
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);	//	�[�x�o�b�t�@�͂Ȃ�
	context->RSSetState(m_states->CullNone());						//	�J�����O�Ȃ�

	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	for (int i = 0; i < m_texture.size(); i++)	{ context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf()); }

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	DrawSwordParticle(view, proj);				// ���̎c���̕`��
	DrawDustParticle(view, proj, cameraDir);	// �y���̕`��
}



// -----------------------------------
// ���̎c���p�[�e�B�N���̕`��
// -----------------------------------
void Particle::DrawSwordParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	// ���̃p�[�e�B�N���̂��߂̃R���X�^���g�o�b�t�@�̍쐬�ƍX�V
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;		// ���̃p�[�e�B�N���ł̓r���{�[�h��K�p���Ȃ�
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	m_swordShader->BeginSharder(context);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();	//  �������`��w��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		//	�������菈��
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);		//	�[�x�o�b�t�@�͂Ȃ�
	context->RSSetState(m_states->CullNone());						//	�J�����O�Ȃ�

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// ���̎c���p�[�e�B�N����`��
	m_batch->Begin();

	// �F��ω������邽�߂̃C�[�W���O�ϐ�
	float t = 0.0f;
	for (auto& sTP : m_swordTrail)
	{
		// ���̃p�[�e�B�N����4�̒��_������
		DirectX::VertexPositionColorTexture ver[4];
		sTP.GetVertices(ver);

		float value1 = 1 - static_cast<float>(pow(1 - t, 2));	// �C�[�W���O1
		t += 1.0f / m_swordTrail.size();
		float value2 = 1 - static_cast<float>(pow(1 - t, 2));	// �C�[�W���O2	// t �� 1�ɋ߂Â��قǐF�������Ȃ�

		ver[1].color = DirectX::SimpleMath::Color(1, 1, 1, value1);		// �E��
		ver[2].color = DirectX::SimpleMath::Color(1, 0.8, 0.8, 0);		// �E��

		ver[0].color = DirectX::SimpleMath::Color(1, 1, 1, value2);		// ����
		ver[3].color = DirectX::SimpleMath::Color(1, 0.8, 0.8, 0);		// ����

		m_batch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	}

	m_batch->End();

	m_swordShader->EndSharder(context);
}



// ----------------------------------------
/// �y���p�[�e�B�N���̕`��
// ----------------------------------------
void Particle::DrawDustParticle(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Vector3 cameraDir
)
{
	// �y���p�[�e�B�N���̂��߂̃R���X�^���g�o�b�t�@�̍쐬�ƍX�V
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose(); // �y���p�[�e�B�N���ł̓r���{�[�h��K�p����
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	m_dustShader->BeginSharder(context);

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// �y���p�[�e�B�N���̒��_���X�g���N���A
	m_dustVertices.clear();
	for (DustTrailParticle& li : m_dustTrail)
	{
		// �J�����̕����Ɍ����Ȃ��p�[�e�B�N���̓X�L�b�v
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f)
			continue;

		// �p�[�e�B�N���̌��݂̏���ݒ�
		DirectX::VertexPositionColorTexture vPCT;
		vPCT.position = DirectX::XMFLOAT3(li.GetPosition());
		vPCT.color = DirectX::XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = DirectX::XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_dustVertices.push_back(vPCT);
	}

	// �p�[�e�B�N�������݂���ꍇ�A�`����s��
	if (!m_dustVertices.empty())
	{
		// �J�����Ƃ̋����Ɋ�Â��ăp�[�e�B�N�����\�[�g
		std::sort(m_dustVertices.begin(), m_dustVertices.end(),
			[&](DirectX::VertexPositionColorTexture lhs, DirectX::VertexPositionColorTexture rhs)
			{
				return cameraDir.Dot(lhs.position - m_cameraPosition) > cameraDir.Dot(rhs.position - m_cameraPosition);
			});

		// �p�[�e�B�N���̕`��
		m_batch->Begin();
		m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_dustVertices[0], m_dustVertices.size());
		m_batch->End();
	}

	m_dustShader->EndSharder(context);
}




// -------------------------------------
// �r���{�[�h�쐬�֐�
// -------------------------------------
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