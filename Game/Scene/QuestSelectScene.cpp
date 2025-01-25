/*
	@file	QuestSelectScene.cpp
	@brief	�^�C�g���V�[���N���X
*/
#include "pch.h"
#include "QuestSelectScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/DebugString.h"
#include "../Factory/Factory.h"
#include "../Sound/Sound.h"
#include "../Data/GameData.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Libraries/MyLib/Math.h"
#include "Effects/Particle.h"
#include "Game/UI/UIAnchor.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"

#include "../Camera/Camera.h"
#include "../Stage/Floor/Floor.h"
#include "../Stage/Sea/Sea.h"
#include "../TitleObject/TitleEnemy.h"
#include "Libraries/MyLib/SkySphere.h"

#include <cassert>

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
QuestSelectScene::QuestSelectScene()
	:
	m_totalSeconds{},
	m_spriteBatch{},
	m_texture{},
	m_texCenter{},
	m_isChangeScene{},
	m_selectIndex{}
{
	m_commonResources = CommonResources::GetInstance();
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
QuestSelectScene::~QuestSelectScene()
{
	// do nothing.
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void QuestSelectScene::Initialize()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �X�v���C�g�o�b�`���쐬����
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// �e�N�X�`�������[�h����
	LoadTextures();

	// �e�N�X�`���̒��S���W���v�Z����
	CalculateTextureCenters();

	// �I�u�W�F�N�g�𐶐�����
	CreateObjects();

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	// ���y�̕ύX
	Sound::ChangeBGM(Sound::BGM_TYPE::TITLE);
}


//---------------------------------------------------------
// ���[�h����
//---------------------------------------------------------
void QuestSelectScene::LoadTextures()
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// �e�N�X�`�������u������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex;

	// �摜�����[�h����
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/LOGO.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SPACE�ŃX�^�[�g.png",
			nullptr,
			m_texture2.ReleaseAndGetAddressOf()
		)
	);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Quest/TitleTutorial.png",
			nullptr,
			tex.ReleaseAndGetAddressOf()
		)
	);

	m_textureList.push_back(tex);


	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Quest/TitleBossFight.png",
			nullptr,
			tex.ReleaseAndGetAddressOf()
		)
	);
	
	m_textureList.push_back(tex);
	
}


//---------------------------------------------------------
// �e�N�X�`���̒��S���擾����
//---------------------------------------------------------
void QuestSelectScene::CalculateTextureCenters()
{
	CalculateTextureCenter(m_texture		, m_texCenter1);
	CalculateTextureCenter(m_texture2		, m_texCenter2);
	CalculateTextureCenter(m_textureList[0]	, m_texCenter3);
	CalculateTextureCenter(m_textureList[1]	, m_texCenter4);


	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// �ˉe�s����쐬����
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(40.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100000.0f
	);
}

// ---------------------------------------------------------
// �e�N�X�`���̒��S���擾����
// ---------------------------------------------------------
void QuestSelectScene::CalculateTextureCenter(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, DirectX::SimpleMath::Vector2& texCenter)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
	D3D11_TEXTURE2D_DESC desc = {};

	// �e�N�X�`���̏����擾
	texture->GetResource(resource.GetAddressOf());
	resource.As(&tex2D);
	tex2D->GetDesc(&desc);

	// �e�N�X�`���̒��S���v�Z
	DirectX::SimpleMath::Vector2 texSize;
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	texCenter = texSize / 2.0f;
}


//---------------------------------------------------------
// �I�u�W�F�N�g�̐���
//---------------------------------------------------------
void QuestSelectScene::CreateObjects()
{
	m_camera	= Factory::CreateCamera	();
	m_floor		= Factory::CreateFloor	();
	m_sea		= Factory::CreateSea();
	m_skySphere = Factory::CreateSkySphere();

	m_enemy = std::make_unique<TitleEnemy>();
	m_enemy->Initialize();
	m_skySphere->LoadSkySphereModel();

	// �^�C�g���V�[���̃J�����X�e�[�g��ݒ�
	m_camera->ChangeState(m_camera->GetTitleState());

	m_particle = Factory::CreateParticle();
}


//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void QuestSelectScene::Update(float elapsedTime)
{
	m_totalSeconds += elapsedTime;

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	DirectX::SimpleMath::Vector3 zeroV = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Matrix zeroM = DirectX::SimpleMath::Matrix::Identity;

	// �I�u�W�F�N�g�̍X�V
	m_camera->Shake(elapsedTime);
	m_camera->Update(zeroV, zeroV, zeroM, elapsedTime);
	m_enemy->Update(elapsedTime);

	// �p�[�e�B�N���̍X�V
	m_particle->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);

	// �X�e�[�W�I��
	SelectStage(kbTracker.get());
}



