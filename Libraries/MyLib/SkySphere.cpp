// -------------------------
//
// �@�V���Ɋւ���N���X
//
// --------------------------

#include "pch.h"

#include "Libraries/MyLib/SkySphere.h"
#include "Game/CommonResources.h"


const float SkySphere::SKYSPHERE_SCALE = 100.f;

// -------------------------------
//  �R���X�g���N�^
// -------------------------------
SkySphere::SkySphere()
	:
	m_skySphereModel()
{
}


// -------------------------------
//  �f�X�g���N�^
// -------------------------------
SkySphere::~SkySphere()
{
}


// -------------------------------
//  ���[�h����
// -------------------------------
void SkySphere::LoadSkySphereModel(ID3D11Device* device)
{
	auto commonResources = CommonResources::GetInstance();

	UNREFERENCED_PARAMETER(commonResources);

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// ���f����ǂݍ���
	m_skySphereModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/SkySphere.cmo", *fx);
}


// -------------------------------
//  �`�揈��
// -------------------------------
void SkySphere::DrawSkySphere(
	 ID3D11DeviceContext1* context
	, DirectX::DX11::CommonStates* states
	, DirectX::SimpleMath::Matrix view
	, DirectX::SimpleMath::Matrix projection
	)
{
	using namespace DirectX;

	context->OMSetDepthStencilState(states->DepthDefault(), 0);	// �[�x�l���Q�Ƃ��ď�������

	// ���f���̃G�t�F�N�g�����X�V���鏈��
	m_skySphereModel->UpdateEffects([](DirectX::IEffect* effect)
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
	// �T�C�Y�𒲐�����
	world *= SimpleMath::Matrix::CreateScale(SKYSPHERE_SCALE);


	// �V����`�悷��
	m_skySphereModel->Draw(context, *states, world, view, projection);
}