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

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"


// ---------------
// �Œ�l
// ---------------
const float Goblin::GOBLIN_SPEED = Enemy::ENEMY_SPEED / 2.0f;	// �ړ���
const float Goblin::GOBLIN_SCALE = Enemy::ENEMY_SCALE / 4.0f;	// �T�C�Y


// �R���X�g���N�^
Goblin::Goblin(PlayScene* playScene)
	: m_playScene(playScene)
	, m_position{ 80.f, 0.f, 0.f }
	, m_velocity{ 0.f, 0.f, 0.f }
	, m_angle{ 0.f, 0.f, 0.f }
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
	, m_nowAttacking(false)
{
}


// �f�X�g���N�^
Goblin::~Goblin()
{
}


// �������֐�
void Goblin::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Body/oni.cmo", *fx);

	// �X�e�[�g�̍쐬
	CreateState();
	// �����蔻��̐���
	CreateCollision();
	// HP�̐���
	m_hp = std::make_unique<HPSystem>(GOBLIN_HP);
}


// �X�e�[�g�̍쐬
void Goblin::CreateState()
{
	m_idling		=	std::make_unique<GoblinIdling>		(this);	// �ҋ@
	m_attacking		=	std::make_unique<GoblinAttacking>	(this);	// �U��

	m_idling	->	Initialize();
	m_attacking	->	Initialize();

	m_currentState = m_idling.get();
}

// �v���C���[�ɓ��������Ƃ��̏���
void Goblin::HitPlayer(InterSectData data)
{
	if (data.objType == ObjectType::Player && data.colType == CollisionType::Sphere)
	{
	}
}


// ���S�ɓ��������Ƃ��̏���
void Goblin::HitGoblin(InterSectData data)
{
	if (data.objType == ObjectType::Goblin && data.colType == CollisionType::Sphere)
	{
	}
}

void Goblin::HitEnemy(InterSectData data)
{
}

void Goblin::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		// ���Z�b�g
		m_oushBackValue = DirectX::SimpleMath::Vector3::Zero;
	}
}


// �����蔻��̐���
void Goblin::CreateCollision()
{
	// �����蔻��̐���
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE * COLLISION_RADIUS);

	// �����蔻���CollisionManager�ɓo�^����
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Goblin,
		CollisionType::Sphere,
		this,
		m_bodyCollision.get());
}


// �X�V����
void Goblin::Update(const float elapsedTime)
{
	// ���[���h�s��̏�����
	m_worldMatrix = 
		DirectX::SimpleMath::Matrix::CreateScale(GOBLIN_SCALE) * DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	// �X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
	// �Փ˔���̈ړ�
	MoveCollision();
}


// �Փ˔���̈ړ�
void Goblin::MoveCollision()
{
	DirectX::SimpleMath::Vector3 pos = m_position;
	pos.y = 2.0f;
	m_bodyCollision->Center = pos;
}


// �`��֐�
void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// �`��ɕK�v�ȃf�[�^���擾����
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���f���̕`��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// �I���֐�
void Goblin::Finalize()
{
	m_idling	->	Finalize();
	m_attacking	->	Finalize();

	// �����蔻��̍폜
	m_playScene->GetCollisionManager()->DeleteCollision(CollisionType::Sphere, this);
}


// ���������Ƃ��̏���
void Goblin::HitAction(InterSectData data)
{
	switch (data.objType)
	{
	case ObjectType::Player:	HitPlayer(data);	break;
	case ObjectType::Goblin:	HitGoblin(data);	break;
	case ObjectType::Enemy:		HitEnemy(data);		break;
	case ObjectType::Stage:		HitStage(data);		break;
	}
}


// �X�e�[�g�̕ύX
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


// �_���[�W���󂯂��Ƃ��̏���
void Goblin::Damaged(float damage)
{
	m_hp->Damage(damage);
}