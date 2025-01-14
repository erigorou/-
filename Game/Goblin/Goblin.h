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
class EnemyDamageEffect;
#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"
#include "State/Header/GoblinDead.h"

class Goblin : public IEnemy
{
	// ---------------
	// �Œ�l
	// ---------------
public:
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	static constexpr float GOBLIN_HP = 1.0f;
	static constexpr float COLLISION_RADIUS = 16.0f;
	static constexpr float COLLISION_POS_Y = 2.0f;

	static constexpr float COOL_TIME = 0.4f;


	// ---------------
	// �A�N�Z�T
	// ---------------
public:
	DirectX::SimpleMath::Vector3	GetPosition		()	override{ return m_position;			}	// �S�̍��W���擾����
	DirectX::SimpleMath::Vector3	GetVelocity		()	const	{ return m_velocity;			}	// ���x�̎擾
	float							GetAngle		()	const	{ return m_angle;				}	// ��]�p�̎擾
	DirectX::SimpleMath::Matrix		GetWorldMatrix	()	const	{ return m_worldMatrix;			}	// ���[���h���W�̎擾
	DirectX::BoundingSphere 		GetCollision	()	const	{ return *m_bodyCollision.get();}	// �̂̓����蔻��̎擾
	PlayScene*						GetPlayScene	()	const	{ return m_playScene;			}	// PlayScene�̎擾
	bool							IsAttacking		()	const	{ return m_nowAttacking;		}	// �U�������ǂ����̎擾
	HPSystem*						GetHPSystem		()	override{ return m_hp.get();			}	// HP�̎擾

	void SetPosition(const DirectX::SimpleMath::Vector3& position)	{ m_position = position; }		// �S�̍��W��ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity)	{ m_velocity = velocity; }		// ���x�̐ݒ�
	void SetAngle	(const float angle)								{ m_angle = angle; }			// ��]�p�̐ݒ�
	void SetScale	(const DirectX::SimpleMath::Vector3& scale)		{ m_scale = scale; }			// �X�P�[���̐ݒ�
	void SetIsAttacking(bool isAttacking)							{ m_nowAttacking = isAttacking; }// �U�������ǂ����̐ݒ�

	GoblinIdling*	 GetIdling		()	const { return m_idling.get(); }	// �ҋ@��Ԃ̎擾
	GoblinAttacking* GetAttacking	()	const { return m_attacking.get(); }	// �U����Ԃ̎擾
	GoblinDead*		 GetDead		()	const { return m_dead.get(); }		// ���S��Ԃ̎擾
	IState* GetCurrentState			()	const { return m_currentState; }	// ���݂̃X�e�[�g�̎擾

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
		const DirectX::SimpleMath::Matrix& projection);

	// �I������
	void Finalize();
	// �Փˏ���
	void HitAction(InterSectData data) override;
	// �X�e�[�g�̕ύX
	void ChangeState(IState* state);

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
	void HitEnemy(InterSectData data);	// �G�ɓ��������Ƃ��̏���
	void HitStage(InterSectData data);	// �X�e�[�W�ɓ��������Ƃ��̏���
	void HitSword(InterSectData data);	// ���ɓ��������Ƃ��̏���

	void Damaged		(float damage);			// �_���[�W���󂯂��Ƃ��̏���
	void CountCoolTime	(float elapsedTime);	// �N�[���^�C���̃J�E���g

	void CreateState();		// �X�e�[�g�̍쐬


// ---------------
// �����o�ϐ�
// ---------------
private:
	DirectX::SimpleMath::Vector3 m_position;	// ���W
	DirectX::SimpleMath::Vector3 m_velocity;	// ���x
	float m_angle;								// ��]�p
	DirectX::SimpleMath::Vector3 m_scale;		// �X�P�[��
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ���[���h���W

	std::unique_ptr<DirectX::Model> m_model;	// ���f��

	// �����߂���
	DirectX::SimpleMath::Vector3 m_pushBackValue;

	// �X�e�[�g�p�ϐ� **
	IState* m_currentState;								// ���݂̃X�e�[�g
	std::unique_ptr<GoblinIdling>		m_idling;		// �ҋ@
	std::unique_ptr<GoblinAttacking>	m_attacking;	// �U��
	std::unique_ptr<GoblinDead>			m_dead;			// ���S

	// �V�X�e�� **
	std::unique_ptr<HPSystem> m_hp;						// HP
	std::unique_ptr<EnemyDamageEffect> m_damageEffect;	// �_���[�W�G�t�F�N�g

	// �v���C�V�[��(�����蔻��̏����Ɏg�p)
	PlayScene* m_playScene;

	// �̂̓����蔻��
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;

	bool m_nowAttacking;	// �U�������ǂ���
	bool m_isHit;			// �U�����󂯂����ǂ���
	float m_coolTime;		// �N�[���^�C��
};


#endif // !GOBLIN_OBJECT