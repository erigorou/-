#pragma once


// -------------------------------------------------------
// �C�x���g���X�g
// -------------------------------------------------------
enum class EventList
{
	// �v���C���[�ɍU�����ł��邩
	PlayerCanHit,
	// �{�X�ɍU�����ł��邩
	BossCanHit,
	// ���̃X�e�[�g��ύX����
	ChangeSwordState,
	// ���_�̃X�e�[�g��ύX����
	ChangeCudgelState,
	// �{�X�̃X�e�[�g��ύX����
	ChangeBossState,
	// �{�X�̊��ύX����
	ChangeBossFace,
	// �S�u������S�ē|��
	DeleteAllGoblin,
	// OBB�Փ˔����o�^����
	AddOBBCollision,
	// ���Փ˔����o�^����
	AddSphereCollision,
	// �Փ˔���̓o�^����������
	DeleteCollision,
	// �ړ��p�[�e�B�N���̍쐬
	CreateTrailDust,
	// �y���p�[�e�B�N���̍쐬
	CreateBashDust,
	// ����̋O�Ճp�[�e�B�N���̍쐬
	CreateWeaponTrail,
	// �J������h�炷
	ShakeCamera,
};



// -------------------------------------------------------
// �Q�b�^�[���X�g
// -------------------------------------------------------
enum class GetterList
{
	// �v���C���[�̃|�C���^���擾����
	GetPlayer,
	// �{�X�̃|�C���^���擾����
	GetBoss,
};