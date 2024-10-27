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
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Stage/Wall/Wall.h"


// �����ŐÓI�����o�[�ϐ����`����
const DirectX::SimpleMath::Vector3 Player::HOME_POSITION(0.0f);
const float Player::PLAYER_SPEED = 0.01f;
const float Player::PLAYER_SCALE = 0.1f;


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Player::Player(PlayScene* playScene)
	:
	m_playScene(playScene),
	m_model{},
	m_position{ 0, 0, 40 },
	m_angle{ 0.f },
	m_acceleration{},
	m_pushBackValue{},
	m_worldMatrix{},
	m_currentState{},
	m_keyboardState{},
	m_tracker{},
	m_particleTime{},
	m_isHit{},
	m_coolTime{},
	m_canHit{false}
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
	using namespace DirectX;
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// �L�[���͂𐶐��i�S�̂Ŏg���j
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/momotaro.cmo", *fx);

	// �����蔻��̍쐬
	CreateCollision();

	// �X�e�[�g�̍쐬
	CreateState();

	// HP���Ǘ�����N���X�̐���
	m_hp = std::make_unique<PlayerHP>();

	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);
	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

}


// --------------------------------
// �����蔻��̍쐬�֐�
// --------------------------------
void Player::CreateCollision()
{
	// �̂̓����蔻����쐬
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>
		(
			m_position, PLAYER_SCALE * 12
		);

	
	// �����蔻����L�^����
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Player,		// �I�u�W�F�N�g�̎��
		this,					// �I�u�W�F�N�g�̃A�h���X
		m_bodyCollision.get()	// �����蔻��̃A�h���X
	);
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
	m_playerAttacking_3 = std::make_unique<PlayerAttacking_3>	(this);
	m_playerAttacking_4 = std::make_unique<PlayerAttacking_4>	(this);

	//////////////////////�X�e�[�g�̏�����////////////////////////////
	m_playerIdling		->Initialize(m_model.get());
	m_playerDodging		->Initialize(m_model.get());
	m_playerAttacking_1	->Initialize(m_model.get());
	m_playerAttacking_2	->Initialize(m_model.get());
	m_playerAttacking_3	->Initialize(m_model.get());
	m_playerAttacking_4	->Initialize(m_model.get());

	// �ŏ��̃X�e�[�g��ݒ�
	m_currentState = m_playerIdling.get();
}


/// <summary>
/// �X�e�[�g��ύX����
/// </summary>
/// <param name="newState">�ύX�������X�e�[�g</param>
void Player::ChangeState(IPlayer* newState)
{
	// �����X�e�[�g�ōX�V���悤�Ƃ���Ƒ������^�[��
	if (m_currentState == newState) return;

	m_currentState->PostUpdate();	// ����X�V�������s��
	m_currentState = newState;		// ���݂̃X�e�[�g��ύX����
	m_currentState->PreUpdate();	// �V�����X�e�[�g�̎��O�X�V���s��
}


// ---------------------------------------------------------
/// <summary>
/// �s���ɑ΂��鎞�Ԃ��v�Z����֐�
/// </summary>
/// <param name="nowTime">���݂̎���</param>
/// <param name="totalTime">�X�e�[�g��ύX���鎞��</param>
/// <param name="newState">�ύX���鎞��</param>
/// <param name="elapsedTime">�o�ߎ���</param>
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
/// <summary>
/// �v���C���[�̍X�V����
/// </summary>
/// <param name="enemyPos">�G�̍��W</param>
/// <param name="elapsedTime">�o�ߎ���</param>
// ---------------------------------------------
void Player::Update(const DirectX::SimpleMath::Vector3 enemyPos,const float elapsedTime)
{
	m_elapsedTime = elapsedTime;	// �o�ߎ��Ԃ�ۑ�����

	////////////////////�X�e�[�g�̍X�V/////////////////////////////
	m_currentState->Update(elapsedTime);

	///////////////////�v���C���[�̈ړ�////////////////////////////
	m_angle = Math::CalculationAngle(m_position, enemyPos);
	CalculationMatrix();
	m_pushBackValue = Vector3::Zero;
	///////////////////�����蔻��̍X�V////////////////////////////
	m_bodyCollision->Center = m_position;

	// �N�[���^�C�����v����
	if (m_isHit && m_coolTime < COOL_TIME)	{ m_coolTime += elapsedTime;		 }
	// �N�[���^�C���I���
	else if (m_coolTime >= COOL_TIME)		{ m_isHit = false;m_coolTime = 0.0f; }
}


// ----------------------------------------------
/// <summary>
/// �L�[���͂��擾����
/// </summary>
/// <param name="key">���͂��ꂽ1�L�[</param>
// ----------------------------------------------
void Player::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	m_currentState->OnKeyPressed(key);
}


