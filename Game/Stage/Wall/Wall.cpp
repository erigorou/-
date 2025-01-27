// -----------------------------------------------------------------
// * �V���ɂ���Ĉ�a�����o�Ȃ��悤�ɂ���ׂ̃X�e�[�W
// -----------------------------------------------------------------
#include "pch.h"
#include <Model.h>
#include <cassert>
// �w�b�_�[
#include "Game/CommonResources.h"	
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "Wall.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Messenger/EventMessenger.h"

// �Œ�l
const float Wall::WALL_SCALE = 2.0f;

// �R���X�g���N�^
Wall::Wall(PlayScene* playScene)
	: m_playScene(playScene)
	, m_worldMatrix()
	, m_model()
{
}

// �f�X�g���N�^
Wall::~Wall()
{
}

// ����������
void Wall::Initialize()
{
	// ���f�����擾����
	m_model = GameResources::GetInstance()->GetModel("wall");

	// �����蔻��̐���
	CreateCollision();
}

// �����蔻��̐���
void Wall::CreateCollision()
{
	m_collision		= std::make_unique<DirectX::BoundingSphere>(DirectX::SimpleMath::Vector3::Zero, COLLISION_RADIUS);
	m_overCollision = std::make_unique<DirectX::BoundingSphere>(DirectX::SimpleMath::Vector3::Zero, COLLISION_RADIUS * COLLISION_SCALE);


	// �Փ˃f�[�^�̍쐬
	CollisionData<DirectX::BoundingSphere>data =
	{
		ObjectType::Stage,		// �I�u�W�F�N�g�̎��
		CollisionType::Sphere,	// �����蔻��̎��
		this,					// ���̃N���X�̃|�C���^
		m_collision.get()		// �����蔻��̃|�C���^
	};


	// �Փ˃f�[�^�̍쐬
	CollisionData<DirectX::BoundingSphere>overData =
	{
		ObjectType::Stage,		// �I�u�W�F�N�g�̎��
		CollisionType::Sphere,	// �����蔻��̎��
		this,					// ���̃N���X�̃|�C���^
		m_overCollision.get()	// �����蔻��̃|�C���^
	};


	// �Փ˔����Manager�ɓo�^
	EventMessenger::Execute("AddSphereCollision", &data);
	EventMessenger::Execute("AddSphereCollision", &overData);
}


// ���[���h���W�X�V����
void Wall::UpdateWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_worldMatrix = Matrix::Identity;						// ������
	m_worldMatrix *= Matrix::CreateScale(WALL_SCALE);		// �g��@�k��
	m_worldMatrix *= Matrix::CreateTranslation(WALL_POS);	// �ړ�
}



// �`��
void Wall::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���[���h�s��̍X�V����
	UpdateWorldMatrix();
	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// �I������
void Wall::Finalize()
{
}



void Wall::HitAction(InterSectData data)			
{
	UNREFERENCED_PARAMETER(data);
}