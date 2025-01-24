#pragma once	// ���d�ǂݍ��ݖh�~
#include "Interface/IState.h"
#include "Interface/IObject.h"
#include "Interface/IFace.h"
#include "Interface/IEnemy.h"

class PlayScene;
class BehaviorTree;
class EnemyDamageEffect;
class EnemyDeadEffect;
class HPSystem;


#include "Face/Header/EnemyFaceIdling.h"
#include "Face/Header/EnemyFaceAttacking.h"

// ===== �G�̏�� =================================================================
#include "States/Header/EnemyStarting.h"		// �J�n���
#include "States/Header/EnemyIdling.h"			// �ҋ@���
#include "States/Header/Enemy_Attacking.h"		// ���������U��
#include "States/Header/Enemy_Sweeping.h"		// �ガ�����U��
#include "States/Header/EnemyDashAttacking.h"	// �ˌ����
#include "States/Header/EnemyApproaching.h"		// �ǔ����
#include "States/Header/EnemyDead.h"			// ���S���



class Enemy : public IEnemy
{
public:
	// �Œ�l
	static const float ENEMY_SPEED;
	static const float ENEMY_SCALE;
	static const float COOL_TIME;

	static constexpr float COLISION_POS_Y	= 10.0f;
	static constexpr float HP				= 20.0f;
	static constexpr float COLLISION_RADIUS = 20.0f;
	static constexpr float ENEMY_HEIGHT		= 50.0f;


public:
	// ���̃_���[�W���󂯂鋖���o��
	void CanHitSword() { m_canHit = true; }
	// ���̃_���[�W���󂯂鋖��������
	void CanNotHitSword() { m_canHit = false; }

public:
	// /////////////////�G�̊�b����n���֐�/////////////////////////////////////////////////////////////////////
	PlayScene*						GetPlayScene	()	const	{ return m_playScene;	}	// PlayScene�̎擾 
	HPSystem*						GetEnemyHP		()	const	{ return m_hp.get();	}	// HP�̎擾	
	DirectX::SimpleMath::Vector3	GetPosition		()	override{ return m_position;	}	// �S�̍��W���擾����
	float							GetAngle		()	const	{ return m_angle;		}	// �S�̉�]�p���擾����
	float							GetBodyTilt		()	const	{ return m_bodyTilt;	}	// �̂̌X�����擾����
	DirectX::SimpleMath::Matrix		GetWorldMatrix	()	const	{ return m_worldMatrix; }	// �G�̃��[���h���W���擾����
	HPSystem*						GetHPSystem		()	override{ return m_hp.get();	}	// HP�̎擾

	void SetPosition	(const DirectX::SimpleMath::Vector3 pos)	{ m_position = pos;		}	// �S�̍��W��ݒ肷��
	void SetAngle		(const float angle)							{ m_angle = angle;		}	// �S�̉�]�p��ݒ肷��
	void SetBodyTilt	(const float tilt)							{ m_bodyTilt = tilt;	}	// �̂̌X����ݒ肷��
	void SetWorldMatrix	(DirectX::SimpleMath::Matrix mat)			{ m_worldMatrix = mat;	}	// �G�̃��[���h���W��ݒ肷��

	////////////////////�G�̓����蔻���n���֐�/////////////////////////////////////////////////////////////////////
	DirectX::BoundingSphere GetBodyCollision() const { return *m_bodyCollision.get(); }	// �̂̓����蔻����擾����

	////////////////////�G�̃X�e�[�g��n���֐�/////////////////////////////////////////////////////////////////////
	EnemyIdling*		GetEnemyIdling		() const { return m_idling		.get();	}	// �ҋ@���
	Enemy_Attacking*	GetEnemyAttacking	() const { return m_attacking	.get();	}	// �U�����
	Enemy_Sweeping*		GetEnemySweeping	() const { return m_sweeping	.get();	}	// �ガ�������
	EnemyDashAttacking* GetEnemyDashAttacking()const { return m_dashAttacking.get();}	// �ˌ����
	EnemyApproaching*	GetEnemyApproaching	() const { return m_approaching	.get();	}	// �ǔ����
	EnemyDead*			GetEnemyDead		() const { return m_dead		.get(); }	// ���S���

