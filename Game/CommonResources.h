// ------------------------------------------------------------------------
//
// 名前: CommonResources.h
// 概要: シーンへ渡す、ゲーム内で使用する共通リソース
// 作成: 2024/06/25
// 
// ------------------------------------------------------------------------

#pragma once
// インクルード
#include <wrl/client.h>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <thread>

// 前方宣言
namespace DX
{
	class StepTimer;
	class DeviceResources;
}
namespace mylib
{
	class DebugString;
	class InputManager;
}

// 共通リソース
class CommonResources
{
private:
	// 受け渡しするリソース一覧
	DX::StepTimer* m_stepTimer;

	DX::DeviceResources* m_deviceResources;

	DirectX::CommonStates* m_commonStates;

	mylib::DebugString* m_debugString;

	mylib::InputManager* m_inputManager;

	// ディファードコンテキスト
	std::unordered_map<std::thread::id, Microsoft::WRL::ComPtr<ID3D11DeviceContext>> m_deferredContexts;
	// DeferredContectのリストのロック用変数
	std::mutex m_contextMutex;

	// リソース
	static std::unique_ptr<CommonResources> m_resources;

private:
	CommonResources();

public:
	~CommonResources() = default;

	// Resoucesクラスのインスタンスを取得する
	static CommonResources* const GetInstance();

	void Initialize(
		DX::StepTimer* timer,
		DX::DeviceResources* dr,
		DirectX::CommonStates* commonStates,
		mylib::DebugString* debugString,
		mylib::InputManager* inputManager
	);

	//ディファードコンテキストを作成する
	void CreateDeferredContexts(ID3D11Device* device);


	// getter
	DX::StepTimer* GetStepTimer() const
	{
		return m_stepTimer;
	}

	DX::DeviceResources* GetDeviceResources() const
	{
		return m_deviceResources;
	}

	DirectX::CommonStates* GetCommonStates() const
	{
		return m_commonStates;
	}

	mylib::DebugString* GetDebugString() const
	{
		return m_debugString;
	}

	mylib::InputManager* GetInputManager() const
	{
		return m_inputManager;
	}

	ID3D11DeviceContext* GetDeferredContext();
};
