#pragma once
#ifndef MESSENGER_DEFINED
#define MESSENGER_DEFINED

#include "Interface/IObserver.h"
#include <unordered_map>

class Messenger
{
public:
	// �ʒm�̎d��
	enum class KeyPressType : UINT
	{
		DOWN,
		PRESSED
	};


	// �z��̓��e
	enum class ArrayContentType : int
	{
		KEYBOARD,
		P_OBSERVER,
		TYPE
	};

	// �ώ@�҂��A�^�b�`����
	static void Attach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type);

	// �ώ@�҂��f�^�b�`����
	static void Detach(const DirectX::Keyboard::Keys& key, IObserver* observer, const KeyPressType type);

	// �ʒm����
	static void Notify(const DirectX::Keyboard::State& keyboardState);
	static void Notify(const DirectX::Keyboard::KeyboardStateTracker& keyboardTracker);


	// �ώ@�҃��X�g���\�[�g����
	static void SortObserverList();

	// �L�[�͈̓��X�g�𐶐�����
	static void CreateKeyRangeList();

	// �ώ@�҃��X�g�ƃL�[�͈̓��X�g���N���A����
	static void Clear();

private:
	// �L�[�{�[�h�ώ@�҃��X�g
	static std::vector<std::tuple<DirectX::Keyboard::Keys, IObserver*, KeyPressType>> s_observerList;
	// �L�[�͈̓��X�g(�J�n�C���f�b�N�X�ƏI���C���f�b�N�X)
	static std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> s_keysRangeList;
};

#endif // MESSENGER_DEFINED

