#pragma once
#ifndef MESSENGER
#define MESSENGER

#include "Interface/IObserver.h"
#include <unordered_map>

class Messenger
{
public:
	// �ώ@�҂��A�^�b�`����
	static void Attach(const DirectX::Keyboard::Keys& key, IObserver* observer);

	// �ώ@�҂��f�^�b�`����
	static void Detach(const DirectX::Keyboard::Keys& key, IObserver* observer);

	// �ʒm����
	static void Notify(const DirectX::Keyboard::State& keyboardState);
	
	// �ώ@�҃��X�g���\�[�g����
	static void SortObserverList();

	// �L�[�͈̓��X�g�𐶐�����
	static void CreateKeyRangeList();

private:
	// �L�[�{�[�h�ώ@�҃��X�g
	static std::vector<std::pair<DirectX::Keyboard::Keys, IObserver*>> s_observerList;
	// �L�[�͈̓��X�g(�J�n�C���f�b�N�X�ƏI���C���f�b�N�X)
	static std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> s_keysRangeList;


};

#endif // MESSENGER

