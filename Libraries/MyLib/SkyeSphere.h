#pragma once

#include "pch.h"

class SkyeSphere
{
public:
	// �R���X�g���N�^
	SkyeSphere();
	// �f�X�g���N�^
	~SkyeSphere();

	// �X�V����
	void Update();

	// ���f�����擾����
	void LoadSkyeSphereModel(ID3D11Device* device);

	// ���f���̕`��
	void DrawSkyeSphere(
		ID3D11DeviceContext1* context
		, DirectX::DX11::CommonStates* states
		, DirectX::SimpleMath::Matrix view
		, DirectX::SimpleMath::Matrix projection
	);


private:
	// SkyeSphere�̃��f��
	std::unique_ptr<DirectX::Model> m_skyeSphereModel;
};
