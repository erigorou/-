// ** ---------------------------------------- **
// <�Ӑ}>�@�G�̑̂̃G�t�F�N�g����(���S)
// <���>�@�r�c
// <���t>�@2024/1/20
// ** ---------------------------------------- **

#include "pch.h"
#include "Effects/EnemyDeadEffect/EnemyDeadEffect.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"


// ----------------------------
/// �R���X�g���N�^
// ----------------------------
EnemyDeadEffect::EnemyDeadEffect()
	:
	m_totalTime{},
	m_deadShader{},
	m_buffer{},
	m_isDead(false)
{
	// �V�F�[�_�[�̐���
	CreateShader();

	// �萔�o�b�t�@�̍쐬
	CreateConstBuffer();
}



// ----------------------------
/// �f�X�g���N�^
// ----------------------------
EnemyDeadEffect::~EnemyDeadEffect()
{
}


// ----------------------------
/// �X�V����
// ----------------------------
void EnemyDeadEffect::Update(float elapsedTime)
{
	// �_���[�W���󂯂ĂȂ��ꍇ�͏����Ȃ�
	if (!m_isDead)	return;

	// �o�ߎ��Ԃ̋L�^
	m_totalTime = std::max(0.0f, (m_totalTime - elapsedTime));
}


// ----------------------------
/// �̃p�[�c�̕`��
// ----------------------------
void EnemyDeadEffect::DrawWithDeadEffect(
	DirectX::Model* model, 
	const DirectX::SimpleMath::Matrix world, 
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj)
{
	// �K�v���̎擾
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// �萔�o�b�t�@�̍X�V
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);

	// ���Ԃ̐ݒ�i�C�[�W���O�Ɛ��K�������Ă����Ԃ̂��́j
	cb->Time = Easing::easeInCirc(m_totalTime / TOTAL_TIME);
	cb->Padding = DirectX::SimpleMath::Vector3::Zero;

	context->Unmap(m_buffer.Get(), 0);

	// �G�t�F�N�g�̐ݒ�
	model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetLightingEnabled		(true);		// ���C�g�L����
				basicEffect->SetPerPixelLighting	(true);		// �s�N�Z���P�ʂ̃��C�e�B���O�L����
				basicEffect->SetTextureEnabled		(false);	// �e�N�X�`���̖�����
				basicEffect->SetVertexColorEnabled	(false);	// ���_�J���[�̖�����
				basicEffect->SetFogEnabled			(false);	// �t�H�O�̖�����
			}
		}
	);


	ID3D11SamplerState* sampler[1] = { };
	context->PSSetSamplers(0, 1, sampler);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = GameResources::GetInstance()->GetTexture("noize");

	// ���f���̕`��
	model->Draw(context, *states, world, view, proj, false, [&]
		{
			// �萔�o�b�t�@��ݒ�
			ID3D11Buffer* cbuff = { m_buffer.Get() };
			// �V�F�[�_�[�Ƀo�b�t�@��n��
			context->PSSetConstantBuffers(1, 1, &cbuff);
			//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
			context->PSSetShaderResources(0, 1, texture.GetAddressOf());

			// �u�����h�X�e�[�g��ݒ�
			context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);

			// �V�F�[�_�[�̐ݒ�
			m_deadShader->BeginSharder(context);
		}
	);
}


// ----------------------------
/// �V�F�[�_�[�̍쐬
// ----------------------------
void EnemyDeadEffect::CreateShader()
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// �V�F�[�_�[�̍쐬
	m_deadShader = std::make_unique<CustomShader>
		(
			device,			// �f�o�C�X
			VS_PATH,		// ���_�V�F�[�_�[
			PS_PATH,		// �s�N�Z���V�F�[�_�[
			nullptr,		// �W�I���g���V�F�[�_�[(�g�p����)
			INPUT_LAYOUT	// ���̓��C�A�E�g
		);
}


// ----------------------------
/// �萔�o�b�t�@�̍쐬
// ----------------------------
void EnemyDeadEffect::CreateConstBuffer()
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(ConstBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DX::ThrowIfFailed(
		device->CreateBuffer(&desc, nullptr, m_buffer.GetAddressOf())
	);
}


// --------------------------------------
/// ���S�������Ƃ�ʒB����֐�
// --------------------------------------
void EnemyDeadEffect::IsDead()
{
	if (m_isDead) return;

	// �c�莞�Ԃ����Z�b�g
	m_totalTime = TOTAL_TIME;
	// ���S�t���O��L����
	m_isDead = true;
}