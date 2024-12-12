// -------------------------------------------------------
// ���S�̐e�N���X
// -------------------------------------------------------

#include "pch.h"
#include "Goblin.h"
#include "Game/Player/Player.h"
#include "Game/Scene/PlayScene.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
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
	, m_position{ 0.f, 0.f, 0.f }
	, m_velocity{ 0.f, 0.f, 0.f }
	, m_angle{ 0.f, 0.f, 0.f }
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
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
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Goblin/goblin.cmo", *fx);

	// �X�e�[�g�̍쐬
	CreateState();

	// HP�̐���
	m_hp = std::make_unique<HPSystem>(GOBLIN_HP);
}


// �X�e�[�g�̍쐬
void Goblin::CreateState()
{
	m_idling		=	std::make_unique<GoblinIdling>		(this);	// �ҋ@
	m_attacking		=	std::make_unique<GoblinAttacking>	(this);	// �U��

	m_idling	->	Initialize(m_model.get());
	m_attacking	->	Initialize(m_model.get());

	m_currentState = m_idling.get();
}


// �����蔻��̐���
void Goblin::CreateCollision()
{
	// �����蔻��̐���
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE);

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
	using namespace DirectX::SimpleMath;

	// ���[���h�s��̏�����
	m_worldMatrix = Matrix::CreateScale(GOBLIN_SCALE);

	// �X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}


// �`��֐�
void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	m_currentState->Render(context, states, view, projection);			// �X�e�[�g���̕`��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);	// ���f���̕`��
}


// �I���֐�
void Goblin::Finalize()
{
	m_idling	->	Finalize();
	m_attacking	->	Finalize();
}


// ���������Ƃ��̏���
void Goblin::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
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
