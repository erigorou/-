// -------------------------------------------------------
// ���S�̐e�N���X
// -------------------------------------------------------

#include "pch.h"
#include "Goblin.h"
#include "Game/Player/Player.h"
#include "Game/Scene/PlayScene.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/DebugString.h"
#include "Interface/IState.h"
#include "Game/Data/HPSystem.h"
#include "Game/GameResources.h"
#include "../Boss/Boss.h"
#include "Effects/EnemyEffect/EnemyEffect.h"
#include "Game/HitStop/HitStop.h"
#include "Game/EnemyManager/EnemyManager.h"
#include "Game/Messenger/EventMessenger.h"

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"


// ---------------
// �Œ�l
// ---------------
const float Goblin::GOBLIN_SPEED = Boss::BOSS_SPEED / 2.0f;	// �ړ���
const float Goblin::GOBLIN_SCALE = Boss::BOSS_SCALE / 4.0f;	// �T�C�Y


// -------------------------------
// �R���X�g���N�^
// -------------------------------
Goblin::Goblin(PlayScene* playScene)
	: m_playScene(playScene)
	, m_position{}
	, m_velocity{}
	, m_angle{}
	, m_scale{1.0f, 1.0f, 1.0f}
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
	, m_nowAttacking(false)
	, m_pushBackValue{}
	, m_isHit(false)
	, m_canHit(false)
	, m_coolTime(0.0f)
{
}

// -------------------------------
// �f�X�g���N�^
// -------------------------------
Goblin::~Goblin()
{
}

// --------------------------------
// �������֐�
// --------------------------------
void Goblin::Initialize()
{
	// ���f���̓ǂݍ���
	m_model = GameResources::GetInstance()->GetModel("goblin");

	// �X�e�[�g�̍쐬
	CreateState();
	// �����蔻��̐���
	CreateCollision();
	// HP�̐���
	m_hp = std::make_unique<HPSystem>(GOBLIN_HP);
	// �G�t�F�N�g�̐���
	m_enemyEffect = std::make_unique<EnemyEffect>();

}

// --------------------------------
// �X�e�[�g�̍쐬
// --------------------------------
void Goblin::CreateState()
{
	m_idling	= std::make_unique<GoblinIdling>	(this);	// �ҋ@
	m_attacking = std::make_unique<GoblinAttacking>	(this);	// �U��
	m_dead		= std::make_unique<GoblinDead>		(this);	// ���S
	m_tutorial	= std::make_unique<GoblinTutorial>	(this);	// �`���[�g���A��

	m_idling->		Initialize();
	m_attacking->	Initialize();
	m_dead->		Initialize();
	m_tutorial->	Initialize();

	m_currentState = m_idling.get();
}


// --------------------------------
// �����蔻��̐���
// --------------------------------
void Goblin::CreateCollision()
{
	// �����蔻��̐���
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE * COLLISION_RADIUS);

	// �Փ˃f�[�^�̍쐬
	CollisionData<DirectX::BoundingSphere> data =
	{
		ObjectType::Goblin,		// �I�u�W�F�N�g�̎��
		CollisionType::Sphere,	// �����蔻��̎��
		this,					// ���̃N���X�̃|�C���^
		m_bodyCollision.get()	// �����蔻��̃|�C���^
	};

	// �Փ˔����Manager�ɓo�^
	EventMessenger::Execute("AddSphereCollision", &data);
}


// --------------------------------
// �X�V����
// --------------------------------
void Goblin::Update(const float elapsedTime)
{
	// ���[���h�s��̏�����
	CalcWorldMatrix();
	// �G�t�F�N�g�̍X�V
	m_enemyEffect->Update(elapsedTime);
	// �Փ˔���̈ړ�
	MoveCollision();
	// �N�[���^�C���̃J�E���g
	CountCoolTime(elapsedTime);
	// �����m�F
	CheckAlive();

	// �X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}


// --------------------------------
// ���[���h�s��̌v�Z
// --------------------------------
void Goblin::CalcWorldMatrix()
{
	m_worldMatrix =
		DirectX::SimpleMath::Matrix::CreateRotationY(m_angle) *				// ��]
		DirectX::SimpleMath::Matrix::CreateScale(GOBLIN_SCALE * m_scale) *	// �傫��
		DirectX::SimpleMath::Matrix::CreateTranslation(m_position);			// ���W
}


// --------------------------------
// �Փ˔���̈ړ�
// --------------------------------
void Goblin::MoveCollision()
{
	// �Փ˔���̍��W�𓮂������W�ɍ��킹��
	DirectX::SimpleMath::Vector3 pos = m_position;
	pos.y = COLLISION_POS_Y;
	m_bodyCollision->Center = pos;
}


// --------------------------------
// �`��֐�
// --------------------------------
void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_enemyEffect->DrawWithEffect(m_model, m_worldMatrix, view, projection);
}


