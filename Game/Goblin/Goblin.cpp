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
#include "../Enemy/Enemy.h"
#include "Effects/EnemyDamageEffect/EnemyDamageEffect.h"
#include "Game/HitStop/HitStop.h"

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"


// ---------------
// �Œ�l
// ---------------
const float Goblin::GOBLIN_SPEED = Enemy::ENEMY_SPEED / 2.0f;	// �ړ���
const float Goblin::GOBLIN_SCALE = Enemy::ENEMY_SCALE / 4.0f;	// �T�C�Y


// -------------------------------
// �R���X�g���N�^
// -------------------------------
Goblin::Goblin(PlayScene* playScene)
	: m_playScene(playScene)
	, m_position{ 80.0f, 0.0f, 0.0f } // ���@���u��
	, m_velocity{}
	, m_angle{}
	, m_scale{1.0f, 1.0f, 1.0f}
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
	, m_nowAttacking(false)
	, m_pushBackValue{}
	, m_isHit(false)
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
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Goblin/goblin.cmo", *fx);

	// �X�e�[�g�̍쐬
	CreateState();
	// �����蔻��̐���
	CreateCollision();
	// HP�̐���
	m_hp = std::make_unique<HPSystem>(GOBLIN_HP);
	// �_���[�W�G�t�F�N�g�̐���
	m_damageEffect = std::make_unique<EnemyDamageEffect>();

}

// --------------------------------
// �X�e�[�g�̍쐬
// --------------------------------
void Goblin::CreateState()
{
	m_idling = std::make_unique<GoblinIdling>(this);		// �ҋ@
	m_attacking = std::make_unique<GoblinAttacking>(this);	// �U��

	m_idling->Initialize();
	m_attacking->Initialize();

	m_currentState = m_idling.get();
}


// --------------------------------
// �����蔻��̐���
// --------------------------------
void Goblin::CreateCollision()
{
	// �����蔻��̐���
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE * COLLISION_RADIUS);

	// �����蔻���CollisionManager�ɓo�^����
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Goblin,
		CollisionType::Sphere,
		this,
		m_bodyCollision.get()
	);
}


// --------------------------------
// �X�V����
// --------------------------------
void Goblin::Update(const float elapsedTime)
{
	// ���[���h�s��̏�����
	CalcWorldMatrix();
	// �X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
	// �_���[�W�G�t�F�N�g�̍X�V
	m_damageEffect->Update(elapsedTime);
	// �Փ˔���̈ړ�
	MoveCollision();
	// �N�[���^�C���̃J�E���g
	CountCoolTime(elapsedTime);
}


// --------------------------------
// ���[���h�s��̌v�Z
// --------------------------------
void Goblin::CalcWorldMatrix()
{
	m_worldMatrix =
		DirectX::SimpleMath::Matrix::CreateRotationY(m_angle) *
		DirectX::SimpleMath::Matrix::CreateScale(GOBLIN_SCALE * m_scale) *
		DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}


// --------------------------------
// �Փ˔���̈ړ�
// --------------------------------
void Goblin::MoveCollision()
{
	DirectX::SimpleMath::Vector3 pos = m_position;
	pos.y = COLLISION_POS_Y;
	m_bodyCollision->Center = pos;
}


// --------------------------------
// �`��֐�
// --------------------------------
void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// �`��ɕK�v�ȃf�[�^���擾����
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// �_���[�W�G�t�F�N�g�t���Ń��f����`�悷��
	m_damageEffect->DrawWithDamageEffect(m_model.get(), m_worldMatrix, view, projection);
}


// --------------------------------
// �I���֐�
// --------------------------------
void Goblin::Finalize()
{
	m_idling->Finalize();
	m_attacking->Finalize();

	// �����蔻��̍폜
	m_playScene->GetCollisionManager()->DeleteCollision(CollisionType::Sphere, this);
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
	case ObjectType::Enemy:		HitEnemy(data);		break;
	case ObjectType::Stage:		HitStage(data);		break;
	case ObjectType::Sword:		HitSword(data);		break;
	}
}

// --------------------------------
// �v���C���[�ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitPlayer(InterSectData data)
{
}


// --------------------------------
// ���S�ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitGoblin(InterSectData data)
{
	auto goblin = static_cast<Goblin*>(data.object);

	// �Փ˂����I�u�W�F�N�g�̏����擾
	auto goblinCollision = *m_bodyCollision.get();
	auto goblinCollision2 = goblin->GetCollision();

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
void Goblin::HitEnemy(InterSectData data)
{
	auto enemy = static_cast<Enemy*>(data.object);

	// �Փ˂����I�u�W�F�N�g�̏����擾
	auto goblinCollision = *m_bodyCollision.get();
	auto enemyCollision = enemy->GetBodyCollision();

	// �����߂��ʂ��v�Z
	m_pushBackValue = Math::pushBack_BoundingSphere(goblinCollision, enemyCollision);
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
}


// --------------------------------
// ���ɓ��������Ƃ��̏���
// --------------------------------
void Goblin::HitSword(InterSectData data)
{
	auto sword = static_cast<Sword*>(data.object);
	// �����ҋ@���łȂ��ꍇ
	if (sword->GetAttackFlag())
	{
		// �_���[�W���󂯂�
		Damaged(1);
	}
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
	// �_���[�W�G�t�F�N�g���Đ�
	m_damageEffect->IsDamaged();
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
