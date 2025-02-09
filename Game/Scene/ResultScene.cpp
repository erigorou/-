// --------------------------------------------------
// 
// ���O:	ResultScene.h
// ���e:	���U���g�V�[����`�悷��N���X
// �쐬:	�r�c����
// 
// --------------------------------------------------
// �C���N���[�h
#include "pch.h"
#include "ResultScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/Texture.h"
#include "../Sound/Sound.h"
#include "State/WinResult.h"
#include "State/LoseResult.h"

// ---------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ---------------------------------------------
ResultScene::ResultScene()
	: m_spriteBatch{}
	, m_texture{}
	, m_texCenter{}
	, m_isChangeScene{}
	, m_winResult{}

{
}

// ---------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ---------------------------------------------
ResultScene::~ResultScene()
{
}

// ---------------------------------------------
/// <summary>
/// ����������
/// </summary>
// ---------------------------------------------
void ResultScene::Initialize()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// �e�N�X�`���̍쐬����уf�[�^�̎擾
	CreateTextures();
	// �I�u�W�F�N�g�̐���
	CreateObjects();
	// �V�[���ύX�t���O������������
	m_isChangeScene = false;
}

// ---------------------------------------------
/// <summary>
/// �e�N�X�`�����쐬���A�f�[�^���擾����
/// </summary>
// ---------------------------------------------
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

// ---------------------------------------------
/// <summary>
/// �I�u�W�F�N�g�𐶐�����
/// </summary>
// ---------------------------------------------
void ResultScene::CreateObjects()
{
	// �������U���g�̐���
	m_winResult = std::make_unique<WinResult>();
	m_loseResult = std::make_unique<LoseResult>();

	// ������
	m_winResult->Initialize();
	m_loseResult->Initialize();

	// �Q�[���f�[�^���擾���A����ɉ��������ʂ��o��
	switch (GameData::GetInstance()->GetBattleResult())
	{
		// ����
	case GameData::BATTLE_RESULT::WIN:
		m_currentState = m_winResult.get();
		Sound::ChangeBGM(Sound::BGM_TYPE::WIN);
		break;

		// �s�k
	case GameData::BATTLE_RESULT::LOSE:
		m_currentState = m_loseResult.get();
		Sound::ChangeBGM(Sound::BGM_TYPE::LOSE);
		break;

		// ����ȊO
	default:
		assert(false && "GameData��VATTLE_RESULT�ɐ��������ʂ������Ă܂���B");
		break;
	}
}

// ---------------------------------------------
/// <summary>
/// �V�[�����X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
// ---------------------------------------------
void ResultScene::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g���X�V����
	m_currentState->Update(elapsedTime);

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = CommonResources::GetInstance()->GetInputManager()->GetKeyboardTracker();

	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
	}
}

// ---------------------------------------------
/// <summary>
/// �`�悷��
/// </summary>
// ---------------------------------------------
void ResultScene::Render()
{
	// �R�����X�e�[�g���擾����
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());

	// TRIDENT���S�̕`��ʒu�����߂�
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };
	// �摜�̒��S���v�Z����
	DirectX::SimpleMath::Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };

	m_spriteBatch->Draw(
		m_texture.Get(),			// �e�N�X�`��(SRV)
		pos,						// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,					// ��`(RECT)
		DirectX::Colors::White,		// �w�i�F
		0.0f,						// ��]�p(���W�A��)
		m_texCenter,				// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,						// �X�P�[��(scale)
		DirectX::SpriteEffects_None,// �G�t�F�N�g(effects)
		0.0f						// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();

	// ���������̕`��
	m_currentState->Render();
}

// ---------------------------------------------
/// <summary>
/// ��n������
/// </summary>
// ---------------------------------------------
void ResultScene::Finalize()
{
}

// ---------------------------------------------
/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns>���̃V�[��ID</returns>
// ---------------------------------------------
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