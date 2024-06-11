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

#include "Game/Enemy/Enemy.h"

#include "Interface/IState.h"

// �Œ�l
const float Enemy::ENEMY_SPEED = 0.05f;

// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Enemy::Enemy()
	:
	 m_position{0.0f, 0.0f, 0.0f}
	,m_angle{0.0f}
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
void Enemy::Initialize(
	ID3D11Device* device,
	const ID3D11DeviceContext* context,
	const DirectX::CommonStates* states
	)
{
	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// ���f����ǂݍ���(���ŃT�C�R����ǂݍ���)
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/akaoni.cmo", *fx);

	// �ҋ@��Ԃ��擾����
	m_enemyIdling = std::make_unique<EnemyIdling>(this);
	m_enemyIdling->Initialize();

	// �����̃X�e�[�g��ҋ@��ԂɊ��蓖�Ă�
	m_currentState = m_enemyIdling.get();

	// �r�w�C�r�A�c���[���擾
	m_pBT = std::make_unique<BehaviorTree>();
}

// --------------------------------
//  �X�V����
// --------------------------------
void Enemy::ChangeState(IState* newState)
{
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

	m_worldMatrix = Matrix::CreateScale(Vector3(2.0f, 2.0f, 2.0f));

	// �X�e�[�^�X���X�V���܁[��
	m_currentState->Update(elapsedTime);

	// �L�[���͂��󂯕t����B
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1)
	{
		m_position += Vector3(0.01f, 0.0f, 0.0f);
	}
	if (keyboardState.F2)
	{
		m_position -= Vector3(0.01f, 0.0f, 0.0f);
	}

	if (keyboardState.Enter)
	{
		m_pBT->run();
	}

	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}


// --------------------------------
//  �\������
// --------------------------------
void Enemy::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources
	)
{
	// �X�e�[�g�̂ق���\������
	m_currentState->Render(context,states,view,projection, resources);

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = resources->GetDebugString();
	debugString->AddString("enemyPos.x : %f", m_position.x);
	debugString->AddString("enemyPos.y : %f", m_position.y);
	debugString->AddString("enemyPos.z : %f", m_position.z);


}


// --------------------------------
//  �I������
// --------------------------------
void Enemy::Finalize()
{
}