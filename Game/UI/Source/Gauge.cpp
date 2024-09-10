// ----------------
//
// �Q�[�W
//
// ----------------

#include "pch.h"
#include "Game/UI/Header/Gauge.h"
#include "Interface/IUserInterface.h"


// ----------------------------
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ---------------------------
GaugeUI::GaugeUI()
	:m_spriteBatch()
	,m_texture()
	,m_position()
	,m_size()
{
}



// ----------------------------
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ----------------------------
GaugeUI::~GaugeUI()
{}



// ----------------------------
/// <summary>
/// ����������
/// </summary>
/// <param name="position">���W�@�ianchor = mid�j</param>
/// <param name="size"></param>
// ----------------------------
void GaugeUI::Initialize(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 size)
{
	m_position	= position;
	m_size		= size;
}



// ----------------------------

/// <summary>
/// �X�V����
/// </summary>

// ----------------------------
void GaugeUI::Update()
{
}



// ----------------------------
/// <summary>
/// �`��֐�
/// </summary>
// ----------------------------
void GaugeUI::Render()
{
}



// ----------------------------
/// <summary>
/// �I���֐�
/// </summary>
// ----------------------------
void GaugeUI::Finalize()
{
}
