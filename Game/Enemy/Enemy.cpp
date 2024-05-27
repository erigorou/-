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

// --------------------------------
//  �R���X�g���N�^
// --------------------------------
Enemy::Enemy()
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

	// ���W�̏����ݒ�
	m_position = DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, 0.0f };
	// ��]�p
	m_angle = 0.0f;
	// ���[���h�s��̏�����
	m_worldMatrix  = DirectX::SimpleMath::Matrix::Identity;

}

// --------------------------------
//  �X�V����
// --------------------------------
void Enemy::Update()
{
	using namespace DirectX::SimpleMath;

	m_worldMatrix = Matrix::CreateScale(Vector3(2.0f, 2.0f, 2.0f));

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