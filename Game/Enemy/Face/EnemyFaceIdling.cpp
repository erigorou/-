// -------------------------------------- //
//
// �G�̊獷�� : �ҋ@
//
// -------------------------------------- //



#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

#include "EnemyFaceIdling.h"


// --------------------
// �R���X�g���N�^
// --------------------
Enemy::EnemyFaceIdling::EnemyFaceIdling(Enemy* enemy)
	: m_enemy(enemy)
{
	CreateModel();
}

// --------------------
// �f�X�g���N�^
// --------------------
Enemy::EnemyFaceIdling::~EnemyFaceIdling()
{
}


// --------------------
// ���f���̐�������
// --------------------
void Enemy::EnemyFaceIdling::CreateModel()
{
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// ���f���̓ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Face/IdlingFace.cmo", *fx);
}



// --------------------
// �f�X�g���N�^
// --------------------
void Enemy::EnemyFaceIdling::DrawFace(
	const DirectX::SimpleMath::Matrix mat,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj
)
{
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// ���f���̕`��
	m_model->Draw(context, *resources->GetCommonStates(), mat, view, proj);
}