void QuestSelectScene::SelectStage(DirectX::Keyboard::KeyboardStateTracker* keyboard)
{
	if (m_isChangeScene) return;

	// �X�y�[�X�L�[�������ꂽ��
	if (keyboard->pressed.Space)
	{
		if (!m_isChangeScene)
		{
			// �Q�[���f�[�^�ɃX�e�[�W��ݒ�
			auto gameData = GameData::GetInstance();
			gameData->SetSelectStage(m_selectIndex);

			// �V�[���ύX�t���O�𗧂Ă�̂Ɖ��y��炷
			m_isChangeScene = true;
			Sound::PlaySE(Sound::SE_TYPE::SYSTEM_OK);
		}
	}

	// �X�e�[�W�I��
	if (keyboard->pressed.Up	)	m_selectIndex = Math::Clamp(m_selectIndex - 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);
	if (keyboard->pressed.Down	)	m_selectIndex = Math::Clamp(m_selectIndex + 1, MIN_STAGE_INDEX, MAX_STAGE_INDEX);
}



//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void QuestSelectScene::Render()
{
	auto states		= m_commonResources	->	GetCommonStates		();
	auto view		= m_camera			->	GetViewMatrix		();

	// �I�u�W�F�N�g�̕`��
	m_floor->		Render(view, m_projection);
	m_sea->			Render(view, m_projection);
	m_enemy->		Render(view, m_projection);
	m_skySphere->	DrawSkySphere(view, m_projection);

	// �X�v���C�g�o�b�`�̊J�n�F�I�v�V�����Ń\�[�g���[�h�A�u�����h�X�e�[�g���w�肷��
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());

	// �e�N�X�`���̕`��
	DrawTexture();

	// �p�[�e�B�N���̕`��
	m_particle->CreateBillboard(m_camera->GetEyePosition(),DirectX::SimpleMath::Vector3::Zero,m_camera->GetUpVector());
	m_particle->Render(view, m_projection);

	DrawStageSelect();

	// �X�v���C�g�o�b�`�̏I���
	m_spriteBatch->End();

#ifdef _DEBUG
#endif // _DEBUG
}


