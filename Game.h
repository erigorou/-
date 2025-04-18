//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

// ★includeの追記★
#include "Game/CommonResources.h"
#include "Game/Scene/IScene.h"
#include "Game/Scene/SceneManager.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

	Game() noexcept(false);
	~Game() = default;

	Game(Game&&) = default;
	Game& operator= (Game&&) = default;

	Game(Game const&) = delete;
	Game& operator= (Game const&) = delete;

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// IDeviceNotify
	void OnDeviceLost() override;
	void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowMoved();
	void OnDisplayChange();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const noexcept;

	// 画面モードを設定する関数（TRUE：フルスクリーン）
	void SetFullscreenState(BOOL value)
	{
		m_fullscreen = value;
		m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
		if (value) m_deviceResources->CreateWindowSizeDependentResources();
	}

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	// Device resources.
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;

	// Rendering loop timer.
	DX::StepTimer                           m_timer;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates>  m_commonStates;
	// コモンリソース
	CommonResources* m_commonResources;
	// デバッグストリング
	std::unique_ptr<mylib::DebugString>     m_debugString;
	// 入力マネージャ
	std::unique_ptr<mylib::InputManager>    m_inputManager;
	// シーンマネージャ
	std::unique_ptr<SceneManager>           m_sceneManager;

	BOOL m_fullscreen;	// ＜―コンストラクタでFALSEで初期化してください。
};
