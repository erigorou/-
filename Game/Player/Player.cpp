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


// �����ŐÓI�����o�[�ϐ����`����
const DirectX::SimpleMath::Vector3 Player::HOME_POSITION(0.0f);
const float Player::PLAYER_SPEED = 0.35f;
const float Player::PLAYER_SCALE = 0.1f;
const float Player::APPLIED_ATTACK_TIME = 1.0f;


// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Player::Player(PlayScene* playScene)
	:
	m_playScene(playScene),
	m_model{},
	m_position{0, 0, 40},
	m_angle{0.f},
	m_worldMatrix{},
	m_currentState{},
	m_keyboardState{},
	m_tracker{}
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
	auto states = resources->GetCommonStates();

	// �L�[���͂𐶐��i�S�̂Ŏg���j
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/momotaro.cmo", *fx);

	// �X�e�[�g�̍쐬
	CreateState();

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
//  �X�e�[�g���쐬����
// --------------------------------
void Player::CreateState()
{
	// �A�C�h�����O�X�e�[�g���擾
	m_playerIdling = std::make_unique<PlayerIdling>(this);
	// �A�C�h�����O�X�e�[�g�̏�����
	m_playerIdling->Initialize(m_model.get());
	// �_�b�W���O�X�e�[�g���擾
	m_playerDodging = std::make_unique<PlayerDodging>(this);
	// �_�b�W���O�X�e�[�g��������
	m_playerDodging->Initialize(m_model.get());

	// �A�^�b�L���O�X�e�[�g1���擾
	m_playerAttacking_1 = std::make_unique<PlayerAttacking_1>(this);
	m_playerAttacking_2 = std::make_unique<PlayerAttacking_2>(this);
	m_playerAttacking_3 = std::make_unique<PlayerAttacking_3>(this);
	m_playerAttacking_4 = std::make_unique<PlayerAttacking_4>(this);

	// �A�^�b�L���O�X�e�[�g1��������
	m_playerAttacking_1->Initialize(m_model.get());
	m_playerAttacking_2->Initialize(m_model.get());
	m_playerAttacking_3->Initialize(m_model.get());
	m_playerAttacking_4->Initialize(m_model.get());


	// �ŏ��̃X�e�[�g��ݒ�
	m_currentState = m_playerIdling.get();
}


/// <summary>
/// �X�e�[�g��ύX����
/// </summary>
/// <param name="newState">�ύX�������X�e�[�g</param>
void Player::ChangeState(IState* newState)
{
	// �����X�e�[�g�ōX�V���悤�Ƃ���Ƒ������^�[��
	if (m_currentState == newState) return;
	// ����X�V�������s��
	m_currentState->PostUpdate();
	// ���݂̃X�e�[�g��ύX����
	m_currentState = newState;
	// �V�����X�e�[�g�̎��O�X�V���s��
	m_currentState->PreUpdate();
}


// ���ԉ�A����
void Player::TimeComparison(float& nowTime, const float totalTime, IState* newState, const float elapsedTime)
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
void Player::Update(
	const DirectX::SimpleMath::Vector3 enemyPos,
	const float elapsedTime
	)
{
	// �X�e�[�g�̍X�V
	m_currentState->Update(elapsedTime, m_position);

	// �����ꂱ����1�s�݂̂ɂ���@����������������������������������������������������������

	// �G�̈ʒu���r���ĉ�]�p���v�Z����
	m_angle = Math::CalculationAngle(m_position, enemyPos);
	// ���[���h���W�̍X�V
	CalculationMatrix();
}


// ----------------------------------------
//  �G�̈ʒu����v���C���[�̉�]�p�����߂�
// ----------------------------------------
void Player::CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos)
{
	using namespace DirectX::SimpleMath;

	Vector3 forward = m_position - enemyPos;		// �G�̕������x�N�g���Ŏ擾
	forward.Normalize();										// ���K��

	Vector3 worldForward = Vector3::Forward;			// ���[���h���W�̑O���x�N�g�����쐬
	float dotProduct = forward.Dot(worldForward);	// ���ς��擾
	m_angle = acosf(dotProduct);								// ���ς���p�x���擾(�ʓx�@)

	Vector3 crossProduct = forward.Cross(worldForward);	// �J�����̑O�����x�N�g�����E�����Ɍ����Ă��邩�ǂ����ŕ���������
	m_angle = (crossProduct.y < 0)? -m_angle: m_angle;	// -180 ~ 180�Ɏ��߂�B
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

	m_velocity = SimpleMath::Vector3::Zero;	// ���x�l�����Z�b�g����

	if (keyboardState.Up)
		m_velocity += Vector3::Forward;	// �u���v�őO�i
	if (keyboardState.Down)
		m_velocity += Vector3::Backward;// �u���v�Ō��
	if (keyboardState.Left)
 		m_velocity += Vector3::Left;	// �u���v�ō��ړ�
	if (keyboardState.Right)
		m_velocity += Vector3::Right;	// �u���v�ŉE�ړ�

	if (m_velocity.LengthSquared() > 0.0f)		// �ړ��ʂ�����ꍇ�F
		m_velocity.Normalize();					// �ړ��ʂ𐳋K������

	m_velocity *= PLAYER_SPEED;					// �ړ��ʂ�␳����
	m_velocity = Math::truncate_vector(m_velocity, 2);

	m_position += Vector3::Transform(m_velocity, Matrix::CreateRotationY(-m_angle));	// �ړ��ʂ����W�ɔ��f
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
		*= Matrix::CreateTranslation(Vector3::Zero)										// ���_�Ɉړ�
		*= Matrix::CreateScale		(PLAYER_SCALE)										// �v���C���[�̃T�C�Y�ύX
		*= Matrix::CreateRotationY	(-m_angle + XMConvertToRadians(180.f))	// �G�̕���������悤�ɐݒ肷��
		*= Matrix::CreateTranslation(m_position);											// ���W���ړ�������
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

	//// �̂̋��E���̕`��
	//DrawBoundingSphere(device, context, states, view, projection,m_currentState->GetBoundingSphereBody());
}


// --------------------------------
// ���E����\��
// --------------------------------
void Player::DrawBoundingSphere(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const DirectX::BoundingSphere boundingSphere)
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
		boundingSphere,			// ���E��
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

