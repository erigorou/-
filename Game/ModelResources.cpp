#include "pch.h"
#include "ModelResources.h"
#include <cassert>
#include <fstream>
#include "nlohmann/json.hpp"
#include "CommonResources.h"
#include "DeviceResources.h"

// ���j�[�N�|�C���^
std::unique_ptr<ModelResources> ModelResources::m_resources = nullptr;

// ���f����Json�t�@�C��
const wchar_t* ModelResources::MODEL_JSON = L"Resources/Jsons/ModelData.Json";
// �e�N�X�`����Json�t�@�C��
const wchar_t* ModelResources::TEXTURE_JSON = L"Resources/Jsons/TextureData.Json";


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
ModelResources::ModelResources()
{
	// Json�t�@�C����ǂݍ���Ń��f���𐶐�����
	LoadModelFromJson();
	// �e�N�X�`����ǂݍ���
	LoadTexture();
}


//---------------------------------------------------------
// �C���X�^���X���擾
//---------------------------------------------------------
ModelResources* const ModelResources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// ModelResources�N���X�̃C���X�^���X�𐶐�����
		m_resources.reset(new ModelResources());
	}
	// ModelResources�N���X�̃C���X�^���X��Ԃ�
	return m_resources.get();
}


// --------------------------------------------------------
// Json�t�@�C����ǂݍ���Ń��f���𐶐�����
// --------------------------------------------------------
void ModelResources::LoadModelFromJson()
{
	// Json�t�@�C�����J��
	std::ifstream ifs(MODEL_JSON);
	if (!ifs)
	{
		MessageBoxA(nullptr, "Json�t�@�C����������܂���", "�G���[", MB_OK);
	}

	// �f�o�C�X���擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// Json�t�@�C����ǂݍ���
	nlohmann::json json = nlohmann::json::parse(ifs);

	// ���f���̐������J��Ԃ�
	for (auto& data : json)
	{
		// ���f���t�@�N�g���̐���
		std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
		fx->SetDirectory(L"Resources/Models");

		// std::string��std::wstring�ɕϊ����āAconst wchar_t*�ɓn��
		std::wstring path = std::wstring(data["path"].get<std::string>().begin(), data["path"].get<std::string>().end());

		// ���f���𐶐�
		auto model = DirectX::Model::CreateFromCMO(device, path.c_str(), *fx);

		// ���f�������X�g�ɒǉ�
		m_modelList[data["key"].get<std::string>()] = std::move(model);
	}
}


// --------------------------------------------------------
// Json�t�@�C����ǂݍ���Ńe�N�X�`���𐶐�����
// --------------------------------------------------------
void ModelResources::LoadTexture()
{
	// Json�t�@�C�����J��
	std::ifstream ifs(TEXTURE_JSON);
	if (!ifs)
	{
		MessageBoxA(nullptr, "Json�t�@�C����������܂���", "�G���[", MB_OK);
	}

	// �f�o�C�X���擾
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// Json�t�@�C����ǂݍ���
	nlohmann::json json = nlohmann::json::parse(ifs);

	// �e�N�X�`���̐������J��Ԃ�
	for (auto& data : json)
	{
		// std::string��std::wstring�ɕϊ����āAconst wchar_t*�ɓn��
		std::wstring path = std::wstring(data["path"].get<std::string>().begin(), data["path"].get<std::string>().end());

		// �e�N�X�`���𐶐�
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		DirectX::CreateWICTextureFromFile(
			device,
			path.c_str(),
			nullptr,
			texture.GetAddressOf()
		);

		// �e�N�X�`�������X�g�ɒǉ�
		m_textureList[data["key"].get<std::string>()] = texture;
	}
}


//---------------------------------------------------------
// ���f�����擾����
//---------------------------------------------------------
DirectX::Model* ModelResources::GetModel(std::string key) const
{
	return nullptr;
}


//---------------------------------------------------------
// �e�N�X�`�����擾����
//---------------------------------------------------------
ID3D11ShaderResourceView* ModelResources::GetTexture(std::string key) const
{
	return nullptr;
}