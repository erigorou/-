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
#include "../Sound/Sound.h"
#include "Game/UI/UIAnchor.h"
#include "Interface/IAction.h"
#include "Game/Messenger/KeyboardMessenger.h"
#include "Libraries/MyLib/Math.h"
#include "Game/UI/Action/NormalAction.h"
#include "Game/Factory/Factory.h"
#include "Game/Camera/Camera.h"
// �X�e�[�W�֘A
#include "Game/Stage/Sea/Sea.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Stage/Floor/Floor.h"
#include "Game/Stage/SkySphere/SkySphere.h"

// ----------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
//-----------------------------------------------
SelectScene::SelectScene()
	: 
	m_spriteBatch(),
	m_isChangeScene(),
	m_selectIndex()
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
	// �L�[�{�[�h�̓o�^
	RegisterKeyboard();
	// SelectScene��UI�ǉ�
	CreateUI();
	// �X�e�[�W�̍쐬
	CreateStage();
}

// ----------------------------------------------
/// <summary>
/// �`�揈��
/// </summary>
// -----------------------------------------------
void SelectScene::Update(float elapsedTime)
{
	// �J�����̍X�V
	m_camera->Update(CAMERA_POSITION, CAMERA_DIRECTION, elapsedTime);

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
	// �X�e�[�W�̕`��
	RenderStage();

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

// -------------------------------------------------
/// <summary>
/// SelectScene����UI��o�^����
/// </summary>
// -------------------------------------------------
void SelectScene::CreateUI()
{
	//// �w�i�̒ǉ�
	//AddUserInterface(
	//	"SelectBack",
	//	BACKGROUND_POSITION,
	//	BACKGROUND_SIZE,
	//	ANCHOR::TOP_LEFT,
	//	new NormalAction()
	//);

	// EspaceUI�̒ǉ�
	AddUserInterface(
		"SelectEscape",
		ESPACE_POSITION,
		ESPACE_SIZE,
		ANCHOR::MIDDLE_CENTER,
		new NormalAction()
	);

	// SpaceUI�̒ǉ�
	AddUserInterface(
		"SelectSpace",
		SPACE_POSITION,
		SPACE_SIZE,
		ANCHOR::MIDDLE_CENTER,
		new NormalAction()
	);

	// �X�e�[�W�I��UI�i�匩�o���j�̒ǉ�
	AddUserInterface(
		"StageSelect",
		LOGO_POSITION,
		LOGO_SIZE,
		ANCHOR::TOP_LEFT,
		new NormalAction()
	);

	// �`���[�g���A��UI�̒ǉ�
	AddUserInterface(
		"SelectTutorial",
		TUTORIAL_POSITION,
		SELECT_STAGE_UI_SIZE,
		ANCHOR::MIDDLE_LEFT,
		new NormalAction()
	);

	// �{�X��UI�̒ǉ�
	AddUserInterface(
		"SelectBoss",
		BOSS_POSITION,
		SELECT_STAGE_UI_SIZE,
		ANCHOR::MIDDLE_LEFT,
		new NormalAction()
	);
}

// -----------------------------------------------
/// <summary>
/// �X�e�[�W���쐬����
/// </summary>
// -----------------------------------------------
void SelectScene::CreateStage()
{
	// �Փ˔���̍쐬
	m_collisionManager = Factory::CreateCollisionManager();
	// �J�����̍쐬
	m_camera = Factory::CreateCamera();

	// �J�������쐬����
	RECT rect{ CommonResources::GetInstance()->GetDeviceResources()->GetOutputSize() };
	// �ˉe�s����쐬����
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(FOV),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		NEAR_Z, FAR_Z);

	// �J�����̃X�e�[�g�ύX
	CameraState playState = CameraState::Select;
	m_camera->ChangeState(&playState);

	// �C�̍쐬
	m_sea = Factory::CreateSea();
	// �ǂ̍쐬
	m_wall = Factory::CreateWall();
	// �n�ʂ̍쐬
	m_floor = Factory::CreateFloor();
	// �X�J�C�{�b�N�X�̍쐬
	m_skySphere = Factory::CreateSkySphere();
}

// -----------------------------------------------
/// <summary>
/// �X�e�[�W�̕`��
/// </summary>
// ------------------------------------------------
void SelectScene::RenderStage()
{
	// �J�����̕K�v�����擾
	auto view = m_camera->GetViewMatrix();

	// �C�̕`��
//	m_sea->Render(view, m_projection);
	// �ǂ̕`��
//	m_wall->Render(view, m_projection);
	// �n�ʂ̕`��
//	m_floor->Render(view, m_projection);
	// �X�J�C�{�b�N�X�̕`��
//	m_skySphere->DrawSkySphere(view, m_projection);
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
	// �V�[���ύX���͏������Ȃ�
	if (m_isChangeScene) return;

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
	// �I���C���f�b�N�X�����炷
	m_selectIndex = Math::Clamp(m_selectIndex - 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);

	// �Q�[���f�[�^�ɃX�e�[�W��ݒ�
	auto gameData = GameData::GetInstance();
	gameData->SetSelectStage(m_selectIndex);
}

// ----------------------------------------------
/// <summary>
// ���L�[�������ꂽ�Ƃ��̏���
/// </summary>
// -----------------------------------------------
void SelectScene::OnKeyDownArrowDown()
{
	// �I���C���f�b�N�X�𑝂₷
	m_selectIndex = Math::Clamp(m_selectIndex + 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);

	// �Q�[���f�[�^�ɃX�e�[�W��ݒ�
	auto gameData = GameData::GetInstance();
	gameData->SetSelectStage(m_selectIndex);
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

// ---------------------------------------------------------
/// <summary>
/// ���[�U�[�C���^�[�t�F�[�X�̒ǉ�
/// </summary>
/// <param name="textureName">�e�N�X�`����</param>
/// <param name="position">�ʒu</param>
/// <param name="scale">�X�P�[��</param>
/// <param name="anchor">�A���J�[</param>
/// <param name="action">�A�N�V����</param>
// ---------------------------------------------------------
void SelectScene::AddUserInterface(
	const std::string& textureName,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	const ANCHOR& anchor,
	IAction* action
)
{
	// �e�N�X�`���̎擾
	auto texture = GameResources::GetInstance()->GetTexture(textureName);
	// ���[�U�[�C���^�[�t�F�[�X�̐���
	auto ui = std::make_unique<UserInterface>();
	// ���[�U�[�C���^�[�t�F�[�X�̍쐬
	ui->Create(texture, position, scale, anchor, action);
	// ���[�U�[�C���^�[�t�F�[�X�̓o�^
	m_uiList.push_back(std::move(ui));
}
