// -------------------------------------
//
// ���̋O�Ղ̓���������
//
// -------------------------------------
#include "pch.h"
#include "Interface/IParticle.h"
#include "Effects/Header/SwordTrialParticle.h"


/// <summary>
/// �R���X�g���N�^
/// �����͏����ݒ�p�̐��l
/// </summary>

SwordTrialParticle::SwordTrialParticle(
	DirectX::VertexPositionTexture vertex[4],
	float life,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor
	)
	:m_startLife(life)			// ��������
	,m_life(life)				// ���ݎ���
	,m_startColor(startColor)	// �ŏ��̐F
	,m_nowColor(startColor)		// ���݂̐F
	,m_endColor(endColor)		// �I���̐F
{
	// �z��̊e�v�f��������
	for (int i = 0; i < 3; i++)
	{
		m_vertices[i] = vertex[i];
	}
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SwordTrialParticle::~SwordTrialParticle()
{
}


/// <summary>
/// �X�V�֐�
/// </summary>
/// <param name="elapsedTimer">Game������󂯎��</param>
/// <returns>��������(life)������Ԃ�true</returns>
bool SwordTrialParticle::Update(float elapsedTime)
{
	using namespace DirectX;

	// �F�B���݂̐������Ԃ���A�F��Lerp�ŎZ�o����
	m_nowColor = SimpleMath::Color::Lerp(
		m_startColor,
		DirectX::SimpleMath::Color(m_endColor.R(), m_endColor.G(), m_endColor.B(), 0), // �����x��0�ɐݒ肵�ē����ɂ���
		1.0f - m_life / m_startLife);

	//	���C�t�����炵�Ă���
	m_life -= elapsedTime;
	//	���C�t��0�����Ȃ玩�g�������Ă��炤
	if (m_life < 0.0f)return false;

	return true;
}