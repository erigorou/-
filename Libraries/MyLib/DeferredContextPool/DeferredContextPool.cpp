// ----------------------------------------------------------------
//
// ファイル：DeferredContextPool.cpp
// 機能：遅延コンテキストのプールクラス
// 作成：2025/05/08
//
// ----------------------------------------------------------------
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "DeferredContextPool.h"
#include "Interface/IRenderable.h"


// シングルトンインスタンス
DeferredContextPool* DeferredContextPool::s_instance = nullptr;


/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
/// <returns>シングルトン</returns>
DeferredContextPool* DeferredContextPool::GetInstance()
{
	if (!s_instance)
	{
		s_instance = new DeferredContextPool();
	}
	return s_instance;
}


/// <summary>
/// コンストラクタ
/// </summary>
DeferredContextPool::DeferredContextPool()
{
	// 遅延コンテキストの作成
	CreateDeferredContexts();
}


/// <summary>
/// デストラクタ
/// </summary>
DeferredContextPool::~DeferredContextPool()
{
}


/// <summary>
/// 全リセット
/// </summary>
void DeferredContextPool::Reset()
{
	// 未使用のコンテキストを全て解放
	for (auto& pair : m_contextMap)
	{
		m_unusedContexts.push_back(pair.second);
	}
	m_contextMap.clear();
}


/// <summary>
/// 遅延コンテキスト群の作成
/// </summary>
void DeferredContextPool::CreateDeferredContexts()
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	for (int i = 0; i < MAX_CONTEXTS; ++i)
	{
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

		HRESULT hr = device->CreateDeferredContext(0, &context);
		if (FAILED(hr))
		{
			MessageBoxA(nullptr, "遅延コンテキストの作成に失敗しました", "エラー", MB_OK | MB_ICONERROR);
			continue;
		}

		// 作成したコンテキストを未使用リストに追加
		m_unusedContexts.push_back(context);
	}
}


/// <summary>
/// DeferredContextを紐づける
/// </summary>
/// <param name="object">使用するオブジェクト</param>
void DeferredContextPool::RegisterRenderable(const IRenderable* object)
{
	// すでに登録済み
	if (m_contextMap.find(object) != m_contextMap.end()){
		return;
	}

	// プールが空
	if (m_unusedContexts.empty())
	{
		MessageBoxA(nullptr, "未使用の遅延コンテキストがありません", "エラー", MB_OK | MB_ICONERROR);
		return;
	}

	// 未使用プールから一つ取得
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = m_unusedContexts.back();
	m_unusedContexts.pop_back();

	// コンテキストをオブジェクトに紐づけ
	m_contextMap[object] = context;
}


/// <summary>
/// オブジェクトと紐づけられた遅延コンテキストを解放する
/// </summary>
/// <param name="object">解放したオブジェクト</param>
void DeferredContextPool::ReleaseDeferredContext(const IRenderable* object)
{
	// オブジェクトが登録されていない
	if (m_contextMap.find(object) == m_contextMap.end())
	{
		MessageBoxA(nullptr, "指定されたオブジェクトは登録されていません", "エラー", MB_OK | MB_ICONERROR);
		return;
	}

	// 紐づけられたコンテキストを取得
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = m_contextMap[object];

	// コンテキストを未使用リストに戻す
	m_unusedContexts.push_back(context);

	// マップから削除
	m_contextMap.erase(object);
}


/// <summary>
/// 使用中のコンテキストを全てリセットする
/// </summary>
void DeferredContextPool::ResetAllContexts()
{
	// 使用中の全てのコンテキストをリセット
	for (auto& pair : m_contextMap)
	{
		if (pair.second)
		{
			pair.second->ClearState();
		}
	}
}


/// <summary>
/// 紐づいた遅延コンテキストを取得する
/// </summary>
/// <param name="object">使用するオブジェクト</param>
/// <returns>紐づいた遅延コンテキスト</returns>
ID3D11DeviceContext* DeferredContextPool::GetDeferredContext(const IRenderable* object)
{
	// オブジェクトが登録されていない
	if (m_contextMap.find(object) == m_contextMap.end())
	{
		MessageBoxA(nullptr, "指定されたオブジェクトは登録されていません", "エラー", MB_OK | MB_ICONERROR);
		return nullptr;
	}

	// 紐づけられたコンテキストを返す
	return m_contextMap[object].Get();
}


/// <summary>
/// 使用済みの全ての遅延コンテキストを取得する
/// </summary>
/// <returns>全ての遅延コンテキストのリスト</returns>
std::vector<Microsoft::WRL::ComPtr<ID3D11DeviceContext>> DeferredContextPool::GetAllDeferredContexts() const
{
	std::vector<Microsoft::WRL::ComPtr<ID3D11DeviceContext>> allContexts;

	// 紐づけられたコンテキストを追加
	for (const auto& pair : m_contextMap)
	{
		allContexts.push_back(pair.second);
	}

	return allContexts;
}


/// <summary>
/// 全て削除
/// </summary>
void DeferredContextPool::Clear()
{
	// 未使用のコンテキストを全て解放
	m_unusedContexts.clear();

	// 紐づけられたコンテキストを全て解放
	m_contextMap.clear();
}
