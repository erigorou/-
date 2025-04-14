// ---------------------------------------------
// 
// ���O		:SelectSceneObject.cpp
// ���e		:�Z���N�g�V�[���ŕ\������I�u�W�F�N�g
// �쐬		:�r�c����
// 
// ---------------------------------------------
// �C���N���[�h
#include "pch.h"
#include "SelectSceneObject.h"
#include "Game/GameResources.h"
#include "Game/Data/GameData.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

// -----------------------------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
SelectSceneObject::SelectSceneObject()
	: 
	m_models(),
	m_stageNo(),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}

// -----------------------------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
SelectSceneObject::~SelectSceneObject()
{
	Finalize();
}

// -----------------------------------------------
/// <summary>
/// ����������
/// </summary>
void SelectSceneObject::Initialize()
{
	// ���f���̎󂯎��
	LoadModel();
}

// -----------------------------------------------
/// <summary>
/// �X�V����
/// </summary>
void SelectSceneObject::Update(float elapsedTime)
{
	// �Q�[���f�[�^�ɃA�N�Z�X
	auto gameData = GameData::GetInstance();
	// �X�e�[�W�ԍ��̎擾
	m_stageNo = gameData->GetSelectStage();

	// ���f���̉�]
	m_rotation.y -= ROTATE_SPEED * elapsedTime;

	// ���[���h�s��̌v�Z
	CalcWorldMatrix();
}

// -----------------------------------------------
/// <summary>
/// �`�揈��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// /// <param name="proj">�ˉe�s��</param>
void SelectSceneObject::Render(const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj)
{
	if (m_models[m_stageNo] == nullptr) return;

	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// �G�t�F�N�g�̐ݒ�
	m_models[m_stageNo]->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetLightingEnabled(false); // ���C�g�L����
				basicEffect->SetPerPixelLighting(true); // �s�N�Z���P�ʂ̃��C�e�B���O�L����
				basicEffect->SetTextureEnabled(false); // �e�N�X�`���̖�����
				basicEffect->SetVertexColorEnabled(false); // ���_�J���[�̖�����
				basicEffect->SetFogEnabled(false); // �t�H�O�̖�����
			}
		}
	);

	m_models[m_stageNo]->Draw(context, *states, m_worldMatrix, view, proj, false, [&]
		{
			// �u�����h�X�e�[�g��ݒ�
			context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
		}
	);
}

// -----------------------------------------------
/// <summary>
/// �I������
/// </summary>
void SelectSceneObject::Finalize()
{
}

// -----------------------------------------------
/// <summary>
/// ���f���̎󂯎��
/// </summary>
void SelectSceneObject::LoadModel()
{
	// ���f���̎擾
	m_models.push_back(GameResources::GetInstance()->GetModel("selectTutorial"));
	m_models.push_back(GameResources::GetInstance()->GetModel("selectBoss"));
}

// -----------------------------------------------
/// <summary>
/// ���[���h�s��̌v�Z
/// </summary>
void SelectSceneObject::CalcWorldMatrix()
{
	// ���[���h�s��̌v�Z
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	// ��]�s��̌v�Z(���[�s�b�`���[��)
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll
	(
		m_rotation.y,
		m_rotation.x,
		m_rotation.z
	);

	// �ړ��s��̌v�Z
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(
		MODEL_POSITION.x,
		MODEL_POSITION.y,
		MODEL_POSITION.z
	);
}