//---------------------------------------------------------
// �e�N�X�`���̕`��
//---------------------------------------------------------
void QuestSelectScene::DrawTexture()
{
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// �摜�̒��S���v�Z����
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	Vector2 titlePos = pos;
	titlePos.y += TITLE_LOGO_CENTER_Y;

	// �^�C�g�����S�̏㉺�ړ��̑���
	float moveValue = titlePos.y * 2;

	// �b���𐳋K��
	float t = Math::Clamp(m_totalSeconds - DELAY, 0.0f, ANIM_END) / ANIM_END;


	// �^�C�g�����S�̕`��ʒu�����߂�i�ړ����l���j
	Vector2 logoPos
	{
		titlePos.x ,
		TITLE_LOGO_CENTER_Y + moveValue * Easing::easeOutElastic(t)
	};

	m_spriteBatch->Draw(
		m_texture.Get(),			// �e�N�X�`��(SRV)
		logoPos,					// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,					// ��`(RECT)
		DirectX::Colors::White,		// �w�i�F
		0.0f,						// ��]�p(���W�A��)
		m_texCenter1,				// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,						// �X�P�[��(scale)
		DirectX::SpriteEffects_None,// �G�t�F�N�g(effects)
		0.0f						// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);

	// SPACE�ŃX�^�[�g.png �̕`��ʒu�𒲐�
	DirectX::SimpleMath::Vector2 pos2 = DirectX::SimpleMath::Vector2(pos.x , pos.y + 325.0f);

	// SPACE�ŃX�^�[�g.png �𒆉��ɕ`�悷��
	m_spriteBatch->Draw(
		m_texture2.Get(),			// �e�N�X�`��(SRV)
		pos2,						// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,					// ��`(RECT)
		DirectX::Colors::White,		// �w�i�F
		0.0f,						// ��]�p(���W�A��)
		m_texCenter2,				// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,						// �X�P�[��(scale)
		DirectX::SpriteEffects_None, // �G�t�F�N�g(effects)
		0.0f						// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);
}


void QuestSelectScene::DrawStageSelect()
{
	// �X�e�[�W�ɉ������F���̐ݒ�
	float colorRGB1 = 1.0f - 0.5f * m_selectIndex;
	float colorRGB2 = 0.5f + 0.5f * m_selectIndex;

	DirectX::SimpleMath::Vector4 color1 = DirectX::SimpleMath::Vector4(colorRGB1, colorRGB1, colorRGB1, 1.0f);
	DirectX::SimpleMath::Vector4 color2 = DirectX::SimpleMath::Vector4(colorRGB2, colorRGB2, colorRGB2, 1.0f);

	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };

	// �摜�̒��S���v�Z����
	Vector2 pos{ rect.right / 2.0f, rect.bottom / 2.0f };
	Vector2 titlePos = pos;

	// �ړ���;
	float moveValue = m_texCenter3.x * 3;

	// �b���𐳋K��
	float t = Math::Clamp(m_totalSeconds - STAGE_SELECT_DELAY, 0.0f, STAGE_SELECT_END) / STAGE_SELECT_END;


	// �^�C�g�����S�̕`��ʒu�����߂�i�ړ����l���j
	Vector2 tutorialPos
	{
		WINDOW_WIDTH + m_texCenter3.x * 2.0f - moveValue * Easing::easeOutBack(t),
		450.0f
	};


	// �`���[�g���A����`��
	m_spriteBatch->Draw(
		m_textureList[0].Get(),		// �e�N�X�`��(SRV)
		tutorialPos,				// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,					// ��`(RECT)
		color1,						// �w�i�F
		0.0f,						// ��]�p(���W�A��)
		m_texCenter3,				// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,						// �X�P�[��(scale)
		DirectX::SpriteEffects_None,// �G�t�F�N�g(effects)
		0.0f						// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);


	float t2 = Math::Clamp(m_totalSeconds - STAGE_SELECT_DELAY - 0.2f, 0.0f, STAGE_SELECT_END) / STAGE_SELECT_END;

	// �{���`��
	Vector2 BossFightPos
	{
		WINDOW_WIDTH + m_texCenter4.x * 2.0f - moveValue * Easing::easeOutBack(t2),
		575.0f
	};



	// SPACE�ŃX�^�[�g.png �𒆉��ɕ`�悷��
	m_spriteBatch->Draw(
		m_textureList[1].Get(),		// �e�N�X�`��(SRV)
		BossFightPos,				// �X�N���[���̕\���ʒu(origin�̕`��ʒu)
		nullptr,					// ��`(RECT)
		color2,						// �w�i�F
		0.0f,						// ��]�p(���W�A��)
		m_texCenter4,				// �e�N�X�`���̊�ɂȂ�\���ʒu(�`�撆�S)(origin)
		1.0f,						// �X�P�[��(scale)
		DirectX::SpriteEffects_None,// �G�t�F�N�g(effects)
		0.0f						// ���C���[�x(�摜�̃\�[�g�ŕK�v)(layerDepth)
	);
}



//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void QuestSelectScene::Finalize()
{
}


//---------------------------------------------------------
// ���̃V�[��ID���擾����
//---------------------------------------------------------
IScene::SceneID QuestSelectScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}


//---------------------------------------------------------
// �J������h�炷
//---------------------------------------------------------
void QuestSelectScene::SetShakeCamera()
{
 	m_camera->SetShake(1.0f);
}


//---------------------------------------------------------
// �V�F�[�_�[�̐���
//---------------------------------------------------------
void QuestSelectScene::CreateShader(const wchar_t* VS, const wchar_t* PS, const wchar_t* GS)
{
	// �f�o�C�X�̎擾
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// �V�F�[�_�[�̐���
	m_shaderList.push_back(std::make_unique<CustomShader>
		(
			device,
			VS,
			PS,
			GS,
			InputElements
		)
	);
}


//---------------------------------------------------------
// �萔�o�b�t�@�̍쐬
//---------------------------------------------------------
void QuestSelectScene::CreateConstantBuffer()
{
	// �f�o�C�X�̎擾
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(CBuffer);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	DX::ThrowIfFailed(
		device->CreateBuffer(&cbDesc, nullptr, m_CBuffer.GetAddressOf())
	);
}


// --------------------
// �����_�[�X�e�[�g�̐ݒ�
// --------------------
void QuestSelectScene::SetRenderState()
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	ID3D11BlendState* blendstate = states->NonPremultiplied(); 	//	�������`��w��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		// ��������
	context->OMSetDepthStencilState(states->DepthNone(), 0);		// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ��Ȃ�
	context->RSSetState(states->CullClockwise());					// �J�����O�͍����
}



void QuestSelectScene::CleateSpamDust(DirectX::SimpleMath::Vector3 pos)
{
	m_particle->CreateSlamDust(pos);
}
