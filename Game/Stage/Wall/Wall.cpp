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
	using namespace DirectX;
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

	CreateCollision();
	
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

// �����蔻��̐���
void Wall::CreateCollision()
{
	m_collision = std::make_unique<DirectX::BoundingSphere>(DirectX::SimpleMath::Vector3::Zero, COLLISION_RADIUS);
}


// ���[���h���W�X�V����
void Wall::UpdateWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_worldMatrix = Matrix::Identity;					// ������
	m_worldMatrix *= Matrix::CreateScale(WALL_SCALE);	// �g��@�k��
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



#ifdef _DEBUG
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
		m_primitiveBatch.get(),		// �v���~�e�B�u�o�b�`
		*m_collision,				// ���E��
		DirectX::Colors::White		// �F
	);
	m_primitiveBatch->End();
#endif // !_DEBUG
}


// �I������
void Wall::Finalize()
{
}



void Wall::HitAction(InterSectData data)			{}
DirectX::SimpleMath::Vector3 Wall::GetPosition()	{ return DirectX::SimpleMath::Vector3(); }