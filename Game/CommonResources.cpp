/*
	@file	CommonResources.cpp
	@brief	シーンへ渡す、ゲーム内で使用する共通リソース
*/
#include "pch.h"
#include "CommonResources.h"
#include <cassert>

std::unique_ptr<CommonResources> CommonResources::m_resources = nullptr;

//---------------------------------------------------------
// コンストラクタ
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
}