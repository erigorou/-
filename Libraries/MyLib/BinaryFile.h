// ------------------------------------------------------------------
// 名前:	BinaryFile.
// 
// 内容:	バイナリファイルを扱うクラス
//			シェーダーの読み込み等に使用
// 制作:	池田桜輔
// 
// ------------------------------------------------------------------
#pragma once
// インクルード
#include <memory>

/// <summary>
/// バイナリファイルクラス
/// </summary>
class BinaryFile
{
	// -----------------------------
	// アクセサ
	// -----------------------------
public:
	// データの取得
	char* GetData() { return m_data.get(); }
	// サイズの取得
	unsigned int GetSize() { return m_size; }

	// -----------------------------
	// メンバ関数(公開)
	// -----------------------------
public:
	// ファイル名を指定してロード
	static BinaryFile LoadFile(const wchar_t* fileName);
	// コンストラクタ
	BinaryFile();
	// ムーブコンストラクタ
	BinaryFile(BinaryFile&& in);

	// -----------------------------
	// メンバ変数
	// -----------------------------
protected:
	// データ
	std::unique_ptr<char[]> m_data;
	// サイズ
	unsigned int m_size;

};
