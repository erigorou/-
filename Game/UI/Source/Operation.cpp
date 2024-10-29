#include "pch.h"

#include "../Header/Operation.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Game/Player/PlayerHP.h"
#include "CommonStates.h"
#include "Game/Observer/Messenger.h"

// �Œ�l
const wchar_t* Operation::TEXT_PATH = L"Resources/Textures/UI/Operation/TEXT.png";

const wchar_t* Operation::X_PATH		= L"Resources/Textures/UI/Operation/X.png"		;
const wchar_t* Operation::SHIFT_PATH	= L"Resources/Textures/UI/Operation/Shift.png"	;
const wchar_t* Operation::UP_PATH		= L"Resources/Textures/UI/Operation/Up.png"		;
const wchar_t* Operation::DOWN_PATH		= L"Resources/Textures/UI/Operation/Down.png"	;
const wchar_t* Operation::LEFT_PATH		= L"Resources/Textures/UI/Operation/Left.png"	;
const wchar_t* Operation::RIGHT_PATH	= L"Resources/Textures/UI/Operation/Right.png"	;


const wchar_t* Operation::VS_PATH = L"Resources/Shaders/Operation/OperationVS.cso";
const wchar_t* Operation::PS_PATH = L"Resources/Shaders/Operation/OperationPS.cso";
const wchar_t* Operation::GS_PATH = L"Resources/Shaders/Operation/OperationGS.cso";


// �R���X�g���N�^
Operation::Operation()
	: m_pDR(nullptr)
	, m_customShader(nullptr)
	, m_CBuffer(nullptr)
	, m_states(nullptr)
	, m_batch(nullptr)
	, m_texture()
	, m_elapsedTime(0.0f)
	, m_totalTime(0.0f)
{
	// �f�o�C�X���\�[�X�̎擾
	m_pDR = CommonResources::GetInstance()->GetDeviceResources();
}


// �f�X�g���N�^
Operation::~Operation()
{
}


// ����������
void Operation::Initialize()
{
	std::unique_ptr<OperateUI> operateUI;

	// X�L�[�̐���
	operateUI = std::make_unique<OperateUI>(X_PATH);
	operateUI->Initialize();

	m_operateUIs.push_back(std::move(operateUI));
	Messenger::Attach(DirectX::Keyboard::Keys::X, m_operateUIs[0].get(), Messenger::KeyPressType::DOWN);

	// Shift�L�[�̐���
	operateUI = std::make_unique<OperateUI>(SHIFT_PATH);
	operateUI->Initialize();

	m_operateUIs.push_back(std::move(operateUI));
	Messenger::Attach(DirectX::Keyboard::Keys::LeftShift, m_operateUIs[1].get(), Messenger::KeyPressType::DOWN);

	// ��L�[�̐���
	operateUI = std::make_unique<OperateUI>(UP_PATH);
	operateUI->Initialize();

	m_operateUIs.push_back(std::move(operateUI));
	Messenger::Attach(DirectX::Keyboard::Keys::Up, m_operateUIs[2].get(), Messenger::KeyPressType::DOWN);

	// ���L�[�̐���
	operateUI = std::make_unique<OperateUI>(DOWN_PATH);
	operateUI->Initialize();

	m_operateUIs.push_back(std::move(operateUI));
	Messenger::Attach(DirectX::Keyboard::Keys::Down, m_operateUIs[3].get(), Messenger::KeyPressType::DOWN);

	// ���L�[�̐���
	operateUI = std::make_unique<OperateUI>(LEFT_PATH);
	operateUI->Initialize();

	m_operateUIs.push_back(std::move(operateUI));
	Messenger::Attach(DirectX::Keyboard::Keys::Left, m_operateUIs[4].get(), Messenger::KeyPressType::DOWN);

	// �E�L�[�̐���	
	operateUI = std::make_unique<OperateUI>(RIGHT_PATH);
	operateUI->Initialize();

	m_operateUIs.push_back(std::move(operateUI));
	Messenger::Attach(DirectX::Keyboard::Keys::Right, m_operateUIs[5].get(), Messenger::KeyPressType::DOWN);


	m_textUI = std::make_unique<UI>(TEXT_PATH);
	m_textUI->Initialize();

}


// �X�V����
void Operation::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ̉��Z
	m_totalTime += elapsedTime;
}


// �`�揈��
void Operation::Render()
{
	for(size_t i = 0; i < m_operateUIs.size(); i++)
	{ 
		m_operateUIs[i]->Render();
	}

	m_textUI->Render();
}


// �I������
void Operation::Finalize()
{
}
