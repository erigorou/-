#pragma once
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

// �I�u�W�F�N�g�̊��N���X�@============================================
#include "Interface/IObject.h"
#include "Interface/IObserver.h"

// �v���C���[�ɕt�^�������� ===========================================
#include "Game/Weapon/Sword/Sword.h"		// ����
#include "Game/Player/PlayerHP.h"			// �v���C���[��HP

// �v���C���[�̏�� =====================================================
#include "Game/Player/State/Header/Player_Idling.h"			// �ҋ@���
#include "Game/Player/State/Header/Player_Dodging.h"		// ������
#include "Game/Player/State/Header/Player_Attacking_1.h"	// �U����ԂP
#include "Game/Player/State/Header/Player_Attacking_2.h"	// �U����ԂQ
#include "Game/Player/State/Header/Player_Attacking_3.h"	// �U����ԂR
#include "Game/Player/State/Header/Player_Attacking_4.h"	// �U����ԂS

class Wall;


class Player :  public IObserver
{

// �Œ�l
public:
	// �������W
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	// ����
	static const float PLAYER_SPEED;
	// �傫��
	static const float PLAYER_SCALE;

	static constexpr float APPLIED_ATTACK_TIME	= 1.4f;	// �Q�i�ڈȍ~�̍U�����������悤�ɂȂ鎞��
	static constexpr float APPLIED_DODGE_TIME	= 0.7f;	// ������ł���悤�ɂȂ鎞��
	static constexpr float COOL_TIME			= 1.0f; // �U���̃N�[���^�C��
	static constexpr float X_COOL_TIME			= 0.8f; // ���̍U�����ł���悤�ɂȂ鎞��

	static constexpr float NORMAL_ATTACK_TIME	= 0.5f; // �ʏ�U���̃A�j���[�V��������

// �A�N�Z�T
public:
	// /////////////////�v���C���[�̊�b����n���֐�//////////////////////////////////////////////////
	DirectX::SimpleMath::Vector3	GetPosition		()override	{ return m_position;			}
	DirectX::SimpleMath::Vector3	GetVelocity		()	const	{ return m_velocity;			}
	DirectX::SimpleMath::Vector3	GetDirection	()	const	{ return m_direction;			}
	float							GetAngle		()	const	{ return m_angle;				}
	PlayerHP*						GetPlayerHP		()	const	{ return m_hp.get();			}
	DirectX::BoundingSphere*		GetBodyCollision()			{ return m_bodyCollision.get(); }


	////////////////////�v���C���[��{����ݒ肷��֐�/////////////////////////////////////////////////
	void SetPosition(DirectX::SimpleMath::Vector3 position)			{ m_position = position; }


	////////////////////�v���C���[�̃X�e�[�g��n���֐�//////////////////////////////////////////////////
	PlayerIdling*		GetPlayerIdlingState	()	const { return m_playerIdling.		get(); }
	PlayerDodging*		GetPlayerDodgingState	()	const { return m_playerDodging.		get(); }
	PlayerAttacking_1*	GetPlayerAttackingState1()	const { return m_playerAttacking_1.	get(); }
	PlayerAttacking_2*	GetPlayerAttackingState2()	const { return m_playerAttacking_2.	get(); }
	PlayerAttacking_3*	GetPlayerAttackingState3()	const { return m_playerAttacking_3.	get(); }
	PlayerAttacking_4*	GetPlayerAttackingState4()	const { return m_playerAttacking_4.	get(); }

	////////////�v���C���[�̈ړ��Ɋւ���X�e�[�g��ݒ肷��֐�///////////////////////////////////////////
	void SetSpeed		(DirectX::SimpleMath::Vector3 velocity)		{ m_velocity = velocity;			}
	void SetAcceleration(DirectX::SimpleMath::Vector3 acceleration)	{ m_acceleration = acceleration;	}
	void SetAngle		(const float angle)							{ m_angle = angle;					}

