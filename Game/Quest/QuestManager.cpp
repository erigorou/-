// ---------------------------------------------
// �N�G�X�g�}�l�[�W���[�N���X�̎���
// ---------------------------------------------

#include "pch.h"
#include "QuestManager.h"
#include "../Scene/PlayScene.h"
#include "QuestRenderer/QuestRenderer.h"

#include "Interface/IQuestChecker.h"
#include "QuestList/QuestPlayerMove.h"
#include "QuestList/QuestPlayerAttack.h"
#include "QuestList/QuestPlayerCombo.h"
#include "QuestList/QuestPlayerAvoid.h"


#include "Libraries/MyLib/DebugString.h"


// -----------------------------
// �R���X�g���N�^
// -----------------------------
QuestManager::QuestManager(PlayScene* playScene)
	: m_playScene(playScene)
	, m_currentQuestNo()	// ��
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
	// �N�G�X�g���X�g�̍쐬
	CreateQuestList_1st();

	// �N�G�X�g���e�̕`��I�u�W�F�N�g
	m_renderer = std::make_unique<QuestRenderer>(this);
	// ����������
	m_renderer->Initialize(m_textureList[0]);
}

// -----------------------------
// �X�V����
// -----------------------------
void QuestManager::Update(float elapsedTime)
{
	// �N�G�X�g�̍X�V
	UpdateQuest();

	// �`��̍X�V
	m_renderer->Update(elapsedTime);
}


// -----------------------------
// �N�G�X�g�̍X�V
// -----------------------------
void QuestManager::UpdateQuest()
{
	m_renderer->IsClear(true);

	// �N�G�X�g�̍X�V
	if (m_currentQuestNo < m_questList.size())
	{
		// �N�G�X�g�̎��s
		auto quest = m_questList[m_currentQuestNo];

		// �N�G�X�g�̃N���A��`��I�u�W�F�N�g�ɒʒm
		m_renderer->IsClear(quest->ExecuteChecker(m_playScene));
	}
}


// -----------------------------
// �N�G�X�g����X�V����
// -----------------------------
void QuestManager::ChangeNextQuest()
{
	// �N�G�X�g���P�i�߂�
	m_currentQuestNo++;

	// �N�G�X�g���Ō�܂ōs���Ă��Ȃ��ꍇ
	if (m_currentQuestNo < m_questList.size())
	{
		// �N�G�X�g�̕`��
		m_renderer->ChangeTexture(m_textureList[m_currentQuestNo]);
	}
	else
	{
		// �N�G�X�g�N���A
	}
}



// -----------------------------
// �N�G�X�g�̕`��
// -----------------------------
void QuestManager::DrawQuest()
{
	// �N�G�X�g�̕`��
	m_renderer->Draw();
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
	m_questList.push_back(new QuestPlayerMove	());
	m_questList.push_back(new QuestPlayerAttack	());
	m_questList.push_back(new QuestPlayerCombo	());
	m_questList.push_back(new QuestPlayerAvoid	());

	// �N�G�X�g�̒��̃e�N�X�`����ǂݍ���
	AddQuestTexture();
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
// �N�G�X�g�̃e�N�X�`����ǂݍ���
// -----------------------------------
void QuestManager::AddQuestTexture()
{
	// �ݒ肵���N�G�X�g����S�Ĕ����o��
	for (auto quest : m_questList)
	{
		// �e�N�X�`���̓ǂݍ���
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		DX::ThrowIfFailed(
			DirectX::CreateWICTextureFromFile(
				CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice(),
				quest->GetTexturePath(),
				nullptr,
				texture.ReleaseAndGetAddressOf()
			)
		);

		// �e�N�X�`�����X�g�ɒǉ�
		m_textureList.push_back(texture);
	}
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

	// �摜���X�g�����ׂč폜����
	for (auto texture : m_textureList)
	{
		texture.Reset();
	}

	// �N�G�X�g���X�g�ƃe�N�X�`�����X�g���N���A����
	m_questList.clear();
	m_textureList.clear();
}
