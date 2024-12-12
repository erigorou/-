// ----------------
//
// �S�̋���
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Scene/PlayScene.h"
#include "../HitStop/HitStop.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Stage/Wall/Wall.h"

#include "Interface/IState.h"
#include "BehaviourTree/Header/BehaviorTree.h"	// �r�w�C�r�A�c���[

// �X�e�[�g�p�^�[���p
#include "States/Header/EnemyIdling.h"			// �ҋ@���
#include "States/Header/Enemy_Attacking.h"		// ���������U��
#include "States/Header/Enemy_Sweeping.h"		// �ガ�����U��
#include "States/Header/EnemyDashAttacking.h"	// �ˌ��U��
#include "States/Header/EnemyApproaching.h"		// �ǔ����

// ��̃p�[�c�p
#include "Face/Header/EnemyFaceIdling.h"
#include "Face/Header/EnemyFaceAttacking.h"

// �_���[�W�G�t�F�N�g
#include "Effects/EnemyDamageEffect/EnemyDamageEffect.h"



// �Œ�l
const float Enemy::ENEMY_SPEED	= 0.1f;
const float Enemy::ENEMY_SCALE	= 0.6f;
const float Enemy::COOL_TIME	= 0.3f;

// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Enemy::Enemy(PlayScene* playScene)
	: m_playScene(playScene)
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_currentState()
	, m_idling()
	, m_attacking()
	, m_approaching()
	, m_coolTime()
	, m_position{ 0.0f, 0.0f, 0.0f }
	, m_angle{ 0.0f }
	, m_bodyTilt{ 0.0f }
	, m_pushBackValue{ 0.0f, 0.0f, 0.0f }
	, m_isHit(false)
	, m_canHit(false)
	, m_isTargetLockOn(true)
{
}

// --------------------------------
//  �f�X�g���N�^
// --------------------------------
Enemy::~Enemy()
{
}

// --------------------------------
//  �C�j�V�����C�Y
 // --------------------------------
void Enemy::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	UNREFERENCED_PARAMETER(states);

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Body/oni.cmo", *fx);

	// HP��ݒ�
	m_hp = std::make_unique<HPSystem>(HP);

	// �r�w�C�r�A�c���[���擾
	m_pBT = std::make_unique<BehaviorTree>();

	// �X�e�[�g�̍쐬
	CreateState();

	// ��p�[�c�̐���
	CreateFace();

	// �_���[�W�G�t�F�N�g�̐���
	m_damageEffect = std::make_unique<EnemyDamageEffect>();

	// �����蔻��̍쐬
	CreateCollision();

	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);
	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
}



// --------------------------------
//  ��Ԃ̐�������
// --------------------------------
void Enemy::CreateState()
{
	// === ��Ԃ̐��� ====
	m_idling		= std::make_unique<EnemyIdling>			(this);	// �ҋ@
	m_attacking		= std::make_unique<Enemy_Attacking>		(this);	// �U��
	m_sweeping		= std::make_unique<Enemy_Sweeping>		(this);	// �ガ����
	m_dashAttacking = std::make_unique<EnemyDashAttacking>	(this);	// �ˌ�
	m_approaching	= std::make_unique<EnemyApproaching>	(this);	// �ǔ�

	// === ��Ԃ̏����� ===
	m_idling		-> Initialize(m_model.get());	// �ҋ@
	m_attacking		-> Initialize(m_model.get());	// �U��
	m_sweeping		-> Initialize(m_model.get());	// �ガ����
	m_dashAttacking	-> Initialize(m_model.get());	// �ˌ�
	m_approaching	-> Initialize(m_model.get());	// �ǔ�

	// �����̃X�e�[�g��ҋ@��ԂɊ��蓖�Ă�
	m_currentState = m_idling.get();
}


// --------------------------------
//  ��p�[�c�̐�������
// --------------------------------
void Enemy::CreateFace()
{
	// ��p�[�c�̐���
	m_faceIdling	= std::make_unique<EnemyFaceIdling>		(this);
	m_faceAttacking = std::make_unique<EnemyFaceAttacking>	(this);


	// �����̊��ҋ@��Ɋ��蓖�Ă�
	m_currentFace = m_faceIdling.get();
}


// --------------------------------
//  �Փ˔���̐�������
// --------------------------------
void Enemy::CreateCollision()
{
	// �����蔻��̐���
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, ENEMY_SCALE * 16.0f);

	// �Փ˔����Messenger�ɓo�^
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Enemy,
		this,
		m_bodyCollision.get());
}


