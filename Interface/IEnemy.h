// ----------------------------------
// �u���e�v�G�̃C���^�[�t�F�[�X
// �u����v�S�u�����@�{�X
// �u���t�v12/18
// �u�쐬�ҁv�r�c
// ----------------------------------

#include "pch.h"
#include "IObject.h"

class IEnemy : public IObject
{
	// �f�X�g���N�^
	virtual ~IEnemy() = default;
	// ����������
	virtual void Initialize(DirectX::Model* model) = 0;
	// �X�V����
	virtual void Update(const float& elapsedTime) = 0;
	// �`�揈��
	virtual void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection) = 0;
	// �I������
	virtual void Finalize() = 0;
};
