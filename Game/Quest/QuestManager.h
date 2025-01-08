// ---------------------------------------------------
// �X�e�[�W�N���A�̃N�G�X�g���Ǘ�����N���X
// ---------------------------------------------------

#pragma once
#include "pch.h"

class PlayScene;
class IQuestChecker;

class QuestManager
{
// ---------------
// �Œ�l
// ---------------
public:
	// �X�e�[�W�̍ő吔
	static constexpr int MAX_STAGE = 3;

	// �X�e�[�W���Ƃ̃N�G�X�g�̍ő吔
	static constexpr int MAX_QUEST_1 = 5;
	static constexpr int MAX_QUEST_2 = 1;
	static constexpr int MAX_QUEST_3 = 1;

	// �N�[���^�C��
	static constexpr float COOL_TIME = 1.0f;

// ---------------
// ���J�֐�
// ---------------
public:
	// �R���X�g���N�^
	QuestManager(PlayScene* playScene);
	// �f�X�g���N�^
	~QuestManager();
	// �N�G�X�g�̏�����
	void InitializeQuest();
	// �X�V����
	void Update(float elapsedTime);
	// �N�G�X�g�̕`��
	void DrawQuest();
	// �N�G�X�g�̏I��
	void Finalize();


// ---------------
// �����֐�
// ---------------
private:
	// �N�G�X�g�̍X�V
	void UpdateQuest();
	// �G�t�F�N�g�̍X�V����
	void UpdateEffect(float elapsedTime);


	// �N�G�X�g���X�g�̍쐬 1st
	void CreateQuestList_1st();
	// �N�G�X�g���X�g�̍쐬 2nd
	void CreateQuestList_2nd();
	// �N�G�X�g���X�g�̍쐬 3rd
	void CreateQuestList_3rd();
	// �N�G�X�g�f�[�^�̏���
	void ClearQuestData();

// ---------------
// �����o�ϐ�
// ---------------
private:

	// ���݂̃N�G�X�g�ԍ�
	int m_currentQuestNo;

	// �v���C�V�[��
	PlayScene* m_playScene;

	// �N�G�X�g�̃��X�g
	std::vector<IQuestChecker*> m_questList;

	// �o�ߎ���
	float m_totalTime;

	// �N�G�X�g�̃N���A���\��
	bool m_canClear;
};
