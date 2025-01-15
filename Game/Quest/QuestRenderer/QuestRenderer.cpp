#include "pch.h"
#include "QuestRenderer.h"
#include "../QuestManager.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/UI/UIAnchor.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "CommonStates.h"


// --------------------
// �Œ�l
// --------------------

const wchar_t* QuestRenderer::VS_PATH = L"Resources/Shaders/Quest/Quest_VS.cso";
const wchar_t* QuestRenderer::PS_PATH = L"Resources/Shaders/Quest/Quest_PS.cso";
const wchar_t* QuestRenderer::GS_PATH = L"Resources/Shaders/Quest/Quest_GS.cso";


// --------------------
// �R���X�g���N�^
// --------------------
QuestRenderer::QuestRenderer(QuestManager* manager)
	: m_questManager(manager)
	, m_position(INIT_POSITION_X, INIT_POSITION_Y, 0)
	, m_angle()
	, m_scale(1.0f, 1.0f)
	, m_alpha(0.0f)
	, m_currentTime()
	, m_canChanegQuest(false)
	, m_clearFlag(false)
{
}


// --------------------
// �f�X�g���N�^
// --------------------
QuestRenderer::~QuestRenderer()
{
}


// --------------------
// ����������
// --------------------
void QuestRenderer::Initialize(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// �V�F�[�_�[�̐���
	CreateShader();
	// �R���X�^���g�o�b�t�@�̍쐬
	ConstantBuffer();
	
	m_texture = texture;

	// �R�����X�e�[�g�̐���
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// �v���~�e�B�u�o�b�`�̐���
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);


	// �X���C�h�A�N�V�����̒ǉ�
	AddSlideAction();
}


// --------------------
// �X�V����
// --------------------
void QuestRenderer::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ̕ۑ�
	m_elapsedTime = elapsedTime;

	// �X���C�h�̍X�V����
	m_currentAction();
}


// --------------------
// �`�揈��
// --------------------
void QuestRenderer::Draw()
{
	// �R���e�L�X�g�̎擾
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// ���_���̐ݒ�
	DirectX::VertexPositionColorTexture vertex[1] = 
	{
		DirectX::VertexPositionColorTexture
		(
			DirectX::SimpleMath::Vector3(m_scale.x, m_scale.y, static_cast<float>(ANCHOR::TOP_LEFT))
		,	DirectX::SimpleMath::Vector4(m_position.x, m_position.y, WIDTH, HEIGHT)
		,	DirectX::SimpleMath::Vector2(WINDOW_WIDTH, WINDOW_HEIGHT)
		)
	};

	// �萔�o�b�t�@�̐ݒ�
	UpdateConstantBuffer();

	// �摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// �����_�[�X�e�[�g�̐ݒ�
	SetRenderState();

	// �V�F�[�_�[�J�n
	m_shader->BeginSharder(context);

	// �e�N�X�`���̐ݒ�
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());


	//	�|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	// �V�F�[�_�[�I��
	m_shader->EndSharder(context);
}



// --------------------
// �I������
// --------------------
void QuestRenderer::Finalize()
{
}


// --------------------
// �V�F�[�_�[�̐���
// --------------------
void QuestRenderer::CreateShader()
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// �V�F�[�_�[�̐���
	m_shader = std::make_unique<CustomShader>
		(
			device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			InputElements
		);
}


// --------------------
// �R���X�^���g�o�b�t�@�̍쐬
// --------------------
void QuestRenderer::ConstantBuffer()
{
	// �f�o�C�X�̎擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"�R���X�^���g�o�b�t�@�̐����Ɏ��s���܂���.", NULL, MB_OK);
	}
}


// --------------------
// �萔�o�b�t�@�̍X�V����
// --------------------
void QuestRenderer::UpdateConstantBuffer()
{
	// �R���e�L�X�g�̎擾
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// �萔�o�b�t�@�̐ݒ�
	CBuffer cbuff;
	cbuff.windowSize = DirectX::SimpleMath::Vector4(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f);
	cbuff.alpha = m_alpha;
	cbuff.padding = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

	// �󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
}


