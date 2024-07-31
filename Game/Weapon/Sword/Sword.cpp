// ----------------
//
// �����Y�̓�
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

#include "Sword.h"
#include "Game/Player/Player.h"


// �Œ�l
const float Sword::SWORD_SCALE = Player::PLAYER_SCALE;
const DirectX::SimpleMath::Vector3 Sword::SWORD_DIR_FOR_PLAYER = DirectX::SimpleMath::Vector3(1.5f, 1.0f, 0.0f);

// �R���X�g���N�^
Sword::Sword(PlayScene* playScene)
	:
	m_playScene(playScene)
{
}

// �f�X�g���N�^
Sword::~Sword()
{
}

// ����������
void Sword::Initialize()
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
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/nihontou.cmo", *fx);


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

	// �X�e�[�g���쐬
	CreateState();

	// ���[���h���W�̏�����
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateScale(SWORD_SCALE);

}


// �V�[���𐶐�����
void Sword::CreateState()
{
	// �X�e�[�g�𐶐�����
	m_swordIdling = std::make_unique<Sword_Idling>(this);
	m_swordAttacking_1 = std::make_unique<Sword_Attacking_1>(this);

	// �X�e�[�g������������
	m_swordIdling->Initialize();
	m_swordAttacking_1->Initialize();

	// ���݂̃X�e�[�g��ݒ�
	m_currentState = m_swordIdling.get();
	//m_currentState = m_swordAttacking_1.get();
}

// --------------------------------------------
/// <summary>
/// �����Y�̓��̍X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
// --------------------------------------------
void Sword::Update(float elapsedTime)
{
	// ���݂̃X�e�[�g�̍X�V����
	m_currentState->Update(elapsedTime);
}


// �`�揈��
void Sword::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources)
{
	// ���݂̃X�e�[�g�̕`�揈��
	m_currentState->Render(context,states,view,projection);

	// ���E�{�b�N�X�̕`��
	DrawBoundingBox(device, context, states, view, projection);
}

// --------------------------------
// ���E�{�b�N�X��\��
// --------------------------------
void Sword::DrawBoundingBox(
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
		Colors::Yellow						// �F
	);
	m_primitiveBatch->End();
}



// �I������
void Sword::Finalize()
{
}


// �V�[���ύX����
void Sword::ChangeState(IWeapon* state)
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