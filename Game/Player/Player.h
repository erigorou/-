
#pragma once

class Player
{
public:
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	static const float PLAYER_SPEED;

public:
	// �v���C���[�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	// �v���C���[�̉�]�p���擾����
	float GetAngle() { return m_angle; }

public:
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize(
		 ID3D11Device* device,
		const ID3D11DeviceContext* context,
		const DirectX::CommonStates* states
	);
	// �X�V����
	void Update(
		DirectX::SimpleMath::Vector3 enemyPos
	);

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
	// ��]�p�̌v�Z
	void CaluclationAngle(DirectX::SimpleMath::Vector3 enemyPos);
	// Matrix�̌v�Z
	void CaluclationMatrix();
	// �ړ��̊Ǘ�
	void MovePlayer();

private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �p�x
	float m_angle;

	// �v���C���[�p�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;
};