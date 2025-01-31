#pragma once	// ���d�ǂݍ��ݖh�~
#include "Interface/IState.h"
#include "Interface/IObject.h"
#include "Interface/IFace.h"
#include "Interface/IEnemy.h"

class PlayScene;
class BehaviorTree;
class EnemyEffect;
class HPSystem;
class Cudgel;


#include "Face/Header/BossFaceIdling.h"
#include "Face/Header/BossFaceAttacking.h"

// ===== �G�̏�� =================================================================
#include "States/Header/BossStarting.h"		// �J�n���
#include "States/Header/BossIdling.h"		// �ҋ@���
#include "States/Header/BossAttacking.h"	// ���������U��
#include "States/Header/BossSweeping.h"		// �ガ�����U��
#include "States/Header/BossDashAttacking.h"// �ˌ����
#include "States/Header/BossApproaching.h"	// �ǔ����
#include "States/Header/BossDead.h"			// ���S���


class Boss : public IEnemy
{
public:
	// �Œ�l
	static constexpr float BOSS_SPEED = 0.1f;
	static constexpr float BOSS_SCALE= 0.6f;
	static constexpr float COOL_TIME = 0.3f;

	static constexpr float COLISION_POS_Y	= 10.0f;
	static constexpr float HP				= 20.0f;
	static constexpr float COLLISION_RADIUS = 20.0f;
	static constexpr float Boss_HEIGHT		= 50.0f;


public:
	// ���̃_���[�W���󂯂鋖���o��
	void CanHitSword() { m_canHit = true; }
	// ���̃_���[�W���󂯂鋖��������
	void CanNotHitSword() { m_canHit = false; }


// --------------------------------
//  �A�N�Z�T�֐�
// --------------------------------
public:
	// �擾
	HPSystem*						GetBossHP		()	const	{ return m_hp.get();	}	// HP
	DirectX::SimpleMath::Vector3	GetPosition		()	override{ return m_position;	}	// �S�̍��W
	float							GetAngle		()	const	{ return m_angle;		}	// �S�̉�]�p
	float							GetBodyTilt		()	const	{ return m_bodyTilt;	}	// �̂̌X��
	DirectX::SimpleMath::Matrix		GetWorldMatrix	()	const	{ return m_worldMatrix; }	// �G�̃��[���h���W
	HPSystem*						GetHPSystem		()	override{ return m_hp.get();	}	// HP
	DirectX::BoundingSphere			GetBodyCollision()	const	{ return *m_bodyCollision.get(); }	// �̂̓����蔻��

	// ��Ԑݒ�
	IState*			GetCurrentState			() const { return m_currentState;		}	// ����
	BossIdling*		GetBossIdling			() const { return m_idling.get();		}	// �ҋ@
	BossAttacking*	GetBossAttacking		() const { return m_attacking.get();	}	// �U��
	BossSweeping*		GetBossSweeping		() const { return m_sweeping.get();		}	// �ガ����
	BossDashAttacking* GetBossDashAttacking	() const { return m_dashAttacking.get();}	// �ˌ�
	BossApproaching*	GetBossApproaching		() const { return m_approaching.get();	}	// �ǔ�
	BossDead*			GetBossDead			() const { return m_dead.get();			}	// ���S

	// �ݒ�
	void SetPosition	(const DirectX::SimpleMath::Vector3 pos)	{ m_position = pos;		}	// �S�̍��W
	void SetAngle		(const float angle)							{ m_angle = angle;		}	// �S�̉�]�p
	void SetBodyTilt	(const float tilt)							{ m_bodyTilt = tilt;	}	// �̂̌X��
	void SetWorldMatrix	(DirectX::SimpleMath::Matrix mat)			{ m_worldMatrix = mat;	}	// �G�̃��[���h���W

	// ��̃p�[�c
	void SetFace(IFace* face) { m_currentFace = face; }	// ��̐ݒ�
	BossFaceIdling*	GetFaceIdling	() const { return m_faceIdling		.get();	}	// �ҋ@��
	BossFaceAttacking* GetFaceAttacking() const { return m_faceAttacking	.get();	}	// �U����


public:
	// �R���X�g���N�^
	Boss();
	// �f�X�g���N�^
	~Boss();
	// ����������
	void Initialize();
	// �V������ԂɑJ�ڂ���i�X�e�[�g�p�^�[���j
	void ChangeState(IState* newState);
	// �X�V����
	void Update(float elapsedTime);
	// ���[���h�s��̌v�Z
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
	// �Փˉ\
	void CanHit(bool flag) override { m_canHit = flag; }
	// ���������Ƃ��̏���
	void HitAction(InterSectData data)override;
	// �Փ˃N�[���^�C�����v��
	void CheckHitCoolTime(float elapsedTime);
	// ���̏Փ˔���
	void HitSword(InterSectData data);
	// �X�e�[�W�̏Փ˔���
	void HitStage(InterSectData data);
	// ���S����
	void CheckAlive();

private:
	// ����
	std::unique_ptr<Cudgel> m_cudgel;
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

	IState* m_currentState;			// ���݂̃X�e�[�g�i�X�e�[�g�p�^�[���j
	std::unique_ptr<BossStarting>		m_starting;		// �J�n���
	std::unique_ptr<BossIdling>		m_idling;		// �ҋ@���
	std::unique_ptr<BossAttacking>	m_attacking;	// �U�����
	std::unique_ptr<BossSweeping>		m_sweeping;		// �ガ�������
	std::unique_ptr<BossDashAttacking>	m_dashAttacking;// �ˌ����
	std::unique_ptr<BossApproaching>	m_approaching;	// �ǔ����
	std::unique_ptr<BossDead>			m_dead;			// ���S���

	IFace* m_currentFace;			// ���݂̊�
	std::unique_ptr<BossFaceIdling>	m_faceIdling;		// �ҋ@��
	std::unique_ptr<BossFaceAttacking>	m_faceAttacking;	// �U����
	// �G�̃_���[�W�G�t�F�N�g
	std::unique_ptr<EnemyEffect> m_effect;
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
	// �̂̓����蔻��
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
	// �Փ˂��Ă��邩
	bool m_isHit;
	// �Փ˃N�[���^�C��
	float m_coolTime;
	// �Փˉ\��
	bool m_canHit;
	// �J�����̗h�炷������
	float m_shakePower;
};
