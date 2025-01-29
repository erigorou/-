/*
	@file	TitleScene.cpp
	@brief	�^�C�g���V�[���N���X
*/
#include "pch.h"
#include "TitleScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Game/Messenger/EventMessenger.h"
#include "Libraries/MyLib/DebugString.h"
#include "../Factory/Factory.h"
#include "../Sound/Sound.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"
#include "Effects/Particle.h"

#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include "../Stage/Sea/Sea.h"
#include "../TitleObject/TitleEnemy.h"
#include "Libraries/MyLib/SkySphere.h"

#include <cassert>


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
TitleScene::TitleScene()
	:
	m_totalSeconds{},
	m_spriteBatch{},
	m_spriteFont{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
TitleScene::~TitleScene()
{
	// do nothing.
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void TitleScene::Initialize()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �X�v���C�g�t�H���g���쐬����
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(
		device,
		L"Resources/Fonts/SegoeUI_18.spritefont"
	);

	// �e�N�X�`�������[�h����
	LoadTextures();

	// �e�N�X�`���̒��S���W���v�Z����
	CalculateTextureCenter();

	// �I�u�W�F�N�g�𐶐�����
	CreateObjects();

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	// ���y�̕ύX
	Sound::ChangeBGM(Sound::BGM_TYPE::TITLE);
}


//---------------------------------------------------------
// ���[�h����
//---------------------------------------------------------
void TitleScene::LoadTextures()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// �摜�����[�h����
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/LOGO.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SPACE�ŃX�^�[�g.png",
			nullptr,
			m_texture2.ReleaseAndGetAddressOf()
		)
	);
}


//---------------------------------------------------------
// �e�N�X�`���̒��S���擾����
//---------------------------------------------------------
void TitleScene::CalculateTextureCenter()
{
	// �ꎞ�I�ȕϐ��̐錾
	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D{};
	D3D11_TEXTURE2D_DESC desc{};
	Vector2 texSize{};

	// LOGO.png �̒��S�ʒu���v�Z
	m_texture->GetResource(resource.GetAddressOf());
	resource.As(&tex2D);
	tex2D->GetDesc(&desc);
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	m_texCenter1 = texSize / 2.0f;

	// SPACE�ŃX�^�[�g.png �̒��S�ʒu���v�Z
	resource = nullptr;
	tex2D = nullptr;
	desc = {};

	m_texture2->GetResource(resource.GetAddressOf());
	resource.As(&tex2D);
	tex2D->GetDesc(&desc);
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	m_texCenter2 = texSize / 2.0f;

	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// �ˉe�s����쐬����
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);
}


//---------------------------------------------------------
// �I�u�W�F�N�g�̐���
//---------------------------------------------------------
void TitleScene::CreateObjects()
{
	m_camera	= Factory::CreateCamera	();
	m_floor		= Factory::CreateFloor	();
	m_sea = Factory::CreateSea();
	m_skySphere = Factory::CreateSkySphere();

	m_enemy = std::make_unique<TitleEnemy>();
	m_enemy->Initialize();
	m_skySphere->LoadSkySphereModel();

	// �^�C�g���V�[���̃J�����X�e�[�g��ݒ�
	m_camera->ChangeState(m_camera->GetTitleState());

	m_particle = Factory::CreateParticle();
}


//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void TitleScene::Update(float elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	DirectX::SimpleMath::Vector3 zeroV = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix zeroM = DirectX::SimpleMath::Matrix::Identity;

	m_camera->Shake(elapsedTime);
	m_camera->Update(zeroV, zeroV, zeroM, elapsedTime);

	m_enemy->Update(elapsedTime);

	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
		if (!m_isChangeScene)
		{
			m_isChangeScene = true;
			Sound::PlaySE(Sound::SE_TYPE::SYSTEM_OK);
		}
	}

	m_particle->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void TitleScene::Render()
{
	auto states		= m_commonResources	->	GetCommonStates		();
	auto view		= m_camera			->	GetViewMatrix		();

	// �I�u�W�F�N�g�̕`��
	m_floor->		Render(view, m_projection);
	m_sea->			Render(view, m_projection);
	m_enemy->		Render(view, m_projection);
	m_skySphere->	DrawSkySphere(view, m_projection);

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());

	// �e�N�X�`���̕`��
	DrawTexture();

	// �p�[�e�B�N���̕`��
	m_particle->CreateBillboard(m_camera->GetEyePosition(),DirectX::SimpleMath::Vector3::Zero,m_camera->GetUpVector());
	m_particle->Render(view, m_projection);

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();

#ifdef _DEBUG
#endif // _DEBUG
}


//---------------------------------------------------------
// �e�N�X�`���̕`��
//---------------------------------------------------------
void TitleScene::DrawTexture()
{
	// TRIDENT���S�̕`��ʒu�����߂�
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// �摜�̒��S���v�Z����
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	Vector2 titlePos = pos;
	titlePos.y += TITLE_LOGO_CENTER_Y;

	// �^�C�g�����S�̏㉺�ړ��̑���
	float moveValue = titlePos.y * 2;

	// �b���𐳋K��
	float t = Math::Clamp(m_totalSeconds - DELAY, 0.0f, ANIM_END) / ANIM_END;

	// �^�C�g�����S�̕`��ʒu�����߂�i�ړ����l���j
	Vector2 logoPos
	{
		titlePos.x ,
		TITLE_LOGO_CENTER_Y + moveValue * Easing::easeOutElastic(t)
	};


	// LOGO.png �𒆉��ɕ`�悷��
	m_spriteBatch->Draw(
		m_texture.Get(),	// �e�N�X�`��(SRV)
		logoPos,			// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,			// ��`(RECT)
		DirectX::Colors::White,		// �w�i�F
		0.0f,				// ��]�p(���W�A��)
		m_texCenter1,		// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,				// �X�P�[��(scale)
		DirectX::SpriteEffects_None,	// �G�t�F�N�g(effects)
		0.0f				// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);

	// SPACE�ŃX�^�[�g.png �̕`��ʒu�𒲐�
	DirectX::SimpleMath::Vector2 pos2 = DirectX::SimpleMath::Vector2(pos.x, pos.y + 300.0f);

	// SPACE�ŃX�^�[�g.png �𒆉��ɕ`�悷��
	m_spriteBatch->Draw(
		m_texture2.Get(),   // �e�N�X�`��(SRV)
		pos2,               // �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,            // ��`(RECT)
		DirectX::Colors::White,      // �w�i�F
		0.0f,               // ��]�p(���W�A��)
		m_texCenter2,       // �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,               // �X�P�[��(scale)
		DirectX::SpriteEffects_None, // �G�t�F�N�g(effects)
		0.0f                // ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);
}



//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void TitleScene::Finalize()
{
	// do nothing.
}

//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::QUEST;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}