// --------------------
// �����_�[�X�e�[�g�̐ݒ�
// --------------------
void QuestRenderer::SetRenderState()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied(); 	//	�������`��w��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		// ��������
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);		// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ��Ȃ�
	context->RSSetState(m_states->CullClockwise());					// �J�����O�͍����
}




// --------------------------------
// �X���C�h�A�N�V�����̒ǉ�
// --------------------------------
void QuestRenderer::AddSlideAction()
{
	// �A�N�V�����̒ǉ�
	m_slideActions.push_back([this]() { return NoSlideTexture(); }); // �ҋ@
	m_slideActions.push_back([this]() { return SlideOutTexture(); }); // �X���C�h�A�E�g
	m_slideActions.push_back([this]() { return SlideCoolTime();	 }); // �N�[���^�C��
	m_slideActions.push_back([this]() { return SlideInTexture(); }); // �X���C�h�C��

	// ���݂̃A�N�V������ݒ�
	m_currentAction = m_slideActions[SLIDE_ACTION::SLIDE_IN];

	// �o�ߎ��Ԃ�-����n�߂邱�ƂŒx����ݒ�
	m_currentTime = INITIAL_TIME;
}



// --------------------------------
// �摜���X���C�h�����Ȃ�
// --------------------------------
void QuestRenderer::NoSlideTexture()
{
	// �}�l�[�W���[���N�G�X�g�̕ύX�������Ă��邩
	if (m_clearFlag)
	{
		// �N�G�X�g�̕ύX
		m_currentAction = m_slideActions[SLIDE_ACTION::SLIDE_OUT];
	}
}



// --------------------------------
// �e�N�X�`�����T�C�h�A�E�g����
// --------------------------------
void QuestRenderer::SlideOutTexture()
{
	// �o�ߎ��Ԃ̍X�V
	m_currentTime += m_elapsedTime;

	// ���K���������Ԃ̎擾
	float t = m_currentTime / ANIMATION_TIME;

	// �����x�̐ݒ�
	m_alpha = MAX_ALPHA - t;

	// �X���C�h�C������
	m_position.x = FINAL_POSITION_WIDTH + WIDTH * Easing::easeInExpo(t);

	// �A�j���[�V�������Ԃ��o�߂����ꍇ�A�I��
	if (m_currentTime > ANIMATION_TIME)
	{
		// �X���C�h�A�E�g�I��
		m_currentTime = 0.0f;
		m_currentAction = m_slideActions[SLIDE_ACTION::COOL_TIME];

		// �N���A�t���O��܂�
		m_clearFlag = false;

		// �N�G�X�g���P�i�߂�
		m_questManager->ChangeNextQuest();
	}

}


// --------------------------------
// �X���C�h�̃N�[���^�C��
// --------------------------------
void QuestRenderer::SlideCoolTime()
{
	// �N�[���^�C���̍X�V
	m_currentTime += m_elapsedTime;

	// �N�[���^�C���̏I��
	if (m_currentTime > ANIMATION_WAIT)
	{
		m_currentTime = 0.0f;
		m_currentAction = m_slideActions[SLIDE_ACTION::SLIDE_IN];
	}
}


// --------------------------------
// �e�N�X�`�����T�C�h�C������
// --------------------------------
void QuestRenderer::SlideInTexture()
{
	// �o�ߎ��Ԃ̍X�V
	m_currentTime += m_elapsedTime;

	// ���K���������Ԃ̎擾
	float t = m_currentTime / ANIMATION_TIME;

	// alpha�l�̐ݒ�
	m_alpha = t;

	// �X���C�h����
	m_position.x = WINDOW_WIDTH - WIDTH * Easing::easeOutExpo(t);

	// �A�j���[�V�������Ԃ��o�߂����ꍇ�A�I��
	if (m_currentTime > ANIMATION_TIME)
	{
		// �X���C�h�C���I��
		m_currentTime = 0.0f;
		m_currentAction = m_slideActions[SLIDE_ACTION::WAIT];
	}
}