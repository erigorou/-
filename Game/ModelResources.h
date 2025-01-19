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
// ファイルパス
// ------------------------
public:
	// モデルのJsonファイル
	static const wchar_t* MODEL_JSON;
	// テクスチャのJsonファイル
	static const wchar_t* TEXTURE_JSON;


// ------------------------
// アクセサ
// ------------------------
public:

	// Resoucesクラスのインスタンスを取得する
	static ModelResources* const GetInstance();
	// モデルを取得する
	DirectX::Model* GetModel(std::string key) const;
	// テクスチャを取得する
	ID3D11ShaderResourceView* GetTexture(std::string key) const;


// ------------------------
// 公開関数
// ------------------------
public:
	// コンストラクタ
	ModelResources();
	// デストラクタ
	~ModelResources() = default;


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
	static std::unique_ptr<ModelResources> m_resources;

	// モデルリスト
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_modelList;

	// テクスチャリスト
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textureList;

};