// --------------------------------
// �I���֐�
// --------------------------------
void Goblin::Finalize()
{
	m_idling->Finalize();
	m_attacking->Finalize();
	m_dead->Finalize();

	// �폜�p�Փ˔���̃f�[�^���쐬
	DeleteCollisionData data = { CollisionType::Sphere, this };

	// �폜�C�x���g�����s
	EventMessenger::Execute("DeleteCollision", &data);
}


// --------------------------------
// ���������Ƃ��̏���
// --------------------------------
void Goblin::HitAction(InterSectData data)
{
	switch (data.objType)
	{
	case ObjectType::Player:	HitPlayer(data);	break;
	case ObjectType::Goblin:	HitGoblin(data);	break;
	case ObjectType::Boss:		HitBoss(data);		break;
	case ObjectType::Stage:		HitStage(data);		break;
	case ObjectType::Sword:		HitSword(data);		break;
	}
}

// --------------------------------
// �v���C���[�ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitPlayer(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}


// --------------------------------
// ���S�ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitGoblin(InterSectData data)
{
	// �Փ˂����I�u�W�F�N�g�̏����擾
	auto goblinCollision = *m_bodyCollision.get();
	auto goblinCollision2 = *data.collision;

	// �����߂��ʂ��v�Z
	m_pushBackValue = Math::pushBack_BoundingSphere(goblinCollision, goblinCollision2);
	m_pushBackValue.y = 0.0f;
	// �v���C���[�̈ʒu�������߂�
	m_position += m_pushBackValue;
	m_bodyCollision->Center = m_position;
}


// --------------------------------
// �G�ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitBoss(InterSectData data)
{
	m_pushBackValue = DirectX::SimpleMath::Vector3::Zero;

	// �Փ˂����I�u�W�F�N�g�̏����擾
	auto goblinCollision = *m_bodyCollision.get();
	auto enemyCollision = *data.collision;

	// �����߂��ʂ��v�Z
	m_pushBackValue += Math::pushBack_BoundingSphere(goblinCollision, enemyCollision);
	m_pushBackValue.y = 0.0f;
	// �v���C���[�̈ʒu�������߂�
	m_position += m_pushBackValue;
	m_bodyCollision->Center = m_position;
}


// --------------------------------
// �X�e�[�W�ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitStage(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}


// --------------------------------
// ���ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitSword(InterSectData data)
{
	// �Փˉ\�łȂ��ꍇ�͏������s��Ȃ�
	if (!m_canHit) return;
	m_canHit = false;

	UNREFERENCED_PARAMETER(data);
	Damaged(1);
}


// --------------------------------
// �X�e�[�g�̕ύX
// --------------------------------
void Goblin::ChangeState(IState* state)
{
	// ���łɓ����X�e�[�g�̏ꍇ�͕ύX���Ȃ�
	if (m_currentState == state) return;
	// �ύX�O�X�e�[�g�̍ŏI����
	m_currentState->PostUpdate();
	// �X�e�[�g�̕ύX
	m_currentState = state;
	// �ύX��X�e�[�g�̏�������
	m_currentState->PreUpdate();
}


// --------------------------------
// �_���[�W���󂯂��Ƃ��̏���
// --------------------------------
void Goblin::Damaged(float damage)
{
	// ���łɃ_���[�W���󂯂Ă���ꍇ�͏������s��Ȃ�
	if (m_isHit)return;
	// HP�����炷
	m_hp->Damage(damage);
	// �q�b�g�X�g�b�v��L���ɂ���
	HitStop::GetInstance()->SetActive();
	// �G�t�F�N�g���Đ�
	m_enemyEffect->SetEffect(EnemyEffect::ENEMY_EFFECT::DAMAGE);

	// ��ʂ�h�炷
	float shakePower = 0.25f;
	EventMessenger::Execute("CameraShake", &shakePower);

	// �N�[���^�C����L���ɂ���
	m_isHit = true;
}


// --------------------------------
// �N�[���^�C���̃J�E���g
// --------------------------------
void Goblin::CountCoolTime(float elapsedTime)
{
	// �ߋ��ɍU�����󂯂Ă��Ȃ��ꍇ�͏������s��Ȃ�
	if (!m_isHit)return;

	// �N�[���^�C�����J�E���g
	m_coolTime += elapsedTime;

	if (m_coolTime > COOL_TIME)
	{
		m_isHit = false;
		m_coolTime = 0.0f;
	}
}


// --------------------------------
// �����m�F
// --------------------------------
void Goblin::CheckAlive()
{
	// ���S���Ă���ꍇ�͏������s��Ȃ�
	if (m_currentState == m_dead.get()) return;

	if (m_hp->GetHP() <= 0)
	{
		// �X�e�[�g��ύX
		ChangeState(m_dead.get());
		// ���S�G�t�F�N�g���Đ�
		m_enemyEffect->SetEffect(EnemyEffect::ENEMY_EFFECT::DEAD);
	}
}


// --------------------------------
// �S�u����������
// --------------------------------
void Goblin::DeleteGoblin()
{
	m_playScene->GetEnemyManager()->DeleteEnemy(this);
}
