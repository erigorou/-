#include "pch.h"

#include "Operation.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "CommonStates.h"
#include "Game/Messenger/KeyboardMessenger.h"

// �Œ�l
const wchar_t* Operation::TEXT_PATH = L"Resources/Textures/UI/Operation/TEXT.png";


// ------------------------------------
// �R���X�g���N�^
// ------------------------------------
Operation::Operation()
	:
	m_customShader(nullptr),
	m_CBuffer(nullptr),
	m_states(nullptr),
	m_batch(nullptr),
	m_texture{},
	m_elapsedTime{},
	m_totalTime{}
{
}

// ------------------------------------
// �f�X�g���N�^
// ------------------------------------
Operation::~Operation()
{
}

// ------------------------------------
// ����������
// ------------------------------------
void Operation::Initialize()
{

	// �e�L�[�ɑ΂��đ���UI�𐶐����A���������ă��X�g�ɒǉ�
	for (const auto& key : keys)
	{
		auto operateUI = std::make_unique<OperateUI>(key);
		operateUI->Initialize();
		m_operateUIs.push_back(std::move(operateUI));
	}

	// �L�[�{�[�h�C�x���g�Ɏg�p�L�[��o�^
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::X, m_operateUIs[0].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::LeftShift, m_operateUIs[1].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Up, m_operateUIs[2].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Down, m_operateUIs[3].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Left, m_operateUIs[4].get(), KeyboardMessenger::KeyPressType::DOWN);
	KeyboardMessenger::Attach(DirectX::Keyboard::Keys::Right, m_operateUIs[5].get(), KeyboardMessenger::KeyPressType::DOWN);

	// �e�L�X�gUI�̏�����
	m_textUI = std::make_unique<UI>(TEXT_PATH);
	m_textUI->Initialize();
}

// ------------------------------------
// �X�V����
// ------------------------------------
void Operation::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ̉��Z
	m_totalTime += elapsedTime;
}

// ------------------------------------
// �`�揈��
// ------------------------------------
void Operation::Render()
{
	for (size_t i = 0; i < m_operateUIs.size(); i++)
	{
		m_operateUIs[i]->Render();
	}

	// �e�L�X�gUI�̕`��
	m_textUI->Render();
}

// ------------------------------------
// �I������
// ------------------------------------
void Operation::Finalize()
{
}