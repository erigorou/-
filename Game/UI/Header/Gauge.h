#pragma once

#ifndef GAUGE_UI
#define GAUGE_UI

// �v���R���p�C���w�b�_�[
#include "pch.h"
#include "Interface/IUserInterface.h"


// �Q�[�W�N���X
class GaugeUI : public IUserInterface
{
public:
	// �R���X�g���N�^
	GaugeUI();
	// �f�X�g���N�^
	~GaugeUI()	override;
	// �������֐�
	void Initialize(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 size)override;
	// �X�V����
	void Update()	override;
	// �`�揈��
	void Render()	override;
	// �I���֐�
	void Finalize()	override;

	// �����֐�
	void MinusValue(float value)	{ m_value += value; }
	// �㏸�֐�
	void PlusValue(float value)		{ m_value -= value; }
	// �Q�[�W�ʂ�ݒ�
	void SetMaxValue(float max)		{ m_value = max; }
	// �Q�[�W�ʂ̎擾
	float GetValue()				{ return m_value; }

private:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �Q�[�W�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;


	// ���W
	DirectX::SimpleMath::Vector2 m_position;
	// �傫��
	DirectX::SimpleMath::Vector2 m_size;
	// �Q�[�W�̗�
	float m_value;
};


#endif