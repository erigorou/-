#include "pch.h"
#include "../Header/GoblinAttacking.h"

// �R���X�g���N�^
Goblin::GoblinAttacking::GoblinAttacking(Goblin* goblin)
	: m_goblin(goblin)
	, m_totalTime(0.0f)
{
}

// �I������
Goblin::GoblinAttacking::~GoblinAttacking()
{
}


// ����������
void Goblin::GoblinAttacking::Initialize(DirectX::Model* model)
{
	m_model = model;
}


// ���O�X�V
void Goblin::GoblinAttacking::PreUpdate()
{
}


// �X�V
void Goblin::GoblinAttacking::Update(const float& elapsedTime)
{
	m_totalTime += elapsedTime;
}


// ����X�V
void Goblin::GoblinAttacking::PostUpdate()
{
}


// �`��
void Goblin::GoblinAttacking::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// ���f���̕`��
	m_model->Draw(context, *states, DirectX::SimpleMath::Matrix::Identity, view, projection);
}


// �I������
void Goblin::GoblinAttacking::Finalize()
{
}


