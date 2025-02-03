// -------------------------------------------------------
// ���S�̐e�N���X
// -------------------------------------------------------

#pragma once

#ifndef GOBLIN_OBJECT
#define GOBLIN_OBJECT

#include "pch.h"
#include "Interface/IObject.h"
#include "Interface/IState.h"
#include "Interface/IEnemy.h"

class Player;
class PlayScene;
class HPSystem;
class EnemyEffect;

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"
#include "State/Header/GoblinDead.h"
#include "State/Header/GoblinTutorial.h"

enum class GoblinState
{
	IDLING,
	ATTACKING,
	DEAD,
	TUTORIAL
};

class Goblin : public IEnemy
{
	// ---------------
	// �Œ�l
	// ---------------
public:
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	static constexpr float GOBLIN_HP = 6.0f;
	static constexpr float COLLISION_RADIUS = 16.0f;
	static constexpr float COLLISION_POS_Y = 2.0f;

	static constexpr float COOL_TIME = 0.1f;

	// ---------------
	// �A�N�Z�T
	// ---------------
public:
	// ���W���擾
	DirectX::SimpleMath::Vector3 GetPosition() override { return m_position; }
	// ���W��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// ���x���擾
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// ���x��ݒ�
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) { m_velocity = velocity; }
	// ��]�p���擾
	float GetAngle() const { return m_angle; }
	// ��]�p��ݒ�
	void SetAngle(const float angle) { m_angle = angle; }
	// ���[���h���W���擾
	DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMatrix; }
	// �X�P�[����ݒ�
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	// �̂̓����蔻����擾
	DirectX::BoundingSphere GetCollision() const { return *m_bodyCollision.get(); }
	// �U�������ǂ������擾
	bool IsAttacking() const { return m_nowAttacking; }
	// HP���擾
	HPSystem* GetHPSystem() override { return m_hp.get(); }
	// �U�������ǂ�����ݒ�
	void SetIsAttacking(bool isAttacking) { m_nowAttacking = isAttacking; }

	// ---------------
	// ���J�֐�
	// ---------------
public:
	// �R���X�g���N�^
	Goblin(PlayScene* playScene);
	// �f�X�g���N�^
	~Goblin();
	// �������֐�
	void Initialize();
	// �����蔻��̐���
	void CreateCollision();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	// �I������
	void Finalize();
	// �Փˉ\
	void CanHit(bool flag) override { m_canHit = flag; }
	// �Փˏ���
	void HitAction(InterSectData data) override;
	// �X�e�[�g�̕ύX
	void ChangeState(GoblinState state);
	// �S�u����������
	void DeleteGoblin();

	// ---------------
	// �����֐�
	// ---------------
private:
	// ���[���h�s��̌v�Z
	void CalcWorldMatrix();
	// �����蔻��̈ړ�
	void MoveCollision();
	// �����m�F
	void CheckAlive();

	void HitPlayer(InterSectData data);	// �v���C���[�ɓ��������Ƃ��̏���
	void HitGoblin(InterSectData data);	// ���S�ɓ��������Ƃ��̏���
	void HitBoss(InterSectData data);	// �G�ɓ��������Ƃ��̏���
	void HitStage(InterSectData data);	// �X�e�[�W�ɓ��������Ƃ��̏���
	void HitSword(InterSectData data);	// ���ɓ��������Ƃ��̏���

	void Damaged(float damage);			// �_���[�W���󂯂��Ƃ��̏���
	void CountCoolTime(float elapsedTime);	// �N�[���^�C���̃J�E���g

	void CreateState();		// �X�e�[�g�̍쐬

	// ---------------
	// �����o�ϐ�
	// ---------------
private:
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// ��]�p
	float m_angle;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// ���[���h���W
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	DirectX::Model* m_model;
	// �����߂���
	DirectX::SimpleMath::Vector3 m_pushBackValue;

	// ���݂̃X�e�[�g
	IState* m_currentState;
	// �ҋ@
	std::unique_ptr<GoblinIdling> m_idling;
	// �U��
	std::unique_ptr<GoblinAttacking> m_attacking;
	// ���S
	std::unique_ptr<GoblinDead> m_dead;
	// �`���[�g���A��
	std::unique_ptr<GoblinTutorial> m_tutorial;
	// �X�e�[�g�̃��X�g
	std::vector<IState*> m_states;

	// HP
	std::unique_ptr<HPSystem> m_hp;
	// �G�G�t�F�N�g
	std::unique_ptr<EnemyEffect> m_enemyEffect;

	// �v���C�V�[��(�����蔻��̏����Ɏg�p)
	PlayScene* m_playScene;

	// �̂̓����蔻��
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;

	// �U�����t���O
	bool m_nowAttacking;
	// �U�����󂯂��t���O
	bool m_isHit;
	// �Փˉ\�t���O
	bool m_canHit;
	// �N�[���^�C��
	float m_coolTime;
};

#endif // !GOBLIN_OBJECT