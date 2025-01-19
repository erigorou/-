#pragma once
#include <unordered_map>

enum class MODEL_ID : int
{
	PLAYER = 0,
	ENEMY,
	SWORD,
	CUDGEL,
	WALL
};


enum class TEXTURE_ID : int
{
	FLOOR = 0
};


class ModelResources
{
// ------------------------
// �t�@�C���p�X
// ------------------------
public:
	// ���f����Json�t�@�C��
	static const wchar_t* MODEL_JSON;
	// �e�N�X�`����Json�t�@�C��
	static const wchar_t* TEXTURE_JSON;


// ------------------------
// �A�N�Z�T
// ------------------------
public:

	// Resouces�N���X�̃C���X�^���X���擾����
	static ModelResources* const GetInstance();
	// ���f�����擾����
	DirectX::Model* GetModel(std::string key) const;
	// �e�N�X�`�����擾����
	ID3D11ShaderResourceView* GetTexture(std::string key) const;


// ------------------------
// ���J�֐�
// ------------------------
public:
	// �R���X�g���N�^
	ModelResources();
	// �f�X�g���N�^
	~ModelResources() = default;


// ------------------------
// �����֐�
// ------------------------
private:
	// Json�t�@�C����ǂݍ���Ń��f���𐶐�����
	void LoadModelFromJson();

	// �e�N�X�`����ǂݍ���
	void LoadTexture();


// ------------------------
// �����ϐ�
// ------------------------
private:

	// ���\�[�X
	static std::unique_ptr<ModelResources> m_resources;

	// ���f�����X�g
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_modelList;

	// �e�N�X�`�����X�g
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textureList;

};