/// ---------------------------
/// 
///
/// �S�̋��_�̑ҋ@���
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Idling.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// -----------------------------------------------
// �R���X�g���N�^
// -----------------------------------------------
Cudgel_Idling::Cudgel_Idling(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
}


// -----------------------------------------------
// �f�X�g���N�^
// -----------------------------------------------
Cudgel_Idling::~Cudgel_Idling()
{
}


// -----------------------------------------------
// ����������
// -----------------------------------------------
void Cudgel_Idling::Initialize()
{
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;		// ���[���h�s��̏�����
	m_model = m_cudgel->GetModel();								// ���f�����擾
}


// -----------------------------------------------
// ���O����
// -----------------------------------------------
void Cudgel_Idling::PreUpdate()
{
}


// -----------------------------------------------
// �X�V����
// -----------------------------------------------
void Cudgel_Idling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	if (!m_cudgel->GetPlayScene()->GetEnemy()) return;

	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	m_position = enemy->GetPosition();	// �G�̍��W���擾
	m_angle	= enemy->GetAngle();		// �G�̉�]���擾

	// ���[���h�s����v�Z����
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(Cudgel::CUDGEL_SCALE)		// �傫���̐ݒ�@���@���Z�b�g
		*= DirectX::SimpleMath::Matrix::CreateTranslation(Cudgel::DIRECTION_ENEMY)		// ���_�̈ʒu���炷�������炷
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)						// ���S�̂̉�]���s��
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);					// �S�̍��W�Ɉړ�����

	m_cudgel->SetCollisionPosition(m_worldMatrix);		// �����蔻��̈ʒu��ݒ肷��
}


// -----------------------------------------------
// ���㏈��
// -----------------------------------------------
void Cudgel_Idling::PostUpdate()
{
}


// -----------------------------------------------
// �`�揈��
// -----------------------------------------------
void Cudgel_Idling::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// �G�����Ȃ��ꍇ�͕`�悵�Ȃ�
	if (m_cudgel->GetPlayScene()->GetEnemy() == nullptr) return;

	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);


#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // !_DEBUG
}


// -----------------------------------------------
// �I������
// -----------------------------------------------
void Cudgel_Idling::Finalize()
{
}



// -----------------------------------------------
// �Փ˔���
// -----------------------------------------------
void Cudgel_Idling::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);

	// Idling��ԂŏՓ˂��Ă��A���ɂ͂Ȃ�
}
