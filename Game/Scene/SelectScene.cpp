// --------------------------------------------------
// 
// ���O:	SelectScene.cpp
// ���e:	�Z���N�g�V�[����`�悷��N���X
// �쐬:	�r�c����
// 
// --------------------------------------------------
// �C���N���[�h
#include "pch.h"
#include "SelectScene.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Game/UI/UserInterface.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Messenger/KeyboardMessenger.h"

// ----------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
//-----------------------------------------------
SelectScene::SelectScene()
	: 
	m_spriteBatch{},
	m_isChangeScene{},
	m_gameData{}
{
}

// ----------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ----------------------------------------------
SelectScene::~SelectScene()
{
	Finalize();
}

// ----------------------------------------------
/// <summary>
/// ����������
/// </summary>
// ----------------------------------------------
void SelectScene::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// �Q�[���f�[�^���擾����
	m_gameData = GameData::GetInstance();
	// �L�[�{�[�h�̓o�^
	RegisterKeyboard();
}

// ----------------------------------------------
/// <summary>
/// �`�揈��
/// </summary>
// -----------------------------------------------
void SelectScene::Update(float elapsedTime)
{
	// UI�̍X�V
	for (auto& ui : m_uiList)
	{
		ui->Update(elapsedTime);
	}
}

// ----------------------------------------------
/// <summary>
/// �`�揈��
/// </summary>
// ----------------------------------------------
void SelectScene::Render()
{
	// �R�����X�e�[�g���擾����
	auto states = CommonResources::GetInstance()->GetCommonStates();
	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();
	// UI�̕`��
	for (auto& ui : m_uiList)
	{
		ui->Render();
	}
}

// ----------------------------------------------
/// <summary>
/// �I������
/// </summary>
// ----------------------------------------------
void SelectScene::Finalize()
{
	// UI�̍폜
	m_uiList.clear();
	// �X�v���C�g�o�b�`�̍폜
	m_spriteBatch.reset();
	// �L�[�{�[�h�̓o�^����
	KeyboardMessenger::Clear();
}

// -----------------------------------------------
/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns>���̃V�[��ID</returns>
// -----------------------------------------------
IScene::SceneID SelectScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}

// -----------------------------------------------
/// <summary>
/// Escape�������ꂽ�ꍇ�̃V�[��ID���擾����
/// </summary>
/// <returns>�ύX��̃V�[��ID</returns>
// ---------------------------------
IScene::SceneID SelectScene::GetPrevSceneID() const
{
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::TITLE;
}


// ----------------------------------------------
/// <summary>
/// �g�p����L�[�{�[�h��o�^����
/// </summary>
// -----------------------------------------------
void SelectScene::RegisterKeyboard()
{
	// �L�[���͂̓o�^
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Up, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Down, this, KeyboardMessenger::KeyPressType::PRESSED);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Space, this, KeyboardMessenger::KeyPressType::PRESSED);
}


// ----------------------------------------------
/// <summary>
/// �L�[���́i�����ꂽ�u�ԁj
/// </summary>
/// <param name="key">�L�[</param>
// -----------------------------------------------
void SelectScene::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// �X�y�[�X�L�[�������ꂽ��
	if (key == DirectX::Keyboard::Keys::Space){
		OnKeyDownSpace();
	}
	// ��L�[�������ꂽ��
	if (key == DirectX::Keyboard::Keys::Up){
		OnKeyDownArrowUp();
	}
	// ���L�[�������ꂽ��
	if (key == DirectX::Keyboard::Keys::Down){
		OnKeyDownArrowDown();
	}
}

// ----------------------------------------------
/// <summary>
/// �L�[���́i������Ă���ԁj
/// </summary>
/// <param name="key">�L�[</param>
// -----------------------------------------------
void SelectScene::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	UNREFERENCED_PARAMETER(key);
}

// ----------------------------------------------
/// <summary>
// ��L�[�������ꂽ�Ƃ��̏���
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownArrowUp()
{
	// ��L�[�������ꂽ�Ƃ��̏���
}

// ----------------------------------------------
/// <summary>
// ���L�[�������ꂽ�Ƃ��̏���
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownArrowDown()
{
	// ���L�[�������ꂽ�Ƃ��̏���
}

// ----------------------------------------------
/// <summary>
// �X�y�[�X�L�[�������ꂽ�Ƃ��̏���
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownSpace()
{
	// �X�y�[�X�L�[�������ꂽ�Ƃ��̏���
	m_isChangeScene = true;
}

