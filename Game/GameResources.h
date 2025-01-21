#pragma once
#include <unordered_map>

class GameResources
{
// ------------------------
// ファイルパス
// ------------------------
public:
	// モデルのJsonファイル
	static const wchar_t* MODEL_JSON;
	// テクスチャのJsonファイル
	static const wchar_t* TEXTURE_JSON;

	static const wchar_t* MODEL_BASE_PATH;
	static const wchar_t* TEXTURE_BASE_PATH;

// ------------------------
// アクセサ
// ------------------------
public:

	// Resoucesクラスのインスタンスを取得する
	static GameResources* const GetInstance();
	// モデルを取得する
	DirectX::Model* GetModel(std::string key) const;
	// テクスチャを取得する
	ID3D11ShaderResourceView* GetTexture(std::string key) const;


// ------------------------
// 公開関数
// ------------------------
public:
	// コンストラクタ
	GameResources();
	// デストラクタ
	~GameResources() = default;


// ------------------------
// 内部関数
// ------------------------
private:
	// Jsonファイルを読み込んでモデルを生成する
	void LoadModelFromJson();

	// テクスチャを読み込む
	void LoadTexture();


// ------------------------
// 内部変数
// ------------------------
private:

	// リソース
	static std::unique_ptr<GameResources> m_resources;

	// モデルリスト
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_modelList;

	// テクスチャリスト
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textureList;

};