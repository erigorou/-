#include "Framework/pch.h"
#include "Framework/SpriteString2D.h"
#include "Framework/Graphics.h"

// �t�H���g�T�C�Y
const float SpriteString2D::DEFAULT_FONT_SIZE = 1.0f;

// �R���X�g���N�^
SpriteString2D::SpriteString2D()
	:
	m_strings{}
{
}

// �f�X�g���N�^
SpriteString2D::~SpriteString2D()
{
	m_strings.clear();
}

// �`�悷�镶�����ǉ�����
void SpriteString2D::AddString(
	const wchar_t* string,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::FXMVECTOR& color,
	const float& size
)
{
	// �����������錾����
	InnerString innerString;
	// �������������
	innerString.m_string = std::wstring(string);
	// �ʒu��ݒ肷��
	innerString.m_position = position;
	// �F��ݒ肷��
	innerString.m_color = color;
	// �T�C�Y��ݒ肷��
	innerString.m_size = size;
	// ������z��ɕ������ǉ�����
	m_strings.push_back(innerString);
}

// ��������폜����
void SpriteString2D::RemoveString(const int& index)
{
	// �����񃊃X�g���當������폜����
	m_strings.erase(m_strings.begin() + index);
}

// ���ׂĂ̕�����z����폜����
void SpriteString2D::RemoveAllString()
{
	// �z����N���A����
	m_strings.clear();
}

// �`�悷��
void SpriteString2D::Render()
{
	// �X�v���C�g�o�b�`���擾����
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	// �t�H���g���擾����
	DirectX::SpriteFont* font = Graphics::GetInstance()->GetFont();
	// �X�v���C�g�o�b�`���J�n����
	spriteBatch->Begin();
	// �ǉ����ꂽ����������ԂɎ擾����
	for (size_t index = 0; index < m_strings.size(); index++)
	{
		// �������`�悷��
		font->DrawString(
			spriteBatch,
			m_strings[index].m_string.c_str(),
			m_strings[index].m_position,
			m_strings[index].m_color,
			0.0f,
			DirectX::SimpleMath::Vector2(0.0f, 0.0f),
			m_strings[index].m_size
		);
	}
	// �X�v���C�g�o�b�`���I������
	spriteBatch->End();
}
