// ------------------------------------------------------------------
// 名前:	BinaryFile.cpp
// 
// 内容:	バイナリファイルを扱うクラス
//			シェーダーの読み込み等に使用
// 制作:	池田桜輔
// 
// ------------------------------------------------------------------
// インクルード
#include "pch.h"
#include "BinaryFile.h"
#include <fstream>
#include <assert.h>


/// <summary>
/// ファイルの読み込み
/// </summary>
/// <param name="fileName">ファイルパス</param>
/// <returns>バイナリーファイル</returns>

BinaryFile BinaryFile::LoadFile(const wchar_t* fileName)
{
	// 保存用のバイナリーファイル
	BinaryFile bin;
	// ファイルストリーム
	std::ifstream ifs;
	// ファイルを開く
	ifs.open(fileName, std::ios::in | std::ios::binary);
	// ファイルが無効の場合
	if (!ifs) {
		MessageBox(nullptr, (std::wstring(L"ファイルを開けませんでした: ") + fileName).c_str(), L"エラー", MB_OK | MB_ICONERROR);
		assert(false && "ファイルを開けませんでした");
	}
	// ファイルサイズを取得
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	bin.m_size = (unsigned int)(eofPos - begPos);

	// 読み込むためのメモリを確保
	bin.m_data.reset(new char[bin.m_size]);

	// ファイル先頭からバッファへコピー
	ifs.read(bin.m_data.get(), bin.m_size);

	// ファイルクローズ
	ifs.close();
	// バイナリーファイルを返す
	return std::move(bin);
}


/// <summary>
/// コンストラクタ
/// </summary>

BinaryFile::BinaryFile()
{
	m_size = 0;
}


/// <summary>
/// ムーブコンストラクタ
/// </summary>
/// <param name="in">ムーブ元</param>
BinaryFile::BinaryFile(BinaryFile&& in)
{
	m_data = std::move(in.m_data);
	m_size = in.m_size;
}