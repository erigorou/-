// �X�e�[�W�̋��E��
#pragma once

class Wall
{
public: // �Œ�l��󂯓n���p�֐�
	static const float WALL_SCALE;		// �傫���i�Œ�l�j

	// �R���X�g���N�^
	Wall();
	// �f�X�g���N�^
	~Wall();
	// ������
	void Initialize();
	// �`�揈��
	void Render(
		 ID3D11DeviceContext* context
		,DirectX::CommonStates* states
		,const DirectX::SimpleMath::Matrix& view
		,const DirectX::SimpleMath::Matrix& projection
	);
	// �I������
	void Finalize();

	// ���[���h�s��X�V����
	void UpdateWorldMatrix();

private:
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

};
