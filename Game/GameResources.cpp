#include "pch.h"
#include "GameResources.h"
#include <cassert>
#include <fstream>
#include "nlohmann/json.hpp"
#include "CommonResources.h"
#include "DeviceResources.h"

// ユニークポインタ
std::unique_ptr<GameResources> GameResources::m_resources = nullptr;

// モデルのJsonファイル
const wchar_t* GameResources::MODEL_JSON = L"Resources/Jsons/ModelData.Json";
// テクスチャのJsonファイル
const wchar_t* GameResources::TEXTURE_JSON = L"Resources/Jsons/TextureData.Json";

// モデルのベースパス
const wchar_t* GameResources::MODEL_BASE_PATH = L"Resources/Models/";
// テクスチャのベースパス
const wchar_t* GameResources::TEXTURE_BASE_PATH = L"Resources/Textures/";


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
GameResources::GameResources()
{
	// Jsonファイルを読み込んでモデルを生成する
	LoadModelFromJson();
	// テクスチャを読み込む
	LoadTexture();
}


//---------------------------------------------------------
// インスタンスを取得
//---------------------------------------------------------
GameResources* const GameResources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// GameResourcesクラスのインスタンスを生成する
		m_resources.reset(new GameResources());
	}
	// GameResourcesクラスのインスタンスを返す
	return m_resources.get();
}


// --------------------------------------------------------
// Jsonファイルを読み込んでモデルを生成する
// --------------------------------------------------------
void GameResources::LoadModelFromJson()
{
	// Jsonファイルを開く
	std::ifstream ifs(MODEL_JSON);
	if (!ifs)
	{
		MessageBoxA(nullptr, "Jsonファイルが見つかりません", "エラー", MB_OK);
		return; // 処理を終了
	}

	// デバイスを取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// Jsonファイルを読み込む
	nlohmann::json json = nlohmann::json::parse(ifs);

	// Jsonデータを順に処理
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		// キーと値を取得
		std::string key = it.key();
		std::string pathStr = it.value();

		// std::stringをstd::wstringに変換
		std::wstring path = MODEL_BASE_PATH + std::wstring(pathStr.begin(), pathStr.end());

		// モデルファクトリの生成
		std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
		fx->SetDirectory(L"Resources/Models");

		// モデルを生成
		auto model = DirectX::Model::CreateFromCMO(device, path.c_str(), *fx);

		// モデルをリストに追加
		m_modelList[key] = std::move(model);
	}
}



// --------------------------------------------------------
// Jsonファイルを読み込んでテクスチャを生成する
// --------------------------------------------------------
void GameResources::LoadTexture()
{
	// Jsonファイルを開く
	std::ifstream ifs(TEXTURE_JSON);
	if (!ifs)
	{
		MessageBoxA(nullptr, "Jsonファイルが見つかりません", "エラー", MB_OK);
		return; // 処理を終了
	}

	// デバイスを取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// Jsonファイルを読み込む
	nlohmann::json json = nlohmann::json::parse(ifs);

	// Jsonデータを順に処理
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		// キーと値を取得
		std::string key = it.key();
		std::string pathStr = it.value();

		// std::stringをstd::wstringに変換
		std::wstring path = TEXTURE_BASE_PATH + std::wstring(pathStr.begin(), pathStr.end());

		// テクスチャを生成
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

		DirectX::CreateWICTextureFromFile(
			device,
			path.c_str(),
			nullptr,
			texture.GetAddressOf()
		);

		// テクスチャをリストに追加
		m_textureList[key] = texture;
	}
}


//---------------------------------------------------------
// モデルを取得する
//---------------------------------------------------------
DirectX::Model* GameResources::GetModel(std::string key) const
{
	// キーを検索
	auto it = m_modelList.find(key);
	if (it != m_modelList.end())
	{
		// モデルを返す
		return it->second.get();
	}
}


//---------------------------------------------------------
// テクスチャを取得する
//---------------------------------------------------------
ID3D11ShaderResourceView* GameResources::GetTexture(std::string key) const
{
	// キーを検索
	auto it = m_textureList.find(key);
	if (it != m_textureList.end())
	{
		// テクスチャを返す
		return it->second.Get();
	}
}