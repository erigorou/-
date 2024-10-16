

#include "pch.h"
#include "Messenger.h"

// �L�[�{�[�h�ώ@�҃��X�g������������
std::vector<std::pair<DirectX::Keyboard::Keys, IObserver*>> Messenger::s_observerList;
// �L�[�͈̓��X�g(�L�[�A�J�n�C���f�b�N�X�ƏI���C���f�b�N�X)
std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> Messenger::s_keysRangeList;


// �ώ@�҂��A�^�b�`����
void Messenger::Attach(const DirectX::Keyboard::Keys& key, IObserver* observer)
{
    // �ώ@�҃��X�g�Ɋώ@�҂�ǉ�����
    s_observerList.emplace_back(key, observer);
}

// �ώ@�҂��f�^�b�`����
void Messenger::Detach(const DirectX::Keyboard::Keys& key, IObserver* observer)
{
    // �ώ@�҃��X�g����ώ@�҂���������
    s_observerList.erase(
        std::remove_if(s_observerList.begin(), s_observerList.end(),
            [key, observer](const std::pair<DirectX::Keyboard::Keys, IObserver*>& entry)
            {
                return entry.first == key && entry.second == observer;
            }),
        s_observerList.end());
}



// �ώ@�҂ɒʒm����
void Messenger::Notify(const DirectX::Keyboard::State& keyboardState)
{
    // �ώ@�҃��X�g����ώ@�҂����o��
    for (const auto& keyRange : s_keysRangeList)
    {
        // �ώ@�҂��������ׂ��L�[���ǂ����𒲂ׂ�
        if (keyboardState.IsKeyDown(keyRange.first))
        {
            // �L�[�̊J�n�C���f�b�N�X����I���C���f�b�N�X�܂ł̃C���f�b�N�X�����o��
            for (const auto& range : keyRange.second)
            {
                for (int i = range.first; i <= range.second; ++i)
                {
                    // �ώ@�҂̒ʒm�֐��ɉ���������ꂽ�L�[��ʒm����
                    s_observerList[i].second->OnKeyPressed(s_observerList[i].first);
                }
            }
        }
    }
}

// �ώ@�҃��X�g���\�[�g����
void Messenger::SortObserverList()
{
    // �ώ@�҃��X�g���\�[�g����
    std::sort(
        s_observerList.begin(),
        s_observerList.end(),
        [](const std::pair<DirectX::Keyboard::Keys, IObserver*>& a, const std::pair<DirectX::Keyboard::Keys, IObserver*>& b)
        {
            return a.first < b.first;
        });
}


// �L�[�͈̓��X�g�𐶐�����
void Messenger::CreateKeyRangeList()
{
    // �L�[�͈͂��N���A����
	if (s_keysRangeList.size()) s_keysRangeList.clear();
    // �J�n�C���f�b�N�X��ݒ肷��
	int startIndex = 0;
    // �ώ@�҃��X�g�̊J�n�L�[�����݂̃L�[�ɂ���
    DirectX::Keyboard::Keys currentKey = s_observerList[startIndex].first;
    for (int index = 1; index < s_observerList.size(); index++)
    {
        // �ώ@�҃��X�g�̌��݃L�[�ƌ��݂̃L�[���������Ȃ��ꍇ
        if (s_observerList[index].first != currentKey)
        {
            // �L�[�A�J�n�C���f�b�N�X�ƏI���C���f�b�N�X���L�[�͈̓��X�g�ɒǉ�����
            s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, index - 1));
            // ���݂̃C���f�b�N�X���J�n�C���f�b�N�X�ɐݒ肷��
            startIndex = index;
            // ���݂̊ώ@�҂̃L�[�����݂̃L�[�ɐݒ肷��
            currentKey = s_observerList[index].first;
        }
    }
    // �L�[�A�J�n�C���f�b�N�X�ƍŌ�̃C���f�b�N�X���L�[�͈̓��X�g�ɒǉ�����
    s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, s_observerList.size() - 1));
}