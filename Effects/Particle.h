// -------------------------------------------
//
// �p�[�e�B�N�������s�F�`�悷��N���X
//
// -------------------------------------------
#pragma once

#include "pch.h"
#include <DeviceResources.h>
#include <list>
#include "Game/CommonResources.h"
#include "Effects/Header/DustTrailParticle.h"

class Particle
{
public:
	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

private:
	// �ϐ�
	DX::DeviceResources* m_pDR;
	// �o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	//	�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// �r���{�[�h�s��
	DirectX::SimpleMath::Matrix m_billboard;
	// ���_�J���[�e�N�X�`���[
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;

	// �J�����̍��W
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// �œ_
	DirectX::SimpleMath::Vector3 m_cameraTarget;

	// *******************************************************
	// �O�Ճ_�X�g
	std::list<DustTrialParticle> m_dustTrailParticle;
	// �O�Ճ_�X�g�̎���
	float m_timer_dustTrail;
	// ******************************************************

	// �v���C���[�̍��W
	DirectX::SimpleMath::Vector3 m_playerPosition;
	// �v���C���[�̑��x
	DirectX::SimpleMath::Vector3 m_playerVelocity;

public:
	// �֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	Particle();
	~Particle();

	// �e�N�X�`���̓ǂݍ���
	void LoadTexture(const wchar_t* path);
	// �p�[�e�B�N���̐������s��
	void Create();
	// �p�[�e�B�N���̍X�V����
	void Update(float elapsedTimer, const DirectX::SimpleMath::Vector3 playerPosition, const DirectX::SimpleMath::Vector3 playerVelocity);
	// �p�[�e�B�N���̕`�揈��
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// �r���{�[�h�s��̍쐬
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);

private:
	// �V�F�[�_�[�̍쐬
	void CreateShader();
	// �O�Ղ̓y���𐶐����鏈��
	void CreateTrailDust(float elapsedTimer);
};