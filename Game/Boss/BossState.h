#pragma once

enum class
	BossState
{
	Starting,		// �J�n���
	Idling,			// �ҋ@���
	Attacking,		// ���������U��
	Sweeping,		// �ガ�����U��
	DashAttacking,	// �ˌ����
	Approaching,	// �ǔ����
	Dead,			// ���S���
};