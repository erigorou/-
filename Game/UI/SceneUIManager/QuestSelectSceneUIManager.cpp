#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "QuestSelectSceneUIManager.h"
#include "Game/UI/UserInterface.h"
#include "Game/UI/UIAnchor.h"

#include "Game/UI/Action/NormalAction.h"
#include "Game/UI/Action/TitleLogoAction.h"
#include "Game/UI/Action/TutorialButtonAction.h"
#include "Game/UI/Action/BossFightButtonAction.h"

// --------------------------------------------------------
// �R���X�g���N�^
// --------------------------------------------------------
QuestSelectSceneUIManager::QuestSelectSceneUIManager()
{
}


// --------------------------------------------------------
// �f�X�g���N�^
// --------------------------------------------------------
QuestSelectSceneUIManager::~QuestSelectSceneUIManager()
{
}


// --------------------------------------------------------
// ������
// --------------------------------------------------------
void QuestSelectSceneUIManager::Initialize()
{
	// ���S�̒ǉ�
	AddUserInterface(
		"logo",
		DirectX::SimpleMath::Vector2(640.0f, -300.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::TOP_CENTER,
		new TitleLogoAction()
	);

	// �{�^���̒ǉ�
	AddUserInterface(
		"startButton",
		DirectX::SimpleMath::Vector2(640.0f, 700.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_CENTER,
		new NormalAction()
	);

	// �`���[�g���A���{�^���̒ǉ�
	AddUserInterface(
		"tutorial",
		DirectX::SimpleMath::Vector2(1580.0f, 500.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_RIGHT,
		new TutorialButtonAction()
	);

	// �{�X��{�^���̒ǉ�
	AddUserInterface(
		"bossfight",
		DirectX::SimpleMath::Vector2(1880.0f, 625.0f),
		DirectX::SimpleMath::Vector2(DEFAULT_SIZE),
		ANCHOR::BOTTOM_RIGHT,
		new BossFightButtonAction()
	);
}


// --------------------------------------------------------
// ���[�U�[�C���^�[�t�F�[�X�̒ǉ�
// --------------------------------------------------------
void QuestSelectSceneUIManager::AddUserInterface(
	const std::string textureName,
	DirectX::SimpleMath::Vector2 position,
	DirectX::SimpleMath::Vector2 scale,
	ANCHOR anchor,
	IAction* action
)
{
	auto texture = GameResources::GetInstance()->GetTexture(textureName);
	auto ui = std::make_unique<UserInterface>();

	// ���[�U�[�C���^�[�t�F�[�X�̍쐬
	ui->Create(texture, position, scale, anchor, action);

	// ���[�U�[�C���^�[�t�F�[�X�̒ǉ�
	m_userInterfaceList.push_back(std::move(ui));
}


// --------------------------------------------------------
// �X�V
// --------------------------------------------------------
void QuestSelectSceneUIManager::Update(const float time)
{
	for (auto& ui : m_userInterfaceList)
	{
		ui->Update(time);
	}
}

// --------------------------------------------------------
// �`��
// --------------------------------------------------------
void QuestSelectSceneUIManager::Render()
{
	for (auto& ui : m_userInterfaceList)
	{
		ui->Render();
	}
}
