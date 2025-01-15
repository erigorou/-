// ---------------------------------------------
// �u���e�v�S�Ă̓G�̊Ǘ����s���N���X
// �u���t�v12/18
// �u�쐬�ҁv�r�c
// ---------------------------------------------
#pragma once
#include "pch.h"
#include "Interface/IEnemy.h"

// �O���錾
class Enemy;
class Goblin;
class PlayScene;
class IObject;
class PlayScene;



// ---------------------------
// �G�̊Ǘ��N���X
// ---------------------------
class EnemyManager
{
// ** �\���� ** 
public:
	// �G�̎��
	enum class EnemyType
	{ Goblin,Boss };

	// �G�̃f�[�^
	struct EnemyData
	{
		EnemyType type;
		std::unique_ptr<IEnemy> data;
	};


// ---------------------------
// �Œ�l
// ---------------------------
public:
	// ���f���̃p�X
	static const wchar_t* GOBLIN_MODEL_PATH;
	static const wchar_t* BOSS_MODEL_PATH;

	// �S�u�����̍ő啦����
	static constexpr int MAX_GOBLIN = 3;


// ---------------------------
// �A�N�Z�T
// ---------------------------
public:
	// �{�X�̃|�C���^���擾
	Enemy* GetBossEnemy();
	// �{�X�̍��W���擾
	DirectX::SimpleMath::Vector3 GetBossPosition();
	// �^�[�Q�b�g�Ƃ��Ă���G�̍��W���擾
	DirectX::SimpleMath::Vector3 GetPicupEnemyPosition();


// ---------------------------
// ���J�֐�
// ---------------------------
public:
	// �R���X�g���N�^
	EnemyManager(PlayScene* playScene);
	// �f�X�g���N�^
	~EnemyManager();
	// ����������
	void Initialize(PlayScene* playScene);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);
	// �I������
	void Finalize();
	// �G�̐�������
	void GenerateEnemy(const DirectX::SimpleMath::Vector3& position, EnemyType type);

	// �S�u�����̑S�폜
	void DeleteAllGoblin();

	// �S�ẴS�u������HP��0�ɂ���
	void AllGoblinHPZero();

	// �G�P�̂̍폜
	void DeleteEnemy(IEnemy* enemy);

	// �J�����̃^�[�Q�b�g�̐ݒ�
	void ChangeCameraTarget();

	// �G�������Ă��邩�̃t���O
	bool IsEnemysAlive();


// ---------------------------
// �����֐�
// ---------------------------
private:

	// �G�����֘A *****
	// �N�G�X�g�̑I��
	void GenerateStartEnemy();
	// �N�G�X�g�P�̓G����
	void GenerateEnemy0();
	// �N�G�X�g�Q�̓G����
	void GenerateEnemy1();
	// �N�G�X�g�R�̓G����
	void GenerateEnemy2();


	// �S�u�����̐�������
	void GenerateGoblin(const DirectX::SimpleMath::Vector3& position);
	// �{�X�̐�������
	void GenerateBoss(const DirectX::SimpleMath::Vector3& position);

	// ���f���̐�������
	DirectX::Model* CreateModel(const wchar_t* filePath);


// ---------------------------
// �����o�ϐ�
// ---------------------------
private:
	// �G�̔z��
	std::vector<EnemyData> m_enemies;

	// �v���C�V�[��
	PlayScene* m_playScene;

	// ���݃^�[�Q�b�g�ɂ��Ă���G�̔z��ԍ�
	int m_targetEnemyIndex;

	DirectX::Model* m_goblinModel;
	DirectX::Model* m_bossModel;

	// �C���f�b�N�X��
	int m_selectQuestIndex;
};