void Player::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	if (m_currentState == this->GetPlayerIdlingState())
	{
		// �ړ�
		if (key == DirectX::Keyboard::Up)		m_inputVelocity += Vector3::Forward;	// �u���v�őO�i
		if (key == DirectX::Keyboard::Down)		m_inputVelocity += Vector3::Backward;	// �u���v�Ō��
		if (key == DirectX::Keyboard::Left)		m_inputVelocity += Vector3::Left;		// �u���v�ō��ړ�
		if (key == DirectX::Keyboard::Right)	m_inputVelocity += Vector3::Right;		// �u���v�ŉE�ړ�
	}

	m_currentState->OnKeyDown(key);
}



// ----------------------------------------------
/// <summary>
/// ��]�p�̌v�Z�֐�
/// </summary>
/// <param name="enemyPos"></param>
//�@---------------------------------------------
void Player::CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos)
{
	using namespace DirectX::SimpleMath;

	Vector3 forward = m_position - enemyPos;		// �G�̕������x�N�g���Ŏ擾
	forward.Normalize();							// ���K��

	Vector3 worldForward = Vector3::Forward;		// ���[���h���W�̑O���x�N�g�����쐬
	float dotProduct = forward.Dot(worldForward);	// ���ς��擾
	m_angle = acosf(dotProduct);					// ���ς���p�x���擾(�ʓx�@)

	Vector3 crossProduct = forward.Cross(worldForward);	// �J�����̑O�����x�N�g�����E�����Ɍ����Ă��邩�ǂ����ŕ���������6
	m_angle = (crossProduct.y < 0) ? -m_angle: m_angle;	// -180 ~ 180�Ɏ��߂�B
}



// --------------------------------
//  �ړ��̊Ǘ�
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// �L�[���͂��󂯕t����B
	Keyboard::State keyboardState = Keyboard::Get().GetState();

	Vector3 moveVelocity  = Vector3::Zero;	// �����p���x


	m_inputVelocity.Normalize();
	m_direction = m_inputVelocity;
	

	///////////////////// �ړ��L�[�̓��͂��Ȃ��ꍇ�̏��� /////////////////
	if (m_inputVelocity == Vector3::Zero)
	{
		float accelerationLength = m_acceleration.Length();				// ���x�̒������擾����
		// 0�̋ߎ��l���傫���ꍇ
		if (accelerationLength >= FLT_EPSILON)
		{
			Vector3 accelerationNormal = m_acceleration / accelerationLength;// �ێ���������x�̐��K���x�N�g�����擾����

			float friction = 0.1f;			// ���C��
			accelerationLength -= friction;	// ���C���v�Z

			// �����x���i�[�j�ɂȂ�Ƃ��Ƀ��Z�b�g����
			if (accelerationLength < 0.0f)	accelerationLength = 0.0f;

			m_acceleration = accelerationNormal * accelerationLength;
			moveVelocity += m_acceleration;								// ��{���x�ɉ����x���㏑������
		}
	}

	///////////////////// �ړ��L�[�̓��͂��������ꍇ�̏��� /////////////////
	else
	{
		// ��{�ړ��ʂ��v�Z����
		moveVelocity += m_inputVelocity * PLAYER_SPEED;

		float acceleration = 0.05f;							// �����x
		m_acceleration += m_inputVelocity * acceleration;		// �����x�̌v�Z���s��

		// 2��ɂ��邱�Ƃŕ������O��
		if (m_acceleration.LengthSquared() > 1)
		{
			m_acceleration.Normalize(); // �����1�ɐݒ肷��
		}

		moveVelocity += m_acceleration;		// ��{�ړ��ɉ����x����悹����
		m_velocity = moveVelocity;			// ���x��ۑ�����
	}

	/////////////////////////// �ړ����� //////////////////////////////////
	m_position += Vector3::Transform(moveVelocity, Matrix::CreateRotationY(-m_angle));

	/////////////////////////// �p�[�e�B�N���̐��� //////////////////////////
	if (moveVelocity != Vector3::Zero)
	{
		m_particleTime += m_elapsedTime;
		if (m_particleTime >= 0.15f)
		{
			m_playScene->GetParticle()->CreateTrailDust();
			m_particleTime = 0.0f;
		}
	}

	m_inputVelocity = Vector3::Zero;	// ��{���x
}


// --------------------------------
//  ���[���h�s��̌v�Z
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	// �s��̌v�Z���s��
	m_worldMatrix = Matrix::Identity;		// �X�V���Ƃɏ��������s��
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3::Zero)							// ���_�Ɉړ�
		*= Matrix::CreateScale		(PLAYER_SCALE)							// �v���C���[�̃T�C�Y�ύX
		*= Matrix::CreateRotationY	(-m_angle + XMConvertToRadians(180.f))	// �G�̕���������悤�ɐݒ肷��
		*= Matrix::CreateTranslation(m_position);							// ���W���ړ�������
}


