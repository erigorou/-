#pragma once
#include "pch.h"
#include "QuestRenderer.h"
#include "DeviceResources.h"

class QuestManager;
class CustomShader;
class DeviceResources;

//�X���C�h�A�N�V�����̗񋓐�
enum SLIDE_ACTION : int
{
	WAIT = 0,
	SLIDE_OUT,
	COOL_TIME,
	SLIDE_IN
};

class QuestRenderer
{
	// --------------------
	// �Œ�l
	// --------------------
public:
	// �e�N�X�`���T�C�Y�̑傫��
	static constexpr int WIDTH = 400;
	static constexpr int HEIGHT = 120;

	// �e�N�X�`���̏������W
	static constexpr int INIT_POSITION_X = 1280;
	static constexpr int INIT_POSITION_Y = 300;

	// ��ʃT�C�Y�̑傫��
	static constexpr float WINDOW_WIDTH = 1280;
	static constexpr float WINDOW_HEIGHT = 720;

	// �e�N�X�`���̍ŏI�ʒu
	static constexpr float FINAL_POSITION_WIDTH = WINDOW_WIDTH - WIDTH;

	// �A�j���[�V��������
	static constexpr float ANIMATION_TIME = 1.0f;
	static constexpr float FIER_TIME = 2.0f;
	static constexpr float INITIAL_TIME = -0.5f;
	static constexpr float ANIMATION_WAIT = 0.5f;

	// �����x
	static constexpr float MAX_ALPHA = 1.0f;

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
		float 							dissolve;   // �f�B�]���u
		DirectX::SimpleMath::Vector2	padding;	// ��
	};

	// --------------------
	// �A�N�Z�T
	// --------------------
public:
	void ChangeTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; }	// �e�N�X�`���̕ύX

	void IsClear(bool flag) { m_clearFlag = flag; }	// �N���A�t���O�̕ύX
	bool CanChangeQuest() { return m_canChanegQuest; }	// �N�G�X�g�̕ύX���\��

	// --------------------
	// ���J�֐�
	// --------------------
public:
	// �R���X�g���N�^
	QuestRenderer(QuestManager* manager);
	// �f�X�g���N�^
	~QuestRenderer();
	// ����������
	void Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Draw();
	// �I������
	void Finalize();

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

	// �摜���X���C�h�����Ȃ�
	void NoSlideTexture();
	// �摜���X���C�h�A�E�g������
	void SlideOutTexture();
	// �N�[���^�C��
	void SlideCoolTime();
	// �摜���X���C�h�C��������
	void SlideInTexture();

	// �X���C�h�A�N�V�����̒ǉ�
	void AddSlideAction();

	// --------------------
	// �����o�[�ϐ�
	// --------------------
private:

	// �N�G�X�g�}�l�[�W���[
	QuestManager* m_questManager;

	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ��]�p
	float m_angle;
	// �g�嗦
	DirectX::SimpleMath::Vector2 m_scale;
	// �����x
	float m_alpha;
	// �f�B�]���u
	float m_dissolve;

	float	m_currentTime;	// �o�ߎ���
	float	m_elapsedTime;	// �o�ߎ���

	bool m_clearFlag;		// �N�G�X�g�̃N���A�t���O
	bool m_canChanegQuest;	// �N�G�X�g�̕ύX���\��

	std::vector<std::function<void()>> m_slideActions;	// �X���C�h�A�N�V����
	std::function<void()> m_currentAction; // ���݂̃A�N�V����

	// �V�F�[�_�[
	std::unique_ptr<CustomShader> m_shader;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �f�B�]���u�p�^�[���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dissolveTexture;
	// �X�e�[�g
	std::unique_ptr<DirectX::CommonStates>	m_states;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
};