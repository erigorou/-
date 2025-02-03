#include "pch.h"

#include "BehaviourTree/Header/Sequence.h"

/// <summary>
/// �V�[�P���X�ɃA�N�V������ǉ�����֐�
/// </summary>
/// <param name="action">�ǉ��������A�N�V�����̃N���X</param>

void Sequence::addAction(Action* action)
{
	// �A�N�V������ǉ�����
	actions.push_back(action);
}

/// <summary>
/// �V�[�P���X���̃A�N�V�������������s����֐�
/// </summary>
/// <returns>���� or ���s</returns>

bool Sequence::execute()
{
	// �A�N�V���������ׂĎ��s����
	for (Action* action : actions)
	{
		if (!action->execute())
		{
			// �A�N�V�������������ĂȂ��Ȃ�F���s
			return false;
		}
	}
	// �F����
	return true;
}