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
#include <cassert>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
TitleScene::TitleScene()
	:
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

	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/LOGO.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SPACE�ŃX�^�[�g.png",
			nullptr,
			m_texture2.ReleaseAndGetAddressOf()
		)
	);

	/*
		�ȉ��A�e�e�N�X�`���̑傫�������߂遨�e�N�X�`���̒��S���W���v�Z����
	*/
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

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
}


//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void TitleScene::Update(float elapsedTime)
{
	// �錾���������A���ۂ͎g�p���Ă��Ȃ��ϐ�
	UNREFERENCED_PARAMETER(elapsedTime);

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
	}
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void TitleScene::Render()
{
	auto states = m_commonResources->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// TRIDENT���S�̕`��ʒu�����߂�
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// �摜�̒��S���v�Z����
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };

	// LOGO.png �𒆉��ɕ`�悷��
	m_spriteBatch->Draw(
		m_texture.Get(),   // �e�N�X�`��(SRV)
		pos,               // �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,           // ��`(RECT)
		Colors::White,     // �w�i�F
		0.0f,              // ��]�p(���W�A��)
		m_texCenter1,      // �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,              // �X�P�[��(scale)
		SpriteEffects_None,// �G�t�F�N�g(effects)
		0.0f               // ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);

	// SPACE�ŃX�^�[�g.png �̕`��ʒu�𒲐�
	DirectX::SimpleMath::Vector2 pos2 = DirectX::SimpleMath::Vector2(pos.x, pos.y + 200.0f);

	// SPACE�ŃX�^�[�g.png �𒆉��ɕ`�悷��
	m_spriteBatch->Draw(
		m_texture2.Get(),   // �e�N�X�`��(SRV)
		pos2,               // �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,            // ��`(RECT)
		Colors::White,      // �w�i�F
		0.0f,               // ��]�p(���W�A��)
		m_texCenter2,       // �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,               // �X�P�[��(scale)
		SpriteEffects_None, // �G�t�F�N�g(effects)
		0.0f                // ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);

#ifdef _DEBUG
	// �����ɃX�v���C�g�t�H���g�ŕ������`�悷����@
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Title Scene", Vector2(10, 40));

	wchar_t buf[32];
	swprintf_s(buf, 32, L"right : %d, bottom : %d", rect.right, rect.bottom);
	m_spriteFont->DrawString(m_spriteBatch.get(), buf, Vector2(10, 70));
#endif // _DEBUG

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
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
		return IScene::SceneID::PLAY;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}
