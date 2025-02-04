#pragma once

// -------------------------------------------------------
// �C�x���g���X�g
// -------------------------------------------------------
enum class EventList
{
	// �v���C���[�ɍU�����ł��邩
	PlayerCanHit,
	// �G�ɍU�����ł��邩
	EnemyCanHit,
	// ���̃X�e�[�g��ύX����
	ChangeSwordState,
	// ���_�̃X�e�[�g��ύX����
	ChangeCudgelState,
	// �{�X�̃X�e�[�g��ύX����
	ChangeBossState,
	// �{�X�̊��ύX����
	ChangeBossFace,
	// �G���P�̍폜
	DeleteEnemy,
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
	// �v���C�V�[���I��
	EndPlayScene
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
	// ���̃|�C���^���擾����
	GetSword,
	// ���_�̃|�C���^���擾����
	GetCudgel,
};