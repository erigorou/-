/*
	@file	LoseResultScene.cpp
	@brief	���U���g�V�[���N���X
*/
#include "pch.h"
#include "LoseResult.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/Texture.h"
#include <cassert>

using namespace DirectX;
using namespace DirectX::SimpleMath;

const wchar_t* LoseResult::TEXTURE_PATH = L"Resources/Textures/UI/Lose.png";

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
LoseResult::LoseResult()
	:
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
LoseResult::~LoseResult()
{
	// do nothing.
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void LoseResult::Initialize()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �e�N�X�`���̍쐬����уf�[�^�̎擾
	mylib::Texture::LoadTexture
	(
		device,
		m_texture,
		TEXTURE_PATH
	);

	// �e�N�X�`���̑傫��
	DirectX::SimpleMath::Vector2 texSize;

	// �e�N�X�`���̒��S���W���v�Z����
	mylib::Texture::CalculateTextureCenter
	(
		m_texture,
		texSize,
		m_texCenter
	);

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void LoseResult::Update(float elapsedTime)
{
	// �錾���������A���ۂ͎g�p���Ă��Ȃ��ϐ�
	UNREFERENCED_PARAMETER(elapsedTime);
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void LoseResult::Render()
{
	auto states = m_commonResources->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	DirectX::SimpleMath::Vector2 texPos{ 0.0f, 200.0f };

	// �e�N�X�`����`�悷��
	mylib::Texture::DrawTexture
	(
		m_spriteBatch.get(),
		texPos,
		m_texture,
		m_texCenter,
		Vector2{ 1.0f, 1.0f },
		0.0f
	);

#ifdef _DEBUG
#endif // _DEBUG

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void LoseResult::Finalize()
{
	// do nothing.
}

//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID LoseResult::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}