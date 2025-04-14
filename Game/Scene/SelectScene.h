// --------------------------------------------------
// 
// ���O:	SelectScene.h
// ���e:	�Z���N�g�V�[����`�悷��N���X
// �쐬:	�r�c����
// 
// --------------------------------------------------
#pragma once
// �C���N���[�h
#include "IScene.h"
#include "Interface/IObserver.h"
#include "Game/Data/GameData.h"
#include "Game/UI/UIAnchor.h"
#include "Screen.h"

// �O���錾
class CollisionManager;
class CommonResources;
class UserInterface;
class IAction;
class Camera;
// �X�e�[�W�֘A
class Sea;
class Wall;
class Floor;
class SkySphere;
class SelectSceneObject;

class SelectScene final : public IScene, public IObserver
{
	// -----------------------------
	// �Œ�l
	// -----------------------------
public:
	// ���=- Espace�L�[��UI -=Ё�
	static constexpr DirectX::SimpleMath::Vector2 ESPACE_POSITION = DirectX::SimpleMath::Vector2(100.0f, (Screen::BOTTOM - 20.0f));
	static constexpr DirectX::SimpleMath::Vector2 ESPACE_SIZE = DirectX::SimpleMath::Vector2(0.4f, 0.4f);
	
	// ���=- SPACE�L�[��UI -=Ё�
	static constexpr DirectX::SimpleMath::Vector2 SPACE_POSITION = DirectX::SimpleMath::Vector2(Screen::RIGHT - 100.0f, (Screen::BOTTOM - 20.0f));
	static constexpr DirectX::SimpleMath::Vector2 SPACE_SIZE = DirectX::SimpleMath::Vector2(0.4f, 0.4f);

	// ���=- �X�e�[�W�Z���N�g��UI -=Ё�
	static constexpr DirectX::SimpleMath::Vector2 LOGO_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT - 100.0f, Screen::TOP);
	static constexpr DirectX::SimpleMath::Vector2 LOGO_SIZE = DirectX::SimpleMath::Vector2(0.6f, 0.6f);

	// ���=-�@�Z���N�g�X�e�[�W��UI -=Ё�
	static constexpr DirectX::SimpleMath::Vector2 TUTORIAL_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, Screen::TOP + 275.0f);
	static constexpr DirectX::SimpleMath::Vector2 BOSS_POSITION = DirectX::SimpleMath::Vector2(Screen::LEFT + 50.0f, Screen::TOP + 425.0f);
	// �Z���N�g�X�e�[�WUI�̑傫��
	static constexpr DirectX::SimpleMath::Vector2 SELECT_STAGE_UI_SIZE = DirectX::SimpleMath::Vector2(0.5f, 0.5f);


	// ���=-�@�J�����̌Œ�l�@-=Ё�
	// ����p
	static constexpr float FOV = 45.0f;
	// ������i�߂��j
	static constexpr float NEAR_Z = 0.1f;
	// ������i�����j
	static constexpr float FAR_Z = 50000.0f;
	// �J�����̈ʒu
	static constexpr DirectX::SimpleMath::Vector3 CAMERA_POSITION = { 50.0f, 100.0f, 30.0f };
	// �J�����̌���
	static constexpr DirectX::SimpleMath::Vector3 CAMERA_DIRECTION = { 0.0f, 200.0f, 200.0f };

	// ���=-�@�V�[���C���f�b�N�X�@-=Ё�
	// �ő�X�e�[�W
	static constexpr int MAX_STAGE_INDEX = 1;
	// �ŏ��X�e�[�W
	static constexpr int MIN_STAGE_INDEX = 0;


	// ------------------------------
	// �����o�֐�(���J)
	// ------------------------------
public:
	// �R���X�g���N�^
	SelectScene();
	// �f�X�g���N�^
	~SelectScene() override;
	// ����������
	void Initialize() override;
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render() override;
	// �I������
	void Finalize() override;
	// ���̃V�[��ID���擾����
	SceneID GetNextSceneID() const;
	// �O�̃V�[��ID���擾����
	SceneID GetPrevSceneID() const;
	// �L�[���́i�����ꂽ�u�ԁj
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;
	// �L�[���́i������Ă���ԁj
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;

	// ------------------------------
	// �����o�ϐ�(����J)
	// ------------------------------
private:
	// UI�̒ǉ�
	void AddUserInterface(
		const std::string& texName,
		const DirectX::SimpleMath::Vector2& position,
		const DirectX::SimpleMath::Vector2& size,
		const ANCHOR& anchor,
		IAction* action
	);

	// ���[�U�[�C���^�[�t�F�[�X�̐���
	void CreateUI();
	// �X�e�[�W�̐���
	void CreateStage();
	// �X�e�[�W�̕`��
	void RenderStage();
	// �g�p�L�[�{�[�h�̓o�^
	void RegisterKeyboard();
	// ���L�[�������ꂽ�Ƃ��̏���
	void OnKeyDownArrowDown();
	// ��L�[�������ꂽ�Ƃ��̏���
	void OnKeyDownArrowUp();
	// �X�y�[�X�L�[�������ꂽ�Ƃ��̏���
	void OnKeyDownSpace();

	// ------------------------------
	// �����o�ϐ�
	// ------------------------------
private:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �e�N�X�`��
	std::unique_ptr<CommonResources> m_commonResources;
	// ���[�U�[�C���^�[�t�F�[�X���X�g
	std::vector<std::unique_ptr<UserInterface>> m_uiList;
	// �Փ˔���
	std::unique_ptr<CollisionManager> m_collisionManager;

	// �J����
	std::unique_ptr<Camera> m_camera;

	// �C
	std::unique_ptr<Sea> m_sea;
	// ��
	std::unique_ptr<Wall> m_wall;
	// ��
	std::unique_ptr<Floor> m_floor;
	// ��
	std::unique_ptr<SkySphere> m_skySphere;
	// �Z���N�g�V�[���I�u�W�F�N�g
	std::unique_ptr<SelectSceneObject> m_selectStageObject;

	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_projection;

	// �V�[���ύX�t���O
	bool m_isChangeScene = false;

	// �I�𒆂̃X�e�[�W�ԍ�
	int m_selectIndex;



	// ���f��
	DirectX::Model* m_bossModel;
};