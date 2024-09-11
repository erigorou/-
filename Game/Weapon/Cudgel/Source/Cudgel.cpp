// ----------------
//
// �S�̋��_
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>
// �w�b�_�[
#include "Game/CommonResources.h"	
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


// �Œ�l									0.7f
const float Cudgel::CUDGEL_SCALE = Enemy::ENEMY_SCALE * 1.1f;
const DirectX::SimpleMath::Vector3 Cudgel::DIRECTION_ENEMY = { 6.0f, 5.0f, 0.0f };

// �R���X�g���N�^
Cudgel::Cudgel(PlayScene* playScene)
	:
	m_playScene(playScene)
	,m_currentState()
	,m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}

// �f�X�g���N�^
Cudgel::~Cudgel()
{
}

// ����������
void Cudgel::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

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

	// ���f���̐���
	CreateModel(device);

	// �X�e�[�g���쐬
	CreateState();
}


// ���f���𐶐�����
void Cudgel::CreateModel(ID3D11Device1* device)
{
	// ���f���𐶐�
	m_model = std::make_unique<DirectX::Model>();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/cudgel.cmo", *fx);
}


// �V�[���𐶐�����
void Cudgel::CreateState()
{
	// ��Ԃ̐���
	m_idling = std::make_unique<Cudgel_Idling>(this);			// �ҋ@
	m_attacking = std::make_unique<Cudgel_Attacking>(this);		// �U��

	// ��Ԃ̏�����
	m_idling->Initialize();		// �ҋ@
	m_attacking->Initialize();	// �U��

	// ������Ԃ��w��
	m_currentState = m_idling.get();
}

// �X�V����
void Cudgel::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}


// �`�揈��
void Cudgel::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// ���݂̃X�e�[�g�̕`�揈��
	m_currentState->Render(context, states, view, projection);

	// ���E�{�b�N�X�̕`��
	DrawBoundingBox(device, context, states, view, projection);
}

// --------------------------------
// ���E�{�b�N�X��\��
// --------------------------------
void Cudgel::DrawBoundingBox(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
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
		m_primitiveBatch.get(),				// �v���~�e�B�u�o�b�`
		m_currentState->GetBoundingBox(),	// �����蔻��
		Colors::White						// �F
	);
	m_primitiveBatch->End();
}



// �I������
void Cudgel::Finalize()
{
}


// �V�[���ύX����
void Cudgel::ChangeState(IWeapon* state)
{
	// �����X�e�[�g�ɕύX���悤�Ƃ����瑁�����^�[��
	if (m_currentState == state)return;
	// �X�e�[�g�̎��㏈��
	m_currentState->PostUpdate();
	// �V�����X�e�[�g�ɐ؂�ւ���
	m_currentState = state;
	// �V�����X�e�[�g�̎��O�������s��
	m_currentState->PreUpdate();
}