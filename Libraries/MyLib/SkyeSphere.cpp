// -------------------------
//
// �@�V���Ɋւ���N���X
//
// --------------------------

#include "pch.h"

#include "Libraries/MyLib/SkyeSphere.h"
#include "Game/CommonResources.h"


// -------------------------------
//  �R���X�g���N�^
// -------------------------------
SkyeSphere::SkyeSphere()
	:
	m_skyeSphereModel()
{
}


// -------------------------------
//  �f�X�g���N�^
// -------------------------------
SkyeSphere::~SkyeSphere()
{
}


// -------------------------------
//  ���[�h����
// -------------------------------
void SkyeSphere::LoadSkyeSphereModel(ID3D11Device* device)
{
	auto commonResources = CommonResources::GetInstance();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// ���f����ǂݍ���
	m_skyeSphereModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/SkyeSphere.cmo", *fx);
}


// -------------------------------
//  �`�揈��
// -------------------------------
void SkyeSphere::DrawSkyeSphere(
	 ID3D11DeviceContext1* context
	, DirectX::DX11::CommonStates* states
	, DirectX::SimpleMath::Matrix view
	, DirectX::SimpleMath::Matrix projection
	)
{
	using namespace DirectX;

	// ���f���̃G�t�F�N�g�����X�V���鏈��
	m_skyeSphereModel->UpdateEffects([](DirectX::IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// �ʂ̃��C�g�����ׂĖ���������
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, false);

				// ���f����������������
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// ������Ԃ̃��[���h�}�g���b�N�X��ݒ�
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	// �V����`�悷��
	m_skyeSphereModel->Draw(context, *states, world, view, projection);
}