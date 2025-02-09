// --------------------------------------------------
// ���O:	Player.h
// ���e:	�v���C���[�N���X
//			�v���C���[�̃X�e�[�g��Փ˔���A�`���A�j���[�V��������
// �쐬:	�r�c����
// --------------------------------------------------
// �C���N���[�h
#pragma once
#include "Interface/IObserver.h"
#include "Game/Player/State/Header/PlayerIdling.h"
#include "Game/Player/State/Header/PlayerDodging.h"
#include "Game/Player/State/Header/PlayerAttacking1.h"
#include "Game/Player/State/Header/PlayerAttacking2.h"
#include "Game/Player/State/Header/PlayerNockBacking.h"

// �N���X�̑O���錾
class Sword;
class HPSystem;

/// <summary>
/// �v���C���[�̃X�e�[�g
/// </summary>
enum class PlayerState : int
{
	Idling,		// �ҋ@
	Dodging,	// ���
	Attacking1,	// �U���P
	Attacking2,	// �U���Q
	NockBacking,// ����
};

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public IObserver
{
	// -----------------
	// �萔
	// -----------------
public:
	// �������W
	static constexpr DirectX::SimpleMath::Vector3 HOME_POSITION = { 40.0f, 0.0f, 40.0f };
	// ����
	static constexpr float PLAYER_SPEED = 0.4f;
	// �傫��
	static constexpr float PLAYER_SCALE = 0.1f;
	// �����蔻��̑傫��
	static constexpr float COLLISION_RADIUS = 12.0f;
	// �v���C���[��HP
	static constexpr float PLAYER_HP = 5.0f;
	// �Q�i�ڈȍ~�̍U�����������悤�ɂȂ鎞��
	static constexpr float APPLIED_ATTACK_TIME = 1.4f;
	// ������ł���悤�ɂȂ鎞��
	static constexpr float APPLIED_DODGE_TIME = 0.5f;
	// �U���̃N�[���^�C��
	static constexpr float COOL_TIME = 1.0f;
	// ���̍U�����ł���悤�ɂȂ鎞��
	static constexpr float X_COOL_TIME = 0.7f;
	// �ʏ�U���̃A�j���[�V��������
	static constexpr float NORMAL_ATTACK_TIME = 0.5f;

	// �{�X�̍U����
	static constexpr float BOSS_ATTACK_POWER = 1.0f;
	// �{�X�̕���(���_)�̍U����
	static constexpr float BOSS_CUDGEL_POWER = 1.0f;
	// ���S�̍U����
	static constexpr float GOBLIN_ATTACK_POWER = 1.0f;

	// �X�e�[�g�̍ő吔
	static constexpr int STATE_MAX = 5;

	// �L�[����
	static constexpr DirectX::SimpleMath::Vector2 INPUT_FLONT = { 0.0f	, 1.0f };	// �O
	static constexpr DirectX::SimpleMath::Vector2 INPUT_BACK = { 0.0f	, -1.0f };	// ��
	static constexpr DirectX::SimpleMath::Vector2 INPUT_LEFT = { -1.0f	, 0.0f };	// ��
	static constexpr DirectX::SimpleMath::Vector2 INPUT_RIGHT = { 1.0f	, 0.0f };	// �E

	// -----------------
	// �A�N�Z�T
	// -----------------
public:
	// �v���C���[�̃I�u�W�F�N�g���擾
	Player* GetObject() { return this; }
	// ���W���擾
	DirectX::SimpleMath::Vector3 GetPosition()override { return m_position; }
	// ���W��ݒ�
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	// ���x
	DirectX::SimpleMath::Vector3 GetVelocity()	const { return m_velocity; }
	// ���x
	void SetSpeed(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }
	// ����
	DirectX::SimpleMath::Vector3 GetDirection()	const { return m_direction; }

	// ���̓x�N�g��
	DirectX::SimpleMath::Vector2 GetinputVector()	const { return m_inputVector; }
	// ��]�p
	float GetAngle()	const { return m_angle; }
	// ��]�p
	void SetAngle(const float angle) { m_angle = angle; }
	// HP
	HPSystem* GetPlayerHP()	const { return m_hp.get(); }

	// �A�j���[�V�����p��]
	void SetAnimationRotate(DirectX::SimpleMath::Vector3 rotate) { m_animationRotate = rotate; }
	// ���̓x�N�g��
	void SetInputVector(DirectX::SimpleMath::Vector2 inputVector) { m_inputVector = inputVector; }
	// �����x
	void SetAcceleration(DirectX::SimpleMath::Vector3 acceleration) { m_acceleration = acceleration; }

	PlayerIdling* GetPlayerIdlingState()	const { return m_playerIdling.get(); }	// �ҋ@���
	PlayerDodging* GetPlayerDodgingState()	const { return m_playerDodging.get(); }	// ������
	PlayerAttacking1* GetPlayerAttackingState1()	const { return m_playerAttacking1.get(); }	// �U����ԂP
	PlayerAttacking2* GetPlayerAttackingState2()	const { return m_playerAttacking2.get(); }	// �U����ԂQ
	PlayerNockBacking* GetPlayerNockBackingState()	const { return m_playerNockBacking.get(); }	// ������
	IPlayer* GetCurrentState()			const { return m_currentState; }	// ���݂̃X�e�[�g


	// -------------------
	// ���J�֐�
	// -------------------
public:
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player();
	//  �X�e�[�g��ύX����
	void ChangeState(PlayerState state);
	// ����������
	void Initialize();
	// ���Ԍv�����s��
	void TimeComparison(float& nowTime, const float totalTime, PlayerState state, const float elapsedTime);
	// �X�V����
	void Update(const float elapsedTime);
	// �`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);
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
	
	// ��_�����󂯂���
	void CanHitBoss(void* flag);
	// ����Ƃ̓����蔻��
	void CanHitCudgel(void* flag);
	// �S�u�����Ƃ̓����蔻��
	void CanHitGoblin(void* flag);

	// -------------------
	// �����֐�
	// -------------------
private:
	// �����蔻��̐����֐�
	void CreateCollision();
	// �X�e�[�g�̍쐬�֐�
	void CreateState();
	// �`��֘A�̏������ݒ�
	void InitializeRender();
	// �C�x���g�̓o�^
	void AttachEvent();
	// �G�̑̂Ƃ̏Փ˔���
	void HitBossBody(InterSectData data);
	// �G�̃u�L�Ƃ̏Փ˔���
	void HitCudgel(InterSectData data);
	// �S�u�����ƏՓ�
	void HitGoblin(InterSectData data);
	// �X�e�[�W�Ƃ̏Փ˔���
	void HitStage(InterSectData data);
	// �_���[�W
	void Damage(float damage);

	// -------------------
	// �����ϐ�
	// -------------------
private:
	// ����
	std::unique_ptr<Sword> m_sword;
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// ���͕ێ��p�ϐ�
	DirectX::SimpleMath::Vector2 m_inputVector;
	// ����
	DirectX::SimpleMath::Vector3 m_direction;
	// �����x
	DirectX::SimpleMath::Vector3 m_acceleration;
	// ��]�A�j���[�V����
	DirectX::SimpleMath::Vector3 m_animationRotate;
	// ��]�p
	float m_angle;
	// �O��̌X�p
	float m_tilt;
	// �����߂���
	DirectX::SimpleMath::Vector3 m_pushBackValue;
	// �v���C���[�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �ړ��L�[��������Ă��邩
	bool m_isInputMoveKey;

	// ���݂̃X�e�[�g
	IPlayer* m_currentState;
	// �ҋ@���
	std::unique_ptr<PlayerIdling> m_playerIdling;
	// ������
	std::unique_ptr<PlayerDodging> m_playerDodging;
	// �U�����
	std::unique_ptr<PlayerAttacking1> m_playerAttacking1;
	// �U�����
	std::unique_ptr<PlayerAttacking2> m_playerAttacking2;
	// ������
	std::unique_ptr<PlayerNockBacking> m_playerNockBacking;
	// �X�e�[�g�̃��X�g
	IPlayer* m_states[STATE_MAX];

	// HP�V�X�e��
	std::unique_ptr<HPSystem> m_hp;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �p�[�e�B�N���̎���
	float m_particleTime;
	// �o�ߎ���
	float m_elapsedTime;
	// ���f��
	DirectX::Model* m_model;

	// �Փ˔���
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
	// �Փ˂�����
	bool m_isHit;
	// �Փ˔���̃N�[���^�C��
	float m_coolTime;

	// �{�X����̍U�����󂯂��邩
	bool m_canHitBoss;
	// �{�X�̕��킩��̍U�����󂯂��邩
	bool m_canHitCudgel;
	// �S�u��������̍U�����󂯂��邩
	bool m_canHitGoblin;
};
