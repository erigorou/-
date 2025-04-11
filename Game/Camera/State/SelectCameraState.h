// ---------------------------------------------
// 
// ���O		:SelectCameraState.h
// ���e		:�X�e�[�W�I�����̃J�����X�e�[�g�N���X
// �쐬		:�r�c����
// 
// ---------------------------------------------
#pragma once
// �C���N���[�h
#include "pch.h"
#include "Interface/ICameraState.h"

// �O���錾
class Camera;

class SelectCameraState : public ICameraState
{
	// ----------------------------
	// �萔
	// ----------------------------
private:
	// sin�g�̎���
	static constexpr float SINE_PERIOD = 4.0f;
	// �㉺��
	static constexpr float SINE_HEIGHT = 1.0f;
	// ��ԗ�
	static constexpr float LERP_RATE = 0.075f;

	// ----------------------------
	// �����o�֐�
	// ----------------------------
public:
	// �R���X�g���N�^
	SelectCameraState(Camera* camera);
	// �f�X�g���N�^
	~SelectCameraState()override;
	// �X�e�[�g�ύX�iin�j
	void PreUpdate()override;
	// �X�V����
	void Update(
		const DirectX::SimpleMath::Vector3& playerPos,
		const DirectX::SimpleMath::Vector3& enemyPos,
		float elapsedTime
	)override;
	// �X�e�[�g�ύX�iout�j
	void PostUpdate()override;

	// ----------------------------
	// �����o�ϐ�
	// ----------------------------
private:
	// �J����
	Camera* m_camera;
	// �J������Y���W
	float m_cameraY;
	// �o�ߎ���
	float m_elapsedTime;
};