	////////////////////�v���C�V�[���Ɋ�����̂Ɏg�p��r�֐�/////////////////////////////////////////////
	PlayScene* GetPlayScene()const { return m_playScene; }

	//////////////////////////////////�Փ˔���Ɏg�p//////////////////////////////////////////////////////
	void CanHit(bool flag) { m_canHit = flag; }

	//////////////////////////////////�A�j���[�V�����Ɏg�p////////////////////////////////////////////////
	void SetAnimationRotate(DirectX::SimpleMath::Vector3 rotate) { m_animationRotate = rotate; }


// ���J�֐�
public:
	// �R���X�g���N�^
	Player(PlayScene* playScene);
	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize();
	// �����蔻��̐����֐�
	void CreateCollision();
	// �X�e�[�g�̍쐬�֐�
	void CreateState();
	// �V������ԂɑJ�ڂ���
	void ChangeState(IPlayer* newState);
	// ���Ԍv�����s��
	void TimeComparison(float& nowTime, const float totalTime, IPlayer* newState, const float elapsedTime);
	// �X�V����
	void Update(
		const DirectX::SimpleMath::Vector3	enemyPos,
		const float							elapsedTime);

	// �`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix&	view,
		const DirectX::SimpleMath::Matrix&	projection);

	// �I������
	void Finalize();
	// ��]�p�̌v�Z
	void CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos);
	// Matrix�̌v�Z
	void CalculationMatrix();
	// �ړ��̊Ǘ�
	void MovePlayer();

	// ���������Ƃ��̏���
	void HitAction(InterSectData data) override;

	// �L�[�{�[�h�̓��͂��擾����
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;

	// �L�[�{�[�h�̓��͂��擾����
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;


// �����ϐ�
private:
	DirectX::SimpleMath::Vector3	m_position;			// �ʒu
	DirectX::SimpleMath::Vector3	m_velocity;			// ���x
	DirectX::SimpleMath::Vector3	m_inputVelocity;	// ���͕ێ��p�ϐ�
	DirectX::SimpleMath::Vector3	m_direction;		// ����
	DirectX::SimpleMath::Vector3	m_acceleration;		// �����x
	float							m_angle;			// ��]	
	DirectX::SimpleMath::Vector3	m_pushBackValue;	// �v�b�V���o�b�N�l


	// �v���C���[�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;

	// �X�e�[�g�֘A =================================================
	IPlayer* m_currentState;									// ���݂̃X�e�[�g
	std::unique_ptr<PlayerIdling> m_playerIdling;				// �ҋ@���
	std::unique_ptr<PlayerDodging> m_playerDodging;				// ������

	std::unique_ptr<PlayerAttacking_1> m_playerAttacking_1;		// �U����ԂP
	std::unique_ptr<PlayerAttacking_2> m_playerAttacking_2;		// �U����ԂQ
	std::unique_ptr<PlayerAttacking_3> m_playerAttacking_3;		// �U����ԂR
	std::unique_ptr<PlayerAttacking_4> m_playerAttacking_4;		// �U����ԂS


	// �v���C���[�ɕt�^�������� ============
	std::unique_ptr<Sword> m_sword;
	std::unique_ptr<PlayerHP> m_hp;

	// �V�F�[�_�[�Ɏg�p������� ==============
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	float m_particleTime;
	float m_elapsedTime;

// �����֐�
private:
	// �v���C�V�[��(���̃I�u�W�F�N�g�̏��̎擾�ȂǂɎg��)
	PlayScene* m_playScene;

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �̂̓����蔻�� ////////
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;	// �Փ˔���
	bool m_isHit;		// �Փ˂�����
	float m_coolTime;	// �N�[���^�C��
	bool m_canHit;		// �Փˉ\��

	// �A�j���[�V�����p�ϐ�
	DirectX::SimpleMath::Vector3 m_animationRotate;
};
