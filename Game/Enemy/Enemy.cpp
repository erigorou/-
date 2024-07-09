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

#include "Game/Enemy/Enemy.h"

#include "Interface/IState.h"

// �Œ�l
const float Enemy::ENEMY_SPEED = 0.2f;
const float Enemy::ENEMY_SCALE = 0.8f;

// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Enemy::Enemy(PlayScene* playScene)
	:
	m_playScene(playScene)
	,m_position{0.f, 0.f, 0.f}
	,m_angle{0.f}
	,m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
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
	// ���f����ǂݍ���(���ŃT�C�R����ǂݍ���)
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/oni.cmo", *fx);

	// HP��ݒ�
	m_hp = std::make_unique<Enemy_HP>(5);
	m_hp->SetHP(5);

	// �r�w�C�r�A�c���[���擾
	m_pBT = std::make_unique<BehaviorTree>();

	// �X�e�[�g�̍쐬
	CreateState();

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
	// �ҋ@��Ԃ��擾����
	m_enemyIdling = std::make_unique<EnemyIdling>(this);
	m_enemyIdling->Initialize(m_model.get());
	// �ǔ���Ԃ��擾����
	m_enemyApproaching = std::make_unique<EnemyApproaching>(this);
	m_enemyApproaching->Initialize(m_model.get());
	// �����̃X�e�[�g��ҋ@��ԂɊ��蓖�Ă�
	m_currentState = m_enemyIdling.get();
}



// --------------------------------
//  �X�V����
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
	m_currentState->Update(elapsedTime, m_position);

	// �L�[���͂��󂯕t����B
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1)
	{m_velocity += Vector3::Forward * Enemy::ENEMY_SPEED;}

	if (keyboardState.F2)
	{m_velocity += Vector3::Backward * Enemy::ENEMY_SPEED;}


	if (keyboardState.Enter)
	{
		// �r�w�C�r�A�c���[�����s�B
		m_pBT->run();
	}

	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));	// ��]�p�̐ݒ�

	m_velocity *= Enemy::ENEMY_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ���[���h�s��̌v�Z
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(ENEMY_SCALE)			// �T�C�Y�v�Z
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// �ʒu�̐ݒ�
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

	// ���\�[�X���擾
	CommonResources* resources = CommonResources::GetInstance();

	// �X�e�[�g��\������
	m_currentState->Render(context,states,view,projection);

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// ���E���̕`��
	DrawBoundingSphere(device, context, states, view, projection, m_currentState->GetBoundingSphereBody());

	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyPos : %f : %f : %f", m_position.x, m_position.y, m_position.z);

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
	const DirectX::BoundingSphere boundingSphere
	)
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
		boundingSphere,			// �`�悵�������E��
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