	////////////////////�@��@/////////////////////////////////////////////////////////////////////////////////////
	void SetFace(IFace* face) { m_currentFace = face; }	// ��̐ݒ�
	EnemyFaceIdling*	GetFaceIdling	() const { return m_faceIdling		.get();	}	// �ҋ@��
	EnemyFaceAttacking* GetFaceAttacking() const { return m_faceAttacking	.get();	}	// �U����

	void SetTargetLockOn(bool flag) { m_isTargetLockOn = flag; }	// ���b�N�I�����邩�ǂ���

	// ���݂̃X�e�[�g��Ԃ�
	IState* GetCurrentState() const { return m_currentState; }

public:
	// �R���X�g���N�^
	Enemy(PlayScene* playScene);
	// �f�X�g���N�^
	~Enemy();
	// ����������
	void Initialize();
	// �V������ԂɑJ�ڂ���i�X�e�[�g�p�^�[���j
	void ChangeState(IState* newState);
	// �X�V����
	void Update(float elapsedTime);
	void CalcrationWorldMatrix();
	// �`�揈��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// �I������
	void Finalize();
	// ���S�������s��
	void DeadAction();

	// �X�e�[�g�̍쐬����
	void CreateState();
	// ��̍쐬����
	void CreateFace();

	// �����蔻��̐�������
	void CreateCollision();
	// ���������Ƃ��̏���
	void HitAction(InterSectData data)override;
	// �Փ˃N�[���^�C�����v��
	void CheckHitCoolTime(float elapsedTime);

	void HitSword(InterSectData data);
	void HitStage(InterSectData data);

	void CheckAlive();

private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;
	// �̂̌X��
	float m_bodyTilt;

	// �G�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// HP
	std::unique_ptr<HPSystem> m_hp;


	// ==== �X�e�[�g�p�^�[���Ɏg�p =============================================�@
	IState* m_currentState;			// ���݂̃X�e�[�g�i�X�e�[�g�p�^�[���j
	std::unique_ptr<EnemyStarting>		m_starting;		// �J�n���
	std::unique_ptr<EnemyIdling>		m_idling;		// �ҋ@���
	std::unique_ptr<Enemy_Attacking>	m_attacking;	// �U�����
	std::unique_ptr<Enemy_Sweeping>		m_sweeping;		// �ガ�������
	std::unique_ptr<EnemyDashAttacking>	m_dashAttacking;// �ˌ����
	std::unique_ptr<EnemyApproaching>	m_approaching;	// �ǔ����
	std::unique_ptr<EnemyDead>			m_dead;			// ���S���


	// ==== ��p�[�c ============================================================
	IFace* m_currentFace;			// ���݂̊�
	std::unique_ptr<EnemyFaceIdling>	m_faceIdling;		// �ҋ@��
	std::unique_ptr<EnemyFaceAttacking>	m_faceAttacking;	// �U����

	// �G�̃_���[�W�G�t�F�N�g
	std::unique_ptr<EnemyDamageEffect> m_damageEffect;
	// �G�̎��S�G�t�F�N�g
	std::unique_ptr<EnemyDeadEffect> m_deadEffect;

	// �r�w�C�r�A�c���[
	std::unique_ptr<BehaviorTree> m_pBT;

	// ���f��
	DirectX::Model* m_model;

	//�@�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �����߂��ʂ̌v�Z
	DirectX::SimpleMath::Vector3 m_pushBackValue;

	// �v���C�V�[��(�����蔻��̏����Ɏg�p)
	PlayScene* m_playScene;

	// �̂̓����蔻��
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
	bool m_isHit;
	float m_coolTime;

	// �Փˉ\���ǂ���
	bool m_canHit;

	// ���b�N�I�����邩�ǂ���
	bool m_isTargetLockOn;









	bool debug = false;
};
