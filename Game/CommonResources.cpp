/*
	@file	CommonResources.cpp
	@brief	シーンへ渡す、ゲーム内で使用する共通リソース
*/
#include "pch.h"
#include "CommonResources.h"
#include "DeviceResources.h"
#include <cassert>

std::unique_ptr<CommonResources> CommonResources::m_resources = nullptr;

//---------------------------------------------------------
// インスタンスを取得する
//---------------------------------------------------------
CommonResources* const CommonResources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// CommonResourcesクラスのインスタンスを生成する
		m_resources.reset(new CommonResources());
	}
	// CommonResourcesクラスのインスタンスを返す
	return m_resources.get();
}

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
CommonResources::CommonResources()
	:
	m_stepTimer{},
	m_deviceResources{},
	m_commonStates{},
	m_debugString{},
	m_inputManager{}
{
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void CommonResources::Initialize(
	DX::StepTimer* timer,
	DX::DeviceResources* dr,
	DirectX::CommonStates* commonStates,
	mylib::DebugString* debugString,
	mylib::InputManager* inputManager
)
{
	assert(timer);
	assert(dr);
	assert(commonStates);
	assert(debugString);
	assert(inputManager);

	m_stepTimer = timer;
	m_deviceResources = dr;
	m_commonStates = commonStates;
	m_debugString = debugString;
	m_inputManager = inputManager;

	// ディファードコンテキストを作成する
	CreateDeferredContexts(m_deviceResources->GetD3DDevice());
}


/// <summary>
/// ディファードコンテキストを作成する
/// </summary>
/// <param name="device">デバイスコンテキスト</param>
/// <summary>
/// ディファードコンテキストを作成する
/// </summary>
/// <param name="device">デバイスコンテキスト</param>
void CommonResources::CreateDeferredContexts(ID3D11Device* device)
{
	assert(device);

	unsigned int threadCount = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;

	{
		// メインスレッドでアクセスする可能性もあるため、事前にロック
		std::lock_guard<std::mutex> lock(m_contextMutex);
		m_deferredContexts.clear();
	}

	for (unsigned int i = 0; i < threadCount; ++i)
	{
		// 各スレッドでディファードコンテキストを作成
		threads.emplace_back([this, device]() {
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
			HRESULT hr = device->CreateDeferredContext(0, &context);
			if (FAILED(hr))
			{
				throw std::runtime_error("ディファードコンテキストの作成に失敗しました");
			}

			// スレッドIDとコンテキストを紐づける
			std::lock_guard<std::mutex> lock(m_contextMutex);
			m_deferredContexts[std::this_thread::get_id()] = context;
			});
	}

	// 全てのスレッドが終了するまで待機
	for (auto& t : threads)
	{
		t.join();
	}
}


/// <summary>
/// ディファードコンテキストを取得する
/// </summary>
/// <returns>ディファードコンテキスト</returns>
ID3D11DeviceContext* CommonResources::GetDeferredContext()
{
	auto threadId = std::this_thread::get_id();
	std::lock_guard<std::mutex> lock(m_contextMutex);

	// スレッドIDに対応するコンテキストを返す
	if (m_deferredContexts.find(threadId) != m_deferredContexts.end())
	{
		return m_deferredContexts[threadId].Get();
	}

	// 見つからなければnullptrを返す
	return nullptr;
}

