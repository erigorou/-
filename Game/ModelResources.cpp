#include "pch.h"
#include "ModelResources.h"
#include <cassert>
#include <fstream>
#include "nlohmann/json.hpp"
#include "CommonResources.h"
#include "DeviceResources.h"

// ユニークポインタ
std::unique_ptr<ModelResources> ModelResources::m_resources = nullptr;

// モデルのJsonファイル
const wchar_t* ModelResources::MODEL_JSON = L"Resources/Jsons/ModelData.Json";
// テクスチャのJsonファイル
const wchar_t* ModelResources::TEXTURE_JSON = L"Resources/Jsons/TextureData.Json";


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ModelResources::ModelResources()
{
	// Jsonファイルを読み込んでモデルを生成する
	LoadModelFromJson();
	// テクスチャを読み込む
	LoadTexture();
}


//---------------------------------------------------------
// インスタンスを取得
//---------------------------------------------------------
ModelResources* const ModelResources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// ModelResourcesクラスのインスタンスを生成する
		m_resources.reset(new ModelResources());
	}
	// ModelResourcesクラスのインスタンスを返す
	return m_resources.get();
}


// --------------------------------------------------------
// Jsonファイルを読み込んでモデルを生成する
// --------------------------------------------------------
void ModelResources::LoadModelFromJson()
{
	// Jsonファイルを開く
	std::ifstream ifs(MODEL_JSON);
	if (!ifs)
	{
		MessageBoxA(nullptr, "Jsonファイルが見つかりません", "エラー", MB_OK);
	}

	// デバイスを取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// Jsonファイルを読み込む
	nlohmann::json json = nlohmann::json::parse(ifs);

	// モデルの数だけ繰り返す
	for (auto& data : json)
	{
		// モデルファクトリの生成
		std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
		fx->SetDirectory(L"Resources/Models");

		// std::stringをstd::wstringに変換して、const wchar_t*に渡す
		std::wstring path = std::wstring(data["path"].get<std::string>().begin(), data["path"].get<std::string>().end());

		// モデルを生成
		auto model = DirectX::Model::CreateFromCMO(device, path.c_str(), *fx);

		// モデルをリストに追加
		m_modelList[data["key"].get<std::string>()] = std::move(model);
	}
}


// --------------------------------------------------------
// Jsonファイルを読み込んでテクスチャを生成する
// --------------------------------------------------------
void ModelResources::LoadTexture()
{
	// Jsonファイルを開く
	std::ifstream ifs(TEXTURE_JSON);
	if (!ifs)
	{
		MessageBoxA(nullptr, "Jsonファイルが見つかりません", "エラー", MB_OK);
	}

	// デバイスを取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// Jsonファイルを読み込む
	nlohmann::json json = nlohmann::json::parse(ifs);

	// テクスチャの数だけ繰り返す
	for (auto& data : json)
	{
		// std::stringをstd::wstringに変換して、const wchar_t*に渡す
		std::wstring path = std::wstring(data["path"].get<std::string>().begin(), data["path"].get<std::string>().end());

		// テクスチャを生成
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		DirectX::CreateWICTextureFromFile(
			device,
			path.c_str(),
			nullptr,
			texture.GetAddressOf()
		);

		// テクスチャをリストに追加
		m_textureList[data["key"].get<std::string>()] = texture;
	}
}


//---------------------------------------------------------
// モデルを取得する
//---------------------------------------------------------
DirectX::Model* ModelResources::GetModel(std::string key) const
{
	return nullptr;
}


//---------------------------------------------------------
// テクスチャを取得する
//---------------------------------------------------------
ID3D11ShaderResourceView* ModelResources::GetTexture(std::string key) const
{
	return nullptr;
}