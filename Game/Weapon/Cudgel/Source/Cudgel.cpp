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
#include "Libraries/MyLib/Collision.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


const float Cudgel::CUDGEL_SCALE = Enemy::ENEMY_SCALE * 1.1f;						// ���_�̊g�嗦
const DirectX::SimpleMath::Vector3 Cudgel::DIRECTION_ENEMY = { 6.0f, 5.0f, 0.0f };	// ������̋����i�G�Ɣ�r�j
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_LENGTH =	{ 0.0f, 22.0f, 0.0f };	// ���_�̒����i��ԉ�����j
const DirectX::SimpleMath::Vector3 Cudgel::CUDGEL_HADLE_POS = { 0.0f, 7.0f, 0.0f };	// ���_�̎����̕����i��ԏ�j


// --------------------------------
// �R���X�g���N�^
// --------------------------------
Cudgel::Cudgel(PlayScene* playScene)
	:
	m_playScene(playScene)
	,m_currentState()
	,m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}


// --------------------------------
// �f�X�g���N�^
// --------------------------------
Cudgel::~Cudgel()
{
}


// --------------------------------
// ����������
// --------------------------------
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

	CreateModel(device);	// ���f���̐���
	CreateState();			// �X�e�[�g�̍쐬
	CreateCollision();		// �����蔻��̐���
}


// ���f���𐶐�����
void Cudgel::CreateModel(ID3D11Device1* device)
{

	m_model = std::make_unique<DirectX::Model>();													// ���f�����쐬

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);	// �G�t�F�N�g�t�@�N�g���[���쐬
	fx->SetDirectory(L"Resources/Models");															// ���f���̃f�B���N�g����ݒ�	
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/cudgel.cmo", *fx);			// �p�X�̃��f���̓ǂݍ��݂��s��
}


// --------------------------------
// ��Ԃ̐���
// --------------------------------
void Cudgel::CreateState()
{
	// ��Ԃ̐���
	m_idling = std::make_unique<Cudgel_Idling>(this);			// �ҋ@
	m_attacking = std::make_unique<Cudgel_Attacking>(this);		// �U��
	m_sweeping = std::make_unique<Cudgel_Sweeping>(this);		// �ガ����

	// ��Ԃ̏�����
	m_idling->Initialize();		// �ҋ@
	m_attacking->Initialize();	// �U��
	m_sweeping->Initialize();	// �ガ����

	// ������Ԃ��w��
	m_currentState = m_idling.get();
}

void Cudgel::CreateCollision()
{
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model.get());
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);
}

// --------------------------------
// �X�V����
// --------------------------------
void Cudgel::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}

// --------------------------------
// �`�揈��
// --------------------------------
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
		*m_collision.get(),					// �`�悷�鋫�E�{�b�N�X	
		Colors::White						// �F
	);
	m_primitiveBatch->End();
}



// --------------------------------
// �I������
// --------------------------------
void Cudgel::Finalize()
{
}


// --------------------------------
// �X�e�[�g�̕ύX
// --------------------------------
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



void Cudgel::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}