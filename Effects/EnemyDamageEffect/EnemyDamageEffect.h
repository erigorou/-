#pragma once
#include "pch.h"

class CustomShader;

class EnemyDamageEffect
{
// ** �Œ�l **
private:
	// ���f���ɃG�t�F�N�g���|���Ă�������
	static constexpr float TOTAL_TIME = 2.5f;

	// �V�F�[�_�[�̃t�@�C���p�X
	static const wchar_t* PS_PATH;
	static constexpr wchar_t VS_PATH[] = L"Resources/Shaders/DamageEffect/DamageEffectVS.cso";


	// ���_���̓��C�A�E�g
	std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// �萔�o�b�t�@
	struct ConstBuffer
	{
		float Time; // ����
		DirectX::SimpleMath::Vector3 Padding;
	};



// ** ���J�֐� **
public:
	// �R���X�g���N�^
	EnemyDamageEffect();
	// �f�X�g���N�^
	~EnemyDamageEffect();

	void Update(float elapsedTime);

	// �̃p�[�c�̕`��
	void DrawWithDamageEffect(
		DirectX::Model* model,
		const DirectX::SimpleMath::Matrix world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	//�_���[�W���󂯂����Ƃ�ʒB����֐�
	void IsDamaged();

// ** ����J�֐� **
private:
	// �V�F�[�_�[�̐���
	void CreateShader();
	// �萔�o�b�t�@�̍쐬
	void CreateConstBuffer();


// ** �����o�ϐ� **
private:
	// �o�ߎ���
	float m_totalTime;
	// �_���[�W�G�t�F�N�g��t�^����̂�
	bool m_isDamaged;

	// �f�o�C�X���\�[�X
	ID3D11Device1* m_device;
	// �V�F�[�_�[
	std::unique_ptr<CustomShader> m_damageShader;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
};