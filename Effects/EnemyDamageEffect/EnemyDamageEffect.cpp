// ** ---------------------------------------- **
// <�Ӑ}>�@�G�̑̂̏�ԏ����N���X�̎���(��_��)
// <���>�@�r�c
// <���t>�@2024/12/09
// ** ---------------------------------------- **

#include "pch.h"
#include "Effects/EnemyDamageEffect/EnemyDamageEffect.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"


// �Œ�l
const wchar_t* EnemyDamageEffect::VS_PATH = L"Resources/Shaders/DamageEffect/DamageEffectVS.cso";
const wchar_t* EnemyDamageEffect::PS_PATH = L"Resources/Shaders/DamageEffect/DamageEffectPS.cso";


// ----------------------------
/// �R���X�g���N�^
// ----------------------------
EnemyDamageEffect::EnemyDamageEffect()
	:
	m_totalTime(0.0f),
	m_damageShader(nullptr),
	m_buffer(nullptr),
	m_isDamaged(false)
{
	// �V�F�[�_�[�̐���
	CreateShader();

	// �萔�o�b�t�@�̍쐬
	CreateConstBuffer();
}



// ----------------------------
/// �f�X�g���N�^
// ----------------------------
EnemyDamageEffect::~EnemyDamageEffect()
{
}


// ----------------------------
/// �X�V����
// ----------------------------
void EnemyDamageEffect::Update(float elapsedTime)
{
	// �_���[�W���󂯂ĂȂ��ꍇ�͏����Ȃ�
	if (!m_isDamaged)	return;

	// �o�ߎ��Ԃ̋L�^
	m_totalTime = std::max(0.0f, (m_totalTime - elapsedTime));

	// �_���[�W�G�t�F�N�g�̕\�����I�������邩�ǂ���
	m_isDamaged = (m_totalTime > 0.0f);
}


// ----------------------------
/// �̃p�[�c�̕`��
// ----------------------------
void EnemyDamageEffect::DrawWithDamageEffect(
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

	// ���[���h�A�r���[�A�v���W�F�N�V�����s��̐ݒ�
	cb->world = world.Transpose();
	cb->view = view.Transpose();
	cb->projection = proj.Transpose();

	// ���Ԃ̐ݒ�i�C�[�W���O�Ɛ��K�������Ă����Ԃ̂��́j
	cb->Time = Easing::easeInCirc(m_totalTime / TOTAL_TIME);
	cb->Padding[0] = 0;
	cb->Padding[1] = 0;
	cb->Padding[2] = 0;

	context->Unmap(m_buffer.Get(), 0);

	// ���f���̕`��
	model->Draw(context, *states, world, view, proj, false, [&]
		{
			if (m_isDamaged)
			{
				// �萔�o�b�t�@��ݒ�
				ID3D11Buffer* cbuff = { m_buffer.Get() };
				// �V�F�[�_�[�Ƀo�b�t�@��n��
				context->VSSetConstantBuffers(1, 1, &cbuff);
				context->PSSetConstantBuffers(1, 1, &cbuff);

				// �V�F�[�_�[�̐ݒ�
				m_damageShader->BeginSharder(context);
			}
		}
	);

	m_damageShader->EndSharder(context);
}


// ----------------------------
/// �V�F�[�_�[�̍쐬
// ----------------------------
void EnemyDamageEffect::CreateShader()
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// �V�F�[�_�[�̍쐬
	m_damageShader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			nullptr,
			INPUT_LAYOUT
		);
}


// ----------------------------
/// �萔�o�b�t�@�̍쐬
// ----------------------------
void EnemyDamageEffect::CreateConstBuffer()
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
/// �_���[�W���󂯂����Ƃ�ʒB����֐�
// --------------------------------------
void EnemyDamageEffect::IsDamaged()
{
	// �_���[�W���󂯂����Ƃ�ʒB
	m_totalTime = TOTAL_TIME;
	m_isDamaged = true;
}