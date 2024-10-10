// -------------------------------------------------------
// ���S�̐e�N���X
// -------------------------------------------------------

#include "Goblin.h"
#include "Game/Player/Player.h"
#include "Game/Scene/PlayScene.h"

#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "Interface/IState.h"

// �Œ�l
const float Goblin::GOBLIN_SPEED = 0.1f;
const float Goblin::GOBLIN_SCALE = 0.4f;


// �R���X�g���N�^
Goblin::Goblin(PlayScene* playScene)
	: m_playScene(playScene)
	, m_position{ 0.f, 0.f, 0.f }
	, m_velocity{ 0.f, 0.f, 0.f }
	, m_angle{ 0.f, 0.f, 0.f }
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
{
}

// �f�X�g���N�^
Goblin::~Goblin()
{
}

// �������֐�
void Goblin::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/oni.cmo", *fx);


}


void Goblin::CreateState()
{
}


void Goblin::CreateCollision()
{
	// �����蔻��̐���
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE);

	// �����蔻���CollisionManager�ɓo�^����
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Goblin,
		this,
		m_bodyCollision.get());
}



void Goblin::Update(const float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// ���[���h�s��̏�����
	m_worldMatrix = Matrix::Identity;

	//// �X�e�[�g�̍X�V����
	//m_currentState->Update(elapsedTime);
}


void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	//m_currentState->Render();				// �X�e�[�g���̕`��
	m_model->Draw(context, *states, m_worldMatrix, view, projection);	// ���f���̕`��
}