// --------------------------------
//  ��Ԃ̐�������
// --------------------------------
void Enemy::ChangeState(IState* newState)
{
	// ����Ȃ��X�e�[�g���X�V���悤�Ƃ�����߂�
	if (m_currentState == newState) return;
	// �V�K�̏�ԑJ�ڑO�Ɏ���X�V���s��
	m_currentState->PostUpdate();
	// �V�K�̏�Ԃ����݂̏�Ԃɐݒ肷��
	m_currentState = newState;
	// �V�K�̏�ԑJ�ڌ�Ɏ��O�X�V���s��
	m_currentState->PreUpdate();
}


// --------------------------------
//  �X�V����
// --------------------------------
void Enemy::Update(float elapsedTime)
{
	// �X�e�[�^�X�̍X�V����
	m_currentState->Update(elapsedTime);
	// �_���[�W���̍X�V����
	m_damageEffect->Update(elapsedTime);
	// ���[���h�s��̌v�Z
	CalcrationWorldMatrix();
	// �����蔻��̍X�V
	m_bodyCollision->Center = DirectX::SimpleMath::Vector3(m_position.x, m_position.y + COLISION_POS_Y, m_position.z);
	// �Փ˂̃N�[���^�C���̌v��
	CheckHitCoolTime(elapsedTime);



#ifdef _DEBUG

	// �L�[���͂��󂯕t����B
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1)	ChangeState(m_attacking.get());
	if (keyboardState.F2)	ChangeState(m_sweeping.get());
	if (keyboardState.F3)	ChangeState(m_dashAttacking.get());

#endif // _DEBUG
}

void Enemy::CalcrationWorldMatrix()
{
	// ��]�����̐ݒ�
	m_worldMatrix
		= DirectX::SimpleMath::Matrix::CreateRotationX(m_bodyTilt)	// ��]�̐ݒ�
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));

	// �ړ����s��
	m_velocity *= Enemy::ENEMY_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ���[���h�s��̌v�Z
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(ENEMY_SCALE)			// �T�C�Y�v�Z
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// �ʒu�̐ݒ�
}



// --------------------------------
//  �Փ˂̃N�[���^�C���̌v�����s��
// --------------------------------
void Enemy::CheckHitCoolTime(float elapsedTime)
{	
	// �N�[���^�C���̌v�����s��
	if (m_isHit && m_coolTime < COOL_TIME)
	{
		m_coolTime += elapsedTime;
	}

	// �N�[���^�C�����I��������
	else if (m_coolTime >= COOL_TIME)
	{
		m_isHit = false;
		m_coolTime = 0.0f;
	}
}



// --------------------------------
//  �\������
// --------------------------------
void Enemy::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();


	// �[�x�l���Q�Ƃ��ď�������
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	m_currentState->Render(context,states,view,projection);				// �X�e�[�g���̕`��
	m_currentFace->DrawFace(m_worldMatrix, view, projection);			// ��̕`��

	m_damageEffect->DrawWithDamageEffect(m_model.get(), m_worldMatrix, view, projection);	// �_���[�W�G�t�F�N�g�̕`��

	//m_model->Draw(context, *states, m_worldMatrix, view, projection);	// ���f���̕`��

#ifdef _DEBUG
#endif // _DEBUG
}


// --------------------------------
//  �I������
// --------------------------------
void Enemy::Finalize()
{
	GetPlayScene()->GetCollisionManager()->DeleteCollision(CollisionType::Sphere, this);
}


// --------------------------------
//  �S�̂̏Փ˔���C�x���g
// --------------------------------
void Enemy::HitAction(InterSectData data)
{
	HitSword(data);
	HitStage(data);
}



// --------------------------------
//  ���Ƃ̏Փ˔���
// --------------------------------
void Enemy::HitSword(InterSectData data)
{
	if (
		!m_isHit &&
		m_canHit &&
		data.objType == ObjectType::Sword &&
		data.colType == CollisionType::OBB)
	{
		m_hp->Damage(1);
		m_isHit = true;
		m_canHit = false;

		// �q�b�g�X�g�b�v��L���ɂ���
		HitStop::GetInstance()->SetActive();
		// �̂̃G�t�F�N�g���Đ�
		m_damageEffect->IsDamaged();
	}
}

// --------------------------------
//  �X�e�[�W�Ƃ̏Փ˔���
// --------------------------------
void Enemy::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		m_pushBackValue = DirectX::SimpleMath::Vector3::Zero;

		// �Փ˂����I�u�W�F�N�g�̏����擾
		auto wall = dynamic_cast<Wall*>(data.object);
		DirectX::BoundingSphere* stageCollision = wall->GetCollision();

		// �����߂��ʂ��v�Z
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), *stageCollision);
		// y���W�ɂ͔��f�����ɐݒ�
		m_pushBackValue.y = 0;
		// �G�̈ʒu�������߂�
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}