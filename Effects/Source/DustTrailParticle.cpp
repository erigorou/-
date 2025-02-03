// -------------------------------------
//
// �ړ������甭������y���̓���������
//
// -------------------------------------
#include "pch.h"
#include "Interface/IParticle.h"
#include "Effects/Header/DustTrailParticle.h"

// �I������
const static float ENDLESS = -100.0f;

// ----------------------------
// �R���X�g���N�^
// ----------------------------
DustTrailParticle::DustTrailParticle(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accelerator,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor
)
	:
	m_startLife(life),			// ��������
	m_life(life),				// ���ݎ���
	m_position(pos),			// ���W
	m_velocity(velocity),		// ���x
	m_accelerator(accelerator),	// �����x�x�N�g��
	m_startScale(startScale),	// �ŏ��̃T�C�Y
	m_nowScale(startScale),		// ���݂̃T�C�Y
	m_endScale(endScale),		// �I���̃T�C�Y
	m_startColor(startColor),	// �ŏ��̐F
	m_nowColor(startColor),		// ���݂̐F
	m_endColor(endColor)		// �I���̐F
{
}

// ----------------------------
// �f�X�g���N�^
// ----------------------------
DustTrailParticle::~DustTrailParticle()
{
}

// ----------------------------
// �X�V����
// ----------------------------
bool DustTrailParticle::Update(float elapsedTime)
{
	//	�X�P�[���B���݂̐������Ԃ���A�傫����Lerp�ŎZ�o����
	m_nowScale = DirectX::SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);

	// �F�B���݂̐������Ԃ���A�F��Lerp�ŎZ�o����
	m_nowColor = DirectX::SimpleMath::Color::Lerp(
		m_startColor,
		DirectX::SimpleMath::Color(m_endColor.R(), m_endColor.G(), m_endColor.B(), 0), // �����x��0�ɐݒ肵�ē����ɂ���
		1.0f - m_life / m_startLife
	);

	//	���x�̌v�Z�B�o�ߎ��ԂƉ����x���|���Z���đ������ƂŁA�����x���l�����Ĉړ����x��ω������Ă���
	m_velocity += m_accelerator * elapsedTime;
	//	���W�̌v�Z�B�o�ߎ��Ԃƈړ����x����A���݂̍��W���Z�o����
	m_position += m_velocity * elapsedTime;
	//	���C�t�����炵�Ă���
	m_life -= elapsedTime;
	//	���C�t��0�����Ȃ玩�g�������Ă��炤
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}