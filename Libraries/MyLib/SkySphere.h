#pragma once

#include "pch.h"

class SkySphere
{
public:
	// �Œ�l
	static const float SKYSPHERE_SCALE;

public:
	// �R���X�g���N�^
	SkySphere();
	// �f�X�g���N�^
	~SkySphere();

	// �X�V����
	void Update();

	// ���f�����擾����
	void LoadSkySphereModel(ID3D11Device* device);

	// ���f���̕`��
	void DrawSkySphere(
		ID3D11DeviceContext1* context
		, DirectX::DX11::CommonStates* states
		, DirectX::SimpleMath::Matrix view
		, DirectX::SimpleMath::Matrix projection
	);


private:
	// SkyeSphere�̃��f��
	std::unique_ptr<DirectX::Model> m_skySphereModel;
};
