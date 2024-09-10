// -----------------------------------------------
//
// �v���C����UI�̕`��␶�����s���֐�
//
// -----------------------------------------------

#include "pch.h"
#include "Game/UI/PlaySceneUIManager/Header/PlaySceneUIManager.h"

#include "Interface/IUserInterface.h"



// ----------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ----------------------------
PlaySceneUIManager::PlaySceneUIManager()
{
	// �z��������Z�b�g
	m_uiList.clear();
}



// ----------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ----------------------------
PlaySceneUIManager::~PlaySceneUIManager()
{
}



// ----------------------------
/// <summary>
/// �������֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Initialize()
{
}


// ----------------------------
/// <summary>
/// �X�V�֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Update()
{
	// ���̑���UI�̑S�Ă̍X�V�������s��
	for (const auto& pair : m_uiList)
	{
		pair.second->Update();
	}
}



// ----------------------------
/// <summary>
/// �`��֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Render()
{
	// ���̑���UI�̑S�Ă̕`�揈�����s��
	for (const auto& pair : m_uiList)
	{
		pair.second->Render();
	}
}



// ----------------------------
/// <summary>
/// �I���֐�
/// </summary>
// ----------------------------
void PlaySceneUIManager::Finalize()
{
	// ���̑���UI�̑S�Ă̏I���������s��
	for (const auto& pair : m_uiList)
	{
		pair.second->Finalize();
	}
}



// ----------------------------------------------------------
/// <summary>
/// �ǉ��֐�
/// </summary>
/// <param name="ui">�ǉ�������UI�@IUserInterface</param>
// ----------------------------------------------------------
void PlaySceneUIManager::Add(IUserInterface* ui, std::string key)
{
	// ui�̃��X�g�ɒǉ�������
	m_uiList[key] = ui;
}



// ----------------------------
/// <summary>
/// �폜�֐�
/// </summary>
/// <param name="key">�L�[</param>
// ----------------------------
void PlaySceneUIManager::Remove(std::string key)
{
	m_uiList[key]->	Finalize();	// �I������
	m_uiList.		erase(key);	// �폜
}
