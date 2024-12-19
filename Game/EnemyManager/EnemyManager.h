// ---------------------------------------------
// �u���e�v�S�Ă̓G�̊Ǘ����s���N���X
// �u���t�v12/18
// �u�쐬�ҁv�r�c
// ---------------------------------------------
#pragma once
#include "pch.h"

// �O���錾
class Enemy;
class Goblin;
class PlayScene;
class IEnemy;

class EnemyManager
{
// ** �Œ�l ** 
public:
	// �G�̎��
	enum class EnemyType
	{ Goblin,Boss };

	// �G�̃f�[�^
	struct EnemyData
	{ EnemyType type; IEnemy* enemy; };


	static const wchar_t* GOBLIN_MODEL_PATH;
	static const wchar_t* BOSS_MODEL_PATH;

	// �S�u�����̍ő啦����
	static constexpr int MAX_GOBLIN = 3;


// ** ���J�֐� **
public:
	// �R���X�g���N�^
	EnemyManager();
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

	void Finalize();

	void CreateGoblin(
		const DirectX::SimpleMath::Vector3 position
	);

	DirectX::Model* CreateModel(const wchar_t* filePath);

// ** ����J�֐� **
private:
	// ��������


// ** �����o�ϐ� **
private:
	std::unique_ptr<EnemyData> m_enemies;

	DirectX::Model* m_goblinModel;
	DirectX::Model* m_bossModel;
};