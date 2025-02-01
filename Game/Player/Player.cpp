// ----------------
//
// �����Y�̋���
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
#include "Libraries/MyLib/KeyboardChacker.h"
#include "Game/GameResources.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"
#include "Game/Factory/Factory.h"
#include "../Data/HPSystem.h"
#include "Game/Boss/Boss.h"
#include "Game/Goblin/Goblin.h"
#include "Game/Stage/Wall/Wall.h"


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Player::Player(PlayScene* playScene)
	:
	m_playScene(playScene),
	m_elapsedTime{},
	m_tilt{},
	m_model{},
	m_position{ HOME_POSITION },
	m_angle{},
	m_inputVector{},
	m_acceleration{},
	m_pushBackValue{},
	m_worldMatrix{},
	m_currentState{},
	m_particleTime{},
	m_isHit{},
	m_coolTime{},
	m_canHit{false},
	m_animationRotate{},
	m_isInputMoveKey{ false }
{
}


// --------------------------------
//  �f�X�g���N�^
// --------------------------------
Player::~Player()
{
}



// --------------------------------
//  �C�j�V�����C�Y
// --------------------------------
void Player::Initialize()
{
	// �`��֘A�̏�����
	InitializeRender();
	// �����蔻��̍쐬
	CreateCollision();
	// �X�e�[�g�̍쐬
	CreateState();
	// HP���Ǘ�����N���X�̐���
	m_hp = std::make_unique<HPSystem>(PLAYER_HP);
	// �C�x���g��o�^
	AttachEvent();
	// ����̐���
	m_sword = Factory::CreateSword(this);
}


// --------------------------------
// �����蔻��̍쐬�֐�
// --------------------------------
void Player::CreateCollision()
{
	// �̂̓����蔻����쐬
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, PLAYER_SCALE * COLLISION_RADIUS);
	
	// �Փ˃f�[�^�̍쐬
	CollisionData<DirectX::BoundingSphere> data =
	{
		ObjectType::Player,		// �I�u�W�F�N�g�̎��
		CollisionType::Sphere,	// �����蔻��`��̎��
		this,					// �I�u�W�F�N�g�̃A�h���X
		m_bodyCollision.get()	// �����蔻��̃A�h���X
	};

	// �����蔻����L�^����
	EventMessenger::Execute("AddSphereCollision", &data);
}


// --------------------------------
//  �X�e�[�g���쐬�֐�
// --------------------------------
void Player::CreateState()
{
	//////////////////////�X�e�[�g�̍쐬////////////////////////////
	m_playerIdling		= std::make_unique<PlayerIdling>		(this);
	m_playerDodging		= std::make_unique<PlayerDodging>		(this);
	m_playerAttacking_1 = std::make_unique<PlayerAttacking_1>	(this);
	m_playerAttacking_2 = std::make_unique<PlayerAttacking_2>	(this);
	m_playerNockBacking = std::make_unique<PlayerNockBacking>	(this);

	//////////////////////�X�e�[�g�̏�����////////////////////////////
	m_playerIdling		->Initialize();
	m_playerDodging		->Initialize();
	m_playerAttacking_1	->Initialize();
	m_playerAttacking_2	->Initialize();
	m_playerNockBacking	->Initialize();

	// �ŏ��̃X�e�[�g��ݒ�
	m_currentState = m_playerIdling.get();
}


// --------------------------------
//  �����_�����O�̏�����
// --------------------------------
void Player::InitializeRender()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// ���f���̎擾
	m_model = GameResources::GetInstance()->GetModel("player");
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
// �C�x���g�̓o�^
// --------------------------------
void Player::AttachEvent()
{
	// IObject* �^�̃I�u�W�F�N�g���擾����
	EventMessenger::AttachGetter("GetPlayerObject", std::bind(&Player::GetObject, this));
	// �Փˉ\����ݒ�
	EventMessenger::Attach("PlayerCanHit", std::bind(&Player::CanHit, this, std::placeholders::_1));
}


// -----------------------------------------------------
// �X�e�[�g��ύX����
// -----------------------------------------------------
void Player::ChangeState(IPlayer* newState)
{
	// �����X�e�[�g�ōX�V���悤�Ƃ���Ƒ������^�[��
	if (m_currentState == newState) return;

	m_currentState->PostUpdate();	// ����X�V�������s��
	m_currentState = newState;		// ���݂̃X�e�[�g��ύX����
	m_currentState->PreUpdate();	// �V�����X�e�[�g�̎��O�X�V���s��
}


// ---------------------------------------------------------
// �s���ɑ΂��鎞�Ԃ��v�Z����֐�
// ---------------------------------------------------------
void Player::TimeComparison(float& nowTime, const float totalTime, IPlayer* newState, const float elapsedTime)
{
	// ��߂�ꂽ���ԂɂȂ�����
	if (nowTime >= totalTime)
	{
		// �V�[����ύX
		ChangeState(newState);
		return;
	}
	// ���Ԃ̌v�����s��
	nowTime += elapsedTime;
}


// ----------------------------------------------
// �v���C���[�̍X�V����
// ---------------------------------------------
void Player::Update(const float elapsedTime)
{
	m_elapsedTime = elapsedTime;	// �o�ߎ��Ԃ�ۑ�����

	// �X�e�[�g�̍X�V
	m_currentState->Update(elapsedTime);

	// �v���C���[�̈ړ�

	CalculationMatrix();
	m_pushBackValue = Vector3::Zero;

	// �����蔻��̍X�V
	m_bodyCollision->Center = m_position;
	m_bodyCollision->Center.y = 0;

	// ����̍X�V����
	m_sword->Update(elapsedTime);

	// �N�[���^�C�����v����
	if (m_isHit && m_coolTime < COOL_TIME)	{ m_coolTime += elapsedTime;		 }
	// �N�[���^�C���I���
	else if (m_coolTime >= COOL_TIME)		{ m_isHit = false;m_coolTime = 0.0f; }
}


// ----------------------------------------------
// �L�[���͂��擾����
// ----------------------------------------------
void Player::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	m_currentState->OnKeyPressed(key);
}


// ----------------------------------------------
// �L�[���͂��擾����
// ----------------------------------------------
void Player::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	m_velocity = DirectX::SimpleMath::Vector3::Zero;

	if (KeyboardChacker::IsInputArrowKey(key))
	{
		if (!m_isInputMoveKey)
		{
			// �ړ��L�[�̓��͂�����ꍇ
			m_isInputMoveKey = true;
			// ���Z�b�g
			m_inputVector = DirectX::SimpleMath::Vector2::Zero;
		}

		// �ړ��L�[�ɉ����ē��̓x�N�g����ݒ肷��
		if (key == DirectX::Keyboard::Up)		m_inputVector += INPUT_FLONT;
		if (key == DirectX::Keyboard::Down)		m_inputVector += INPUT_BACK;
		if (key == DirectX::Keyboard::Left)		m_inputVector += INPUT_LEFT;
		if (key == DirectX::Keyboard::Right)	m_inputVector += INPUT_RIGHT;
	}

	m_currentState->OnKeyDown(key);
}


// ----------------------------------------------
// ��]�p�̌v�Z�֐�
//�@---------------------------------------------
float Player::CalucratePlayerRotation(DirectX::SimpleMath::Vector3 const enemyPos)
{
	// ���͂��Ȃ��ꍇ��0��Ԃ�
	if (m_inputVector.LengthSquared() < FLT_EPSILON) 
	{
		return 0.0f;
	}

	// �G�̕������擾�i���W�A���P�ʂŌv�Z�j
	float lookEnemy = Math::CalculationAngle(m_position, enemyPos);

	// ���̓x�N�g���𐳋K��
	Vector2 normalizedInput = m_inputVector;
	normalizedInput.Normalize();

	// atan2�œ��̓x�N�g���̊p�x���v�Z�i���W�A���P�ʁj
	float inputAngle = std::atan2(normalizedInput.x, normalizedInput.y);

	// �G�̕����Ɠ��͕����̍����p�x���v�Z
	float resultAngle = lookEnemy + inputAngle;

	// �K�v�Ȃ�p�x��0�`2�΂͈̔͂ɐ��K��
	while (resultAngle < 0) 
	{
		resultAngle += DirectX::XM_2PI; // 2�΂������Đ��͈̔͂�
	}
	
	while (resultAngle > DirectX::XM_2PI) 
	{
		resultAngle -= DirectX::XM_2PI; // 2�΂������Ĕ͈͓���
	}

	return resultAngle; // �v�Z���ʁi���W�A���P�ʁj��Ԃ�
}




// --------------------------------
//  �ړ��̊Ǘ�
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	Vector3 moveVelocity = Vector3::Zero; // �����p���x

	///////////////////// �ړ��L�[�̓��͂��Ȃ��ꍇ�̏��� /////////////////
	if (m_isInputMoveKey == false)
	{
		float accelerationLength = m_acceleration.Length(); // ���x�̒������擾����
		// 0�̋ߎ��l���傫���ꍇ
		if (accelerationLength >= FLT_EPSILON)
		{
			Vector3 accelerationNormal = m_acceleration / accelerationLength; // �ێ���������x�̐��K���x�N�g�����擾����

			float friction = 0.05f; // ���C��
			accelerationLength -= friction;	// ���C���v�Z

			// �����x���i�[�j�ɂȂ�Ƃ��Ƀ��Z�b�g����
			if (accelerationLength < 0.0f)	accelerationLength = 0.0f;

			m_acceleration = accelerationNormal * accelerationLength;
			moveVelocity += m_acceleration;	// ��{���x�ɉ����x���㏑������
		}
	}

	///////////////////// �ړ��L�[�̓��͂��������ꍇ�̏��� /////////////////
	else
	{
		// ��{�ړ��ʂ��v�Z����
		moveVelocity += Vector3::Forward * PLAYER_SPEED;

		float acceleration = 0.05f; // �����x
		m_acceleration += Vector3::Forward * acceleration; // �����x�̌v�Z���s��

		// 2��ɂ��邱�Ƃŕ������O��
		if (m_acceleration.LengthSquared() > 1.0f)
		{
			m_acceleration.Normalize(); // �����1�ɐݒ肷��
		}

		moveVelocity += m_acceleration; // ��{�ړ��ɉ����x����悹����
		m_velocity = moveVelocity; // ���x��ۑ�����
	}

	moveVelocity *= PLAYER_SPEED;
	/////////////////////////// �ړ����� //////////////////////////////////
	m_position += Vector3::Transform(moveVelocity, Matrix::CreateRotationY(-m_angle));


	/////////////////////////// �p�[�e�B�N���̐��� //////////////////////////
	if (moveVelocity != Vector3::Zero)
	{
		m_particleTime += m_elapsedTime;
		if (m_particleTime >= 0.15f)
		{
			EventMessenger::Execute("CreateTrailDust", nullptr);
			m_particleTime = 0.0f;
		}
	}

	m_isInputMoveKey = false;	// �ړ��L�[�̓��͂����Z�b�g����
}


// --------------------------------
//  ���[���h�s��̌v�Z
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	// �s��̌v�Z���s��
	m_worldMatrix = Matrix::Identity;	// �X�V���Ƃɏ��������s��
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3::Zero)							// ���_�Ɉړ�
		*= Matrix::CreateScale		(PLAYER_SCALE)							// �v���C���[�̃T�C�Y�ύX

		*= Matrix::CreateRotationZ(m_animationRotate.z)
		*= Matrix::CreateRotationX(m_animationRotate.x)
		*= Matrix::CreateRotationY(m_animationRotate.y)

		*= Matrix::CreateRotationY	(-m_angle + XMConvertToRadians(180.f))	// �G�̕���������悤�ɐݒ肷��
		*= Matrix::CreateTranslation(m_position);							// ���W���ړ�������

}


// --------------------------------
//  �\������
// --------------------------------
void Player::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���f���̃G�t�F�N�g�����X�V����
	m_model->UpdateEffects([](DirectX::IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// ���C�g��L��������
				basicEffect->SetLightingEnabled(true);
				// �A���r�G���g���C�g�̐ݒ�
				basicEffect->SetAmbientLightColor(DirectX::Colors::Gray); // �K�؂ȃA���r�G���g�F��ݒ�
				// �K�v�ɉ����ă��C�g�̐ݒ���s��
				basicEffect->SetLightEnabled(0, true);
				basicEffect->SetLightDiffuseColor(0, DirectX::Colors::White);  // �f�B�t���[�Y�F��ݒ�
				basicEffect->SetLightSpecularColor(0, DirectX::Colors::White); // �X�y�L�����[�F��ݒ�

				basicEffect->SetLightEnabled(1, false); // �ǉ��̃��C�g���s�v�Ȃ疳����
				basicEffect->SetLightEnabled(2, false); // �ǉ��̃��C�g���s�v�Ȃ疳����
				// ���f���̎������F�����ɐݒ肵�Ė�����
				basicEffect->SetEmissiveColor(DirectX::Colors::Black);
			}
		}
	);

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
	
	// �����`�悷��
	m_sword->Render(view, projection);
#ifdef _DEBUG
#endif // !_DEBUG

}


// --------------------------------
//  �I������
// --------------------------------
void Player::Finalize()
{
}


// --------------------------------
//  �Փˏ���
// --------------------------------
void Player::HitAction(InterSectData data)
{
	// �̂Ƃ̓����蔻��
	HitBossBody(data);

	// ����Ƃ̓����蔻��
	HitCudgel(data);

	// �X�e�[�W�Ƃ̓����蔻��
	HitStage(data);

	// �S�u�����Ƃ̓����蔻��
	HitGoblin(data);
}


// --------------------------------
// �_���[�W����
// --------------------------------
void Player::Damage(float damage)
{
	// HP�����炷
	m_hp->Damage(damage);
	m_isHit = true;
	m_canHit = false;
	// �m�b�N�o�b�N������
	ChangeState(m_playerNockBacking.get());
	// ���ʉ���炷
	Sound::GetInstance()->PlaySE(Sound::SE_TYPE::PLAYER_DAMAGED);
}


// --------------------------------
//  �G�̑̂Ƃ̏Փ˔���
// --------------------------------
void Player::HitBossBody(InterSectData data)
{
	if (data.objType == ObjectType::Boss && data.colType == CollisionType::Sphere)
	{
		// �G�̃X�e�[�g���_�b�V���U���̏ꍇ�ő��肪�U�����̏ꍇ
		if (!m_isHit &&
			m_canHit)
		{
			Damage(1);
		}

		// �Փ˂����I�u�W�F�N�g�̏����擾
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		DirectX::BoundingSphere enemyCollision = *data.collision;

		// �����߂��ʂ��v�Z
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, enemyCollision);
		// y���W�ɂ͔��f�����ɐݒ�
		m_pushBackValue.y = 0;
		// �v���C���[�̈ʒu�������߂�
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}


// ---------------------------------
// �S�u�����Ƃ̏Փ�
// ---------------------------------
void Player::HitGoblin(InterSectData data)
{
	if (data.objType == ObjectType::Goblin && data.colType == CollisionType::Sphere)
	{
		// �G�̃X�e�[�g���_�b�V���U���̏ꍇ�ő��肪�U�����̏ꍇ
		if (!m_isHit &&
			m_canHit)
		{
			Damage(1);
		}

		// �Փ˂����I�u�W�F�N�g�̏����擾
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		DirectX::BoundingSphere goblinCollision = *data.collision;

		// �����߂��ʂ��v�Z
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, goblinCollision);
		// y���W�ɂ͔��f�����ɐݒ�
		m_pushBackValue.y = 0;
		// �v���C���[�̈ʒu�������߂�
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}


// --------------------------------
// �G�̕���i���_�j�Ƃ̏Փ˔���
// --------------------------------
void Player::HitCudgel(InterSectData data)
{
	if (!m_isHit &&
		m_canHit &&
		data.objType == ObjectType::Cudgel &&
		data.colType == CollisionType::OBB &&
		m_currentState != m_playerDodging.get()
		)
	{
		Damage(1);
	}
}


// --------------------------------
// �X�e�[�W�Ƃ̏Փ˔���
// --------------------------------
void Player::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		// �Փ˂����I�u�W�F�N�g�̏����擾
		DirectX::BoundingSphere stageCollision = *data.collision;

		// �����߂��ʂ��v�Z
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), stageCollision);
		// y���W�ɂ͔��f�����ɐݒ�
		m_pushBackValue.y = 0;
		// �v���C���[�̈ʒu�������߂�
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}


// --------------------------------
//  �Փˉ\��
// --------------------------------
void Player::CanHit(void* flag)
{
	m_canHit = *(bool*)flag;
}