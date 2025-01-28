#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "QuestSelectSceneUIManager.h"
#include "Game/UI/UserInterface.h"
#include "Game/UI/UIAnchor.h"

#include "Game/UI/Action/NormalAction.h"
#include "Game/UI/Action/TitleLogoAction.h"

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
		DirectX::SimpleMath::Vector2(360.0f, -300.0f),
		DirectX::SimpleMath::Vector2(1.0f, 1.0f),
		ANCHOR::TOP_LEFT,
		new TitleLogoAction()
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
