#pragma once
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

// �I�u�W�F�N�g�̊��N���X�@============================================
#include "Interface/IObject.h"
#include "Interface/IObserver.h"

// �v���C���[�ɕt�^�������� ===========================================
#include "Game/Weapon/Sword/Sword.h"// ����
class HPSystem;						// HP

// �v���C���[�̏�� =====================================================
#include "Game/Player/State/Header/Player_Idling.h"			// �ҋ@���
#include "Game/Player/State/Header/Player_Dodging.h"		// ������
#include "Game/Player/State/Header/Player_Attacking_1.h"	// �U����ԂP
#include "Game/Player/State/Header/Player_Attacking_2.h"	// �U����ԂQ
#include "Game/Player/State/Header/Player_NockBacking.h"	// ������

class Wall;


class Player :  public IObserver
{

// �Œ�l *
public:
	// �������W
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	// ����
	static const float PLAYER_SPEED;
	// �傫��
	static const float PLAYER_SCALE;

	static constexpr float PLAYER_HP			= 5.0f;	// �v���C���[��HP
	static constexpr float APPLIED_ATTACK_TIME	= 1.4f;	// �Q�i�ڈȍ~�̍U�����������悤�ɂȂ鎞��
	static constexpr float APPLIED_DODGE_TIME	= 0.7f;	// ������ł���悤�ɂȂ鎞��
	static constexpr float COOL_TIME			= 1.0f; // �U���̃N�[���^�C��
	static constexpr float X_COOL_TIME			= 0.7f; // ���̍U�����ł���悤�ɂȂ鎞��

	static constexpr float NORMAL_ATTACK_TIME	= 0.5f; // �ʏ�U���̃A�j���[�V��������

	static constexpr DirectX::SimpleMath::Vector2 INPUT_FLONT	= { 0.0f	, 1.0f	};	// �O
	static constexpr DirectX::SimpleMath::Vector2 INPUT_BACK	= { 0.0f	, -1.0f };	// ��
	static constexpr DirectX::SimpleMath::Vector2 INPUT_LEFT	= { -1.0f	, 0.0f	};	// ��
	static constexpr DirectX::SimpleMath::Vector2 INPUT_RIGHT	= { 1.0f	, 0.0f	};	// �E


// �A�N�Z�T *
public:
	// /////////////////�v���C���[�̊�b����n���֐�//////////////////////////////////////////////////
	DirectX::SimpleMath::Vector3	GetPosition		()override	{ return m_position;			}
	DirectX::SimpleMath::Vector3	GetVelocity		()	const	{ return m_velocity;			}
	DirectX::SimpleMath::Vector3	GetDirection	()	const	{ return m_direction;			}
	DirectX::SimpleMath::Vector2	GetinputVector	()	const	{ return m_inputVector;			}
	float							GetAngle		()	const	{ return m_angle;				}
	HPSystem*						GetPlayerHP		()	const	{ return m_hp.get();			}
	DirectX::BoundingSphere*		GetBodyCollision()			{ return m_bodyCollision.get(); }


	////////////////////�v���C���[��{����ݒ肷��֐�/////////////////////////////////////////////////
	void SetPosition(DirectX::SimpleMath::Vector3 position)			{ m_position = position;	}
	void SetAnimarionRotate(DirectX::SimpleMath::Vector3 rotate)	{ m_animationRotate = rotate; }
	void SetInputVector(DirectX::SimpleMath::Vector2 inputVector)	{ m_inputVector = inputVector; }

	////////////////////�v���C���[�̃X�e�[�g��n���֐�//////////////////////////////////////////////////
	PlayerIdling*		GetPlayerIdlingState	()	const { return m_playerIdling.		get(); }
	PlayerDodging*		GetPlayerDodgingState	()	const { return m_playerDodging.		get(); }
	PlayerAttacking_1*	GetPlayerAttackingState1()	const { return m_playerAttacking_1.	get(); }
	PlayerAttacking_2*	GetPlayerAttackingState2()	const { return m_playerAttacking_2.	get(); }
	PlayerNockBacking* GetPlayerNockBackingState()	const { return m_playerNockBacking.	get(); }

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


// ���J�֐� *
public:
	// �R���X�g���N�^
	Player(PlayScene* playScene);
	// �f�X�g���N�^
	~Player();
	// �V������ԂɑJ�ڂ���
	void ChangeState(IPlayer* newState);
	// ����������
	void Initialize();
	// ���Ԍv�����s��
	void TimeComparison(float& nowTime, const float totalTime, IPlayer* newState, const float elapsedTime);
	// �X�V����
	void Update(const float elapsedTime);

	// �`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix&	view,
		const DirectX::SimpleMath::Matrix&	projection);

	// �I������
	void Finalize();
	// ��]�p�̌v�Z
	float CalucratePlayerRotation(DirectX::SimpleMath::Vector3 const enemyPos);
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


// �����֐� *
private:
	// �����蔻��̐����֐�
	void CreateCollision();
	// �X�e�[�g�̍쐬�֐�
	void CreateState();
	// �G�̑̂Ƃ̏Փ˔���
	void HitEnemyBody(InterSectData data);
	// �G�̃u�L�Ƃ̏Փ˔���
	void HitCudgel(InterSectData data);
	// �X�e�[�W�Ƃ̏Փ˔���
	void HitStage(InterSectData data);
	// �_���[�W
	void Damage(float damage);


// �����ϐ� *
private:
	DirectX::SimpleMath::Vector3	m_position;			// �ʒu
	DirectX::SimpleMath::Vector3	m_velocity;			// ���x
	DirectX::SimpleMath::Vector2	m_inputVector;		// ���͕ێ��p�ϐ�
	DirectX::SimpleMath::Vector3	m_direction;		// ����
	DirectX::SimpleMath::Vector3	m_acceleration;		// �����x
	float							m_angle;			// ��]	
	float							m_tilt;				// �X��
	DirectX::SimpleMath::Vector3	m_pushBackValue;	// �v�b�V���o�b�N�l

	bool m_isInputMoveKey;		// �ړ��L�[��������Ă��邩


	// �v���C���[�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;

	// �X�e�[�g�֘A =================================================
	IPlayer* m_currentState;									// ���݂̃X�e�[�g
	std::unique_ptr<PlayerIdling> m_playerIdling;				// �ҋ@���
	std::unique_ptr<PlayerDodging> m_playerDodging;				// ������
	std::unique_ptr<PlayerAttacking_1> m_playerAttacking_1;		// �U����ԂP
	std::unique_ptr<PlayerAttacking_2> m_playerAttacking_2;		// �U����ԂQ
	std::unique_ptr<PlayerNockBacking> m_playerNockBacking;		// ������

	// �v���C���[�ɕt�^�������� ============
	std::unique_ptr<Sword> m_sword;
	std::unique_ptr<HPSystem> m_hp;

	// �V�F�[�_�[�Ɏg�p������� ==============
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �p�[�e�B�N��
	float m_particleTime;
	float m_elapsedTime;

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
