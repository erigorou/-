#pragma once
#include "pch.h"

class CustomShader;

class EnemyDeadEffect
{
// ** �Œ�l **
private:
	// ���f���ɃG�t�F�N�g���|���Ă�������
	static constexpr float TOTAL_TIME = 2.5f;

	// �V�F�[�_�[�̃t�@�C���p�X
	static constexpr wchar_t PS_PATH[] = L"Resources/cso/DeadEffectPS.cso";
	static constexpr wchar_t VS_PATH[] = L"Resources/cso/DeadEffectVS.cso";


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
	EnemyDeadEffect();
	// �f�X�g���N�^
	~EnemyDeadEffect();

	void Update(float elapsedTime);

	// �̃p�[�c�̕`��
	void DrawWithDeadEffect(
		DirectX::Model* model,
		const DirectX::SimpleMath::Matrix world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	void IsDead();

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
	// �V�F�[�_�[
	std::unique_ptr<CustomShader> m_deadShader;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	// ���S�t���O
	bool m_isDead;
};