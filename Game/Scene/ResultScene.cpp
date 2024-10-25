/*
	@file	ResultScene.cpp
	@brief	���U���g�V�[���N���X
*/
#include "pch.h"
#include "ResultScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "../Data/GameData.h"
#include "Libraries/MyLib/Texture.h"
#include "State/WinResult.h"
#include "State/LoseResult.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
ResultScene::ResultScene()
	: m_spriteBatch{}
	, m_texture{}
	, m_texCenter{}
	, m_isChangeScene{}
	, m_commonResources{}
	, m_winResult{}

{
	m_commonResources = CommonResources::GetInstance();
}


//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
ResultScene::~ResultScene()
{
	// do nothing.
}


//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void ResultScene::Initialize()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// �e�N�X�`���̍쐬����уf�[�^�̎擾
	CreateTextures();
	// �I�u�W�F�N�g�̐���
	CreateObjects();
	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
}



//---------------------------------------------------------
// �e�N�X�`���֘A
//---------------------------------------------------------
void ResultScene::CreateTextures()
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// �e�N�X�`����ǂݍ���
	mylib::Texture::LoadTexture(
		device,
		m_texture,
		L"Resources/Textures/SPACE�Ń^�C�g��.png"
	);

	DirectX::SimpleMath::Vector2 texSize{};


	// �e�N�X�`���̃T�C�Y�ƒ��S�_���v�Z����
	mylib::Texture::CalculateTextureCenter
	(
		m_texture,
		texSize,
		m_texCenter
	);
}


//---------------------------------------------------------
// �I�u�W�F�N�g�̐���
//---------------------------------------------------------
void ResultScene::CreateObjects()
{
	// �������U���g�̐���
	m_winResult		= std::make_unique<WinResult>	();
	m_loseResult	= std::make_unique<LoseResult>	();

	// ������
	m_winResult	->	Initialize();
	m_loseResult->	Initialize();

	//// �Q�[���f�[�^�̎擾
	//m_gameData = GameData::GetInstance();

	// �Q�[���f�[�^���擾���A����ɉ��������ʂ��o��
	switch (GameData::GetInstance()->GetBattleResult())
	{
	case GameData::BATTLE_RESULT::WIN:
		m_currentState = m_winResult.get();
		
		break;
	case GameData::BATTLE_RESULT::LOSE:
		m_currentState = m_loseResult.get();
		break;

	default:
		assert(false && "GameData��VATTLE_RESULT�ɐ��������ʂ������Ă܂���BPlayScene��Update�̂Ƃ���̔���̂Ƃ������ƌ��Ă�B");
		break;
	}
}


//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void ResultScene::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g���X�V����
	m_currentState->Update(elapsedTime);

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
void ResultScene::Render()
{
	auto states = m_commonResources->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// TRIDENT���S�̕`��ʒu�����߂�
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// �摜�̒��S���v�Z����
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };


	m_spriteBatch->Draw(
		m_texture.Get(),	// �e�N�X�`��(SRV)
		pos,				// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,			// ��`(RECT)
		Colors::White,		// �w�i�F
		0.0f,				// ��]�p(���W�A��)
		m_texCenter,		// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,				// �X�P�[��(scale)
		SpriteEffects_None,	// �G�t�F�N�g(effects)
		0.0f				// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();


	// ���������̕`��
	m_currentState->Render();

#ifdef _DEBUG
#endif // _DEBUG

}


//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void ResultScene::Finalize()
{
	// do nothing.
}


//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID ResultScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}