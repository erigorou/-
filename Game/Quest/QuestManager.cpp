// ---------------------------------------------
// �N�G�X�g�}�l�[�W���[�N���X�̎���
// ---------------------------------------------

#include "pch.h"
#include "QuestManager.h"
#include "../Scene/PlayScene.h"

#include "Interface/IQuestChecker.h"
#include "QuestList/QuestPlayerMove.h"
#include "QuestList/QuestPlayerAttack.h"
#include "QuestList/QuestPlayerCombo.h"
#include "QuestList/QuestPlayerAvoid.h"


// -----------------------------
// �R���X�g���N�^
// -----------------------------
QuestManager::QuestManager(PlayScene* playScene)
	: m_playScene(playScene)
	, m_currentQuestNo(0)	// ��
{
}


// -----------------------------
// �f�X�g���N�^
// -----------------------------
QuestManager::~QuestManager()
{
}


// -----------------------------
// �N�G�X�g�̏�����
// -----------------------------
void QuestManager::InitializeQuest()
{
	CreateQuestList_1st();
}

// -----------------------------
// �X�V����
// -----------------------------
void QuestManager::Update(float elapsedTime)
{
	// �N�G�X�g�̍X�V
	UpdateQuest();
}


// -----------------------------
// �N�G�X�g�̍X�V
// -----------------------------
void QuestManager::UpdateQuest()
{
	// �N�G�X�g�̍X�V
	if (m_currentQuestNo < m_questList.size())
	{
		// �N�G�X�g�̎��s
		auto quest = m_questList[m_currentQuestNo];
		if (quest->ExecuteChecker(m_playScene))
		{
			// ���̃N�G�X�g��
			m_currentQuestNo++;
		}
	}
}


// -----------------------------
// �G�t�F�N�g�̍X�V����
// -----------------------------
void QuestManager::UpdateEffect(float elapsedTime)
{
}



// -----------------------------
// �N�G�X�g�̕`��
// -----------------------------
void QuestManager::DrawQuest()
{

}


// -----------------------------
// �N�G�X�g�}�l�[�W���[�̏I��
// -----------------------------
void QuestManager::Finalize()
{
}


// -----------------------------------
// �N�G�X�g���X�g�̍쐬 �X�e�[�W�P
// -----------------------------------
void QuestManager::CreateQuestList_1st()
{
	// �N�G�X�g�f�[�^�̏���
	ClearQuestData();

	// �N�G�X�g�̍쐬
	m_questList.push_back(new QuestPlayerMove());
	m_questList.push_back(new QuestPlayerAttack());
	m_questList.push_back(new QuestPlayerCombo());
	m_questList.push_back(new QuestPlayerAvoid());
}


// -----------------------------------
// �N�G�X�g���X�g�̍쐬 �X�e�[�W�Q
// -----------------------------------

void QuestManager::CreateQuestList_2nd()
{
	// �N�G�X�g�f�[�^�̏���
	ClearQuestData();
}


// -----------------------------------
// �N�G�X�g���X�g�̍쐬 �X�e�[�W�R
// -----------------------------------

void QuestManager::CreateQuestList_3rd()
{
	// �N�G�X�g�f�[�^�̏���
	ClearQuestData();
}


// -----------------------------------
// �N�G�X�g�f�[�^�̏���
// -----------------------------------
void QuestManager::ClearQuestData()
{
	// �N�G�X�g���X�g��S�č폜����
	for (auto quest : m_questList)
	{
		delete quest;
	}

	// �N�G�X�g���X�g���N���A
	m_questList.clear();
}
