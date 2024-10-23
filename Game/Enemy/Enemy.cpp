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

#include "Game/Enemy/Enemy.h"
#include "Game/Stage/Wall/Wall.h"

#include "Interface/IState.h"

// �Œ�l
const float Enemy::ENEMY_SPEED	= 0.1f;
const float Enemy::ENEMY_SCALE	= 0.4f;
const float Enemy::COOL_TIME	= 0.4f;

// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Enemy::Enemy(PlayScene* playScene)
	: m_playScene(playScene)
	, m_currentState()
	, m_idling()
	, m_attacking()
	, m_approaching()
	, m_position{0.f, 0.f, 0.f}
	, m_angle{0.f}
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_isHit(false)
	, m_coolTime()
	, m_canHit(false)
	, m_pushBackValue{ 0.f, 0.f, 0.f }
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
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/oni.cmo", *fx);

	// HP��ݒ�
	m_hp = std::make_unique<EnemyHP>();

	// �r�w�C�r�A�c���[���擾
	m_pBT = std::make_unique<BehaviorTree>();

	// �X�e�[�g�̍쐬
	CreateState();

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
//  �X�V����
// --------------------------------
void Enemy::CreateState()
{
	// === ��Ԃ̐��� ====
	m_idling = std::make_unique<EnemyIdling>(this);				// �ҋ@
	m_attacking = std::make_unique<Enemy_Attacking>(this);		// �U��
	m_sweeping = std::make_unique<Enemy_Sweeping>(this);		// �ガ����
	m_approaching = std::make_unique<EnemyApproaching>(this);	// �ǔ�

	// === ��Ԃ̏����� ===
	m_idling->Initialize(m_model.get());		// �ҋ@
	m_attacking->Initialize(m_model.get());		// �U��
	m_sweeping->Initialize(m_model.get());		// �ガ����
	m_approaching->Initialize(m_model.get());	// �ǔ�

	// �����̃X�e�[�g��ҋ@��ԂɊ��蓖�Ă�
	m_currentState = m_idling.get();
}


void Enemy::CreateCollision()
{
	// �����蔻��̐���
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, ENEMY_SCALE * 12.0f);

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
	using namespace DirectX::SimpleMath;

	m_worldMatrix = Matrix::Identity;

	// �X�e�[�^�X���X�V���܁[��
	m_currentState->Update(elapsedTime);

	// �L�[���͂��󂯕t����B
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.Enter)
	{
		// �r�w�C�r�A�c���[�����s�B
		m_pBT->run();
	}

	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));	// ��]�p�̐ݒ�

	m_velocity *= Enemy::ENEMY_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ���[���h�s��̌v�Z@;;;;;;;;;;;;;;;;
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(ENEMY_SCALE)			// �T�C�Y�v�Z
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// �ʒu�̐ݒ�

	// �����蔻��̍X�V
	m_bodyCollision->Center = m_position;

	//////////////////////////////////�N�[���^�C���̌v�����s��//////////////////////////////////
	if (m_isHit && m_coolTime < COOL_TIME) {	m_coolTime += elapsedTime;				}
	else if (m_coolTime >= COOL_TIME	 ) {	m_isHit = false;	m_coolTime = 0.0f;	}
}


// --------------------------------
//  �\������
// --------------------------------
void Enemy::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{


	// �[�x�l���Q�Ƃ��ď�������
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	m_currentState->Render(context,states,view,projection);				// �X�e�[�g���̕`��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);	// ���f���̕`��

#ifdef _DEBUG
	DrawBoundingSphere(device, context, states, view, projection, m_bodyCollision.get());	// �����蔻��̕`��

	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("EnemyPos : %f, %f, %f", m_position.x, m_position.y, m_position.z);

	if (m_isHit)
	{
		debugString->AddString("Hit");
	}

#endif // _DEBUG
}


// --------------------------------
// ���E����\��
// --------------------------------
void Enemy::DrawBoundingSphere(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const DirectX::BoundingSphere* boundingSphere
	)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(device);

	// �`��ɂ�����ݒ荀��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);

	// �`��
	m_primitiveBatch->Begin();
	DX::Draw(
		m_primitiveBatch.get(),	// �v���~�e�B�u�o�b�`
		*boundingSphere,		// �`�悵�������E��
		Colors::DarkRed			// �F
	);
	m_primitiveBatch->End();
}


// --------------------------------
//  �I������
// --------------------------------
void Enemy::Finalize()
{
}


// --------------------------------
//  ���������Ƃ��̏���
// --------------------------------
void Enemy::HitAction(InterSectData data)
{
	HitSword(data);
	HitStage(data);
}



/// <summary>
/// ���Ƃ̏Փ˔���
/// </summary>
/// <param name="data">�Փ˂�������I�u�W�F�N�g�̏������\����</param>
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
	}
}

/// <summary>
/// �X�e�[�W�Ƃ̏Փ˔���
/// </summary>
/// <param name="data">�Փ˂�������I�u�W�F�N�g�̏������\����</param>
void Enemy::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
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