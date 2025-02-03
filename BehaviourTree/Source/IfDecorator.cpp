#include "pch.h"

#include "BehaviourTree/Header/IfDecorator.h"
#include <iostream>

// �������m�F���ăA�N�V���������s����
bool IfDecorator::execute()
{
	// �����ł̓f���Ƃ���臒l��200�Ɖ��肷��
	int possession = 200;

	// h�̃R���X�g���N�^��threshold�ɏ����l�����Ă���
	if (possession >= threshold)
	{
		return action->execute();
	}
	std::cout << "�����𖞂����Ă��܂���: " << threshold << std::endl;
	return false;
}