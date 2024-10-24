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

#include "TitleEnemy.h"
#include "EnemyTitleMoving.h"
#include "EnemyTitleIdling.h"

#include "Interface/IState.h"

// �Œ�l
const float TitleEnemy::TITLE_ENEMY_SPEED	= 0.1f;
const float TitleEnemy::TITLE_ENEMY_SCALE	= 1.0f;
const float TitleEnemy::COOL_TIME	= 0.4f;

// --------------------------------
//  �R���X�g���N�^
// --------------------------------
TitleEnemy::TitleEnemy(TitleScene* titleScene)
	: m_TitleScene(titleScene)
	, m_currentState()
	, m_position{0.f, 0.f, 0.f}
	, m_angle{0.f}
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
{
}

// --------------------------------
//  �f�X�g���N�^
// --------------------------------
TitleEnemy::~TitleEnemy()
{
}

// --------------------------------
//  �C�j�V�����C�Y
 // --------------------------------
void TitleEnemy::Initialize()
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
void TitleEnemy::CreateState()
{
	// === ��Ԃ̐��� ====
	m_titleIdling = std::make_unique<EnemyTitleIdling>(this);		// �^�C�g����ʂ̓G�̑ҋ@
	m_titleMoving = std::make_unique<EnemyTitleMoving>(this);	// �^�C�g����ʂ̓G�̈ړ�


	// === ��Ԃ̏����� ===
	m_titleIdling->Initialize(m_model.get());	// �^�C�g����ʂ̓G�̑ҋ@
	m_titleMoving->Initialize(m_model.get());	// �^�C�g����ʂ̓G�̈ړ�

	// �����̃X�e�[�g��ҋ@��ԂɊ��蓖�Ă�
	m_currentState = m_titleIdling.get();
}


// --------------------------------
//  ��Ԃ̐�������
// --------------------------------
void TitleEnemy::ChangeState(IState* newState)
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
void TitleEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_worldMatrix = Matrix::Identity;

	// �X�e�[�^�X���X�V���܁[��
	m_currentState->Update(elapsedTime);


	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));	// ��]�p�̐ݒ�

	m_velocity *= TitleEnemy::TITLE_ENEMY_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ���[���h�s��̌v�Z@;;;;;;;;;;;;;;;;
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(TITLE_ENEMY_SCALE)		// �T�C�Y�v�Z
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// �ʒu�̐ݒ�
}


// --------------------------------
//  �\������
// --------------------------------
void TitleEnemy::Render(
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
#endif // _DEBUG
}



// --------------------------------
//  �I������
// --------------------------------
void TitleEnemy::Finalize()
{
}