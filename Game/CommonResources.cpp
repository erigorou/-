// ----------------------------------------------------------------
//
// 名前：CommonResources.cpp
// 機能：ゲーム内で使用する共通リソースの管理
// 作成：2025/05/07
//
// ----------------------------------------------------------------

// インクルード
#include "pch.h"
#include "CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/ThreadPool/ThreadPool.h"
#include <cassert>

// 静的メンバ変数の定義
std::unique_ptr<CommonResources> CommonResources::m_resources = nullptr;

// ---------------------------------------------------------
// インスタンスを取得する
// ---------------------------------------------------------
/// <summary>
/// CommonResourcesのシングルトンインスタンスを取得する
/// </summary>
/// <returns>CommonResourcesクラスのインスタンス</returns>
CommonResources* const CommonResources::GetInstance()
{
    if (m_resources == nullptr)
    {
        // インスタンスが存在しない場合は生成する
        m_resources.reset(new CommonResources());
    }
    return m_resources.get();
}

// ---------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------
/// <summary>
/// CommonResourcesクラスのコンストラクタ
/// </summary>
CommonResources::CommonResources()
    : m_stepTimer(nullptr),
    m_deviceResources(nullptr),
    m_commonStates(nullptr),
    m_debugString(nullptr),
    m_inputManager(nullptr)
{
}

// ---------------------------------------------------------
// 初期化する
// ---------------------------------------------------------
/// <summary>
/// 共通リソースを初期化する
/// </summary>
/// <param name="timer">タイマー</param>
/// <param name="dr">デバイスリソース</param>
/// <param name="commonStates">共通ステート</param>
/// <param name="debugString">デバッグ文字列</param>
/// <param name="inputManager">入力管理</param>
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