#pragma once
#include "pch.h"
#include "DeviceResources.h"

class QuestManager;
class CustomShader;
class DeviceResources;

class Tutorial
{
	// --------------------
	// �Œ�l
	// --------------------
public:
	// �e�N�X�`���T�C�Y�̑傫��
	static constexpr int WIDTH = 1000;
	static constexpr int HEIGHT = 100;

	// ��ʃT�C�Y�̑傫��
	static constexpr float WINDOW_WIDTH = 1280;
	static constexpr float WINDOW_HEIGHT = 720;

	// �e�N�X�`���̏������W
	static constexpr float INIT_POSITION_X = (WINDOW_WIDTH - WIDTH) / 2;
	static constexpr float INIT_POSITION_Y = 600.0f;

	// �A�j���[�V��������
	static constexpr float ANIMATION_TIME = 2.0f;

	// �V�F�[�_�[�̃t�@�C���p�X
	static const wchar_t* VS_PATH;
	static const wchar_t* PS_PATH;
	static const wchar_t* GS_PATH;

	// ���̓��C�A�E�g
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElements =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �萔�o�b�t�@
	struct CBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;	// �E�B���h�E�T�C�Y
		float							alpha;		// �����x
		DirectX::SimpleMath::Vector3	padding;	// ��
	};

	// --------------------
	// �A�N�Z�T
	// --------------------
public:
	void ChangeTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; m_canUseTimer = true; }	// �e�N�X�`���̕ύX
	bool GetTimerIsEnd() { return !m_timerPlay; }	// �^�C�}�[�t���O�̎擾

	// --------------------
	// ���J�֐�
	// --------------------
public:
	// �R���X�g���N�^
	Tutorial(QuestManager* manager);
	// �f�X�g���N�^
	~Tutorial();
	// ����������
	void Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Draw();
	// �I������
	void Finalize();

	void StartTimer();	// �^�C�}�[�̊J�n

	void StartAlphaMinus();

	// --------------------
	// �����֐�
	// --------------------
private:
	// �V�F�[�_�[�̐���
	void CreateShader();
	// �R���X�^���g�o�b�t�@�̍쐬
	void ConstantBuffer();
	// �萔�o�b�t�@�̍X�V����
	void UpdateConstantBuffer();
	// �`��ݒ�
	void SetRenderState();

	// �^�C�}�[�̍X�V����
	void UpdateTimer(float elapsedTime);
	// ���l�̍X�V����
	void UpdateAlpha(float elapsedTime);

	// --------------------
	// �����o�[�ϐ�
	// --------------------
private:

	QuestManager* m_questManager;	// �N�G�X�g�}�l�[�W���[

	DirectX::SimpleMath::Vector3	m_position;	// ���W
	float							m_angle;	// ��]�p
	DirectX::SimpleMath::Vector2	m_scale;	// �g�嗦
	float							m_alpha;	// �����x

	float	m_currentTime;	// �o�ߎ���
	float	m_elapsedTime;	// �o�ߎ���

	bool m_timerPlay;		// �^�C�}�[�t���O
	bool m_canUseTimer;		// �^�C�}�[�̎g�p�t���O
	bool m_alphaFlag;		// ���l�����炷

	// �V�F�[�_�[
	std::unique_ptr<CustomShader> m_shader;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �X�e�[�g
	std::unique_ptr<DirectX::CommonStates>	m_states;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};