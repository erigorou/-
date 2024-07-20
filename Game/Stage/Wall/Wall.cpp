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
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"

#include "Wall.h"

// �Œ�l
const float Wall::WALL_SCALE = 2.0f;

// �R���X�g���N�^
Wall::Wall()
	:m_worldMatrix()
	,m_model()
{
}

// �f�X�g���N�^
Wall::~Wall()
{
}

// ����������
void Wall::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���f���𐶐�
	m_model = std::make_unique<DirectX::Model>();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/wall.cmo", *fx);
}


// ���[���h���W�X�V����
void Wall::UpdateWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_worldMatrix = Matrix::Identity;									// ������
	m_worldMatrix *= Matrix::CreateScale(WALL_SCALE);		// �g��@�k��
}


// �`��
void Wall::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// ���[���h�s��̍X�V����
	UpdateWorldMatrix();
	// ���f����`�悷��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// �I������
void Wall::Finalize()
{
}