#pragma once

class Enemy
{
	// �Œ�l
public:

public:
	// �S�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	// �S�̉�]�p���擾����
	float GetAngle() { return m_angle; }

public:
	// �R���X�g���N�^
	Enemy();
	// �f�X�g���N�^
	~Enemy();

	// ����������
	void Initialize(
		ID3D11Device* device,
		const ID3D11DeviceContext* context,
		const DirectX::CommonStates* states
	);

	// �X�V����
	void Update();

	// �`�揈��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources
	);

	// �I������
	void Finalize();


private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;

	// �G�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;
};
