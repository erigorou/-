#pragma once
#ifndef SPRITE_STRING2D_DEFINED
#define SPRITE_STRING2D_DEFINED
#include <vector>
#include <string>

class SpriteString2D
{
public:
	// �t�H���g�T�C�Y
	static const float DEFAULT_FONT_SIZE;

private:
	// InnerString�N���X
	struct InnerString
	{
		// �ʒu
		DirectX::SimpleMath::Vector2 m_position;
		// ������
		std::wstring m_string;
		// �F
		DirectX::SimpleMath::Color m_color;
		// �T�C�Y
		float m_size;
	};
public:
	// �R���X�g���N�^
	SpriteString2D();
	// �f�X�g���N�^
	~SpriteString2D();
	// ������Z�q�폜
	void operator=(const SpriteString2D& object) = delete;
	// �R�s�[�R���X�g���N�^�폜
	SpriteString2D(const SpriteString2D& object) = delete;
	// �`�悷�镶�����ǉ�����
	void AddString(
		const wchar_t* m_string,
		const DirectX::SimpleMath::Vector2& m_position,
		const DirectX::FXMVECTOR& m_color = DirectX::Colors::White,
		const float& m_size = DEFAULT_FONT_SIZE
	);
	// ��������폜����
	void RemoveString(const int& index);
	// ���ׂĂ̕�����z����폜����
	void RemoveAllString();
	// �`�悷��
	void Render();

private:
	// �\��������̔z��
	std::vector<InnerString> m_strings;
};

#endif	// SPRITE_STRING2D_DEFINED