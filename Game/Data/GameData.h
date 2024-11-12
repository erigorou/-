#pragma once


class GameData
{
	// �񋓌^ -------------------
public:
	enum class BATTLE_RESULT : unsigned int
	{
		WIN,
		LOSE,
		NONE
	};


// �A�N�Z�T�֐� ------------
public:

	// �ÓI�֐�===========================================
	// GameData�N���X�̃C���X�^���X���擾����
	static GameData* const GetInstance();


	// �퓬����
	void			SetBattleResult(BATTLE_RESULT result)	{ m_battleResult = result;	}
	BATTLE_RESULT	GetBattleResult() const					{ return m_battleResult;	}

	// ��ʂ̃t�F�[�h��
	void	SetFadeValue(float value) { m_fadeValue = value; }
	float	GetFadeValue()		const { return m_fadeValue; }

	// �o�ߎ���
	void	SetElapsedTime(float time) { m_elapsedTime = time; }
	float	GetElapsedTime()	const { return m_elapsedTime; }


// �C���X�^���X���ׂ̈�private	(���d������h������)
private:
	// �R���X�g���N�^
	GameData();

// ���J�֐� -----------------
public:
	// �f�X�g���N�^
	~GameData() = default;

	// ����������
	void Initialize();


// �����o�ϐ� --------------
private:

	static std::unique_ptr<GameData> s_data;	// �C���X�^���X

	BATTLE_RESULT m_battleResult; // �퓬����

	float m_fadeValue;	// ��ʂ̃t�F�[�h��

	float m_elapsedTime;	// �o�ߎ���

};