// -------------------------------------- //
//
// �G�̊獷�� : �ҋ@
//
// -------------------------------------- //



#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "../../Enemy.h"

#include "../Header/EnemyFaceAttacking.h"


// --------------------
// �R���X�g���N�^
// --------------------
EnemyFaceAttacking::EnemyFaceAttacking(Enemy* enemy)
	: m_enemy(enemy)
{
	CreateModel();
}

// --------------------
// �f�X�g���N�^
// --------------------
EnemyFaceAttacking::~EnemyFaceAttacking()
{
}


// --------------------
// ���f���̐�������
// --------------------
void EnemyFaceAttacking::CreateModel()
{
	// ���\�[�X�̎擾
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// ���f���̓ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Face/AttackingFace.cmo", *fx);
}



// --------------------
// �f�X�g���N�^
// --------------------
void EnemyFaceAttacking::DrawFace(
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