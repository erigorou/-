#pragma once
#include "pch.h"

class CustomShader;

class EnemyDamageEffect
{
// ** �Œ�l **
private:
	// ���f���ɃG�t�F�N�g���|���Ă�������
	static constexpr float TOTAL_TIME = 0.25f;

	// �V�F�[�_�[�̃t�@�C���p�X
	static const wchar_t* VS_PATH;
	static const wchar_t* PS_PATH;

	// ���_���̓��C�A�E�g
	std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		float Time; // ���� (�V�F�[�_�[�p) // ��
		float Padding[3]; // 16�o�C�g�A���C�����g // ��
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