// --------------------------------
//  �\������
// --------------------------------
void Player::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources
	)
{
	using namespace DirectX;

	context->OMSetDepthStencilState(states->DepthDefault(), 0);	// �[�x�l���Q�Ƃ��ď�������

	// ���f���̃G�t�F�N�g�����X�V����
	m_model->UpdateEffects([](DirectX::IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// ���C�g��L��������
				basicEffect->SetLightingEnabled(true);
				// �A���r�G���g���C�g�̐ݒ�
				basicEffect->SetAmbientLightColor(Colors::Gray); // �K�؂ȃA���r�G���g�F��ݒ�
				// �K�v�ɉ����ă��C�g�̐ݒ���s��
				basicEffect->SetLightEnabled(0, true);
				basicEffect->SetLightDiffuseColor(0, Colors::White);  // �f�B�t���[�Y�F��ݒ�
				basicEffect->SetLightSpecularColor(0, Colors::White); // �X�y�L�����[�F��ݒ�

				basicEffect->SetLightEnabled(1, false); // �ǉ��̃��C�g���s�v�Ȃ疳����
				basicEffect->SetLightEnabled(2, false); // �ǉ��̃��C�g���s�v�Ȃ疳����
				// ���f���̎������F�����ɐݒ肵�Ė�����
				basicEffect->SetEmissiveColor(Colors::Black);
			}
		}
	);

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// �X�e�[�g�ŕ`�悷��
	m_currentState->Render(
		context,
		states,
		view,
		projection);

#ifdef _DEBUG
	// �̂̋��E���̕`��
	DrawBoundingSphere(device, context, states, view, projection, m_bodyCollision.get());

	auto debugString = resources->GetDebugString();
	debugString->AddString("push : %f, %f, %f", m_pushBackValue.x, m_pushBackValue.y, m_pushBackValue.z);
	debugString->AddString("PlayerPos : %f, %f, %f", m_position.x, m_position.y, m_position.z);
	
	if (m_canHit)	debugString->AddString("Player_CAN");
	if (!m_isHit)	debugString->AddString("Player_nonHit");

#endif // !_DEBUG

}


// --------------------------------
// ���E����\��
// --------------------------------
void Player::DrawBoundingSphere(
	ID3D11Device*						device,
	ID3D11DeviceContext*				context,
	DirectX::CommonStates*				states,
	const DirectX::SimpleMath::Matrix&	view,
	const DirectX::SimpleMath::Matrix&	projection,
	const DirectX::BoundingSphere*		boundingSphere)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(device);


	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);
	// �`��
	m_primitiveBatch->Begin();
	DX::Draw(
		m_primitiveBatch.get(),	// �v���~�e�B�u�o�b�`
		*boundingSphere,		// ���E��
		Colors::White			// �F
	);
	m_primitiveBatch->End();
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
	//////////////////////�G�̑̂ƏՓ˂������̏���////////////////////////////
	if (data.objType == ObjectType::Enemy && data.colType == CollisionType::Sphere)
	{
		// �Փ˂����I�u�W�F�N�g�̏����擾
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		auto enemy = dynamic_cast<Enemy*>(data.object);
		DirectX::BoundingSphere enemyCollision = enemy->GetBodyCollision();

		// �����߂��ʂ��v�Z
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, enemyCollision);
		// y���W�ɂ͔��f�����ɐݒ�
		m_pushBackValue.y = 0;
		// �v���C���[�̈ʒu�������߂�
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}


	/////////////////////�G��������ƏՓ˂������̏���////////////////////////
	else if (	! m_isHit							&&
				m_canHit							&&
				data.objType == ObjectType::Cudgel	&&
				data.colType == CollisionType::OBB	&&
				m_currentState != m_playerDodging.get())
	{
		// HP�����炷
		m_hp->Damage(1);
		m_isHit = true;
		m_canHit = false;
		// �m�b�N�o�b�N������
	}


	////////////////////�X�e�[�W�ƏՓ˂����Ƃ��̏���//////////////////////////
	else if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		// �Փ˂����I�u�W�F�N�g�̏����擾
		auto wall = dynamic_cast<Wall*>(data.object);
		DirectX::BoundingSphere* stageCollision = wall->GetCollision();

		// �����߂��ʂ��v�Z
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), *stageCollision);
		// y���W�ɂ͔��f�����ɐݒ�
		m_pushBackValue.y = 0;
		// �v���C���[�̈ʒu�������߂�
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}

}



