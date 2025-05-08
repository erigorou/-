// ----------------------------------------------------------------
//
// 名前：CommonResources.h
// 機能：ゲーム内で使用する共通リソースの管理
// 作成：2025/05/07
//
// ----------------------------------------------------------------

// インクルード
#pragma once
#include "pch.h"
#include "Libraries/MyLib/ThreadPool/ThreadPool.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <map>
#include <mutex>
#include <wrl.h>

// 前方宣言
namespace DX {
    class DeviceResources;
    class StepTimer;
}

namespace mylib {
    class DebugString;
    class InputManager;
}

/// <summary>
/// ゲーム内で使用する共通リソースを管理するクラス
/// </summary>
class CommonResources
{
    // ------------------
    // メンバ関数（公開）
    // ------------------
public:
    // インスタンスを取得する
    static CommonResources* const GetInstance();

    // 初期化する
    void Initialize(
        DX::StepTimer* timer,
        DX::DeviceResources* dr,
        DirectX::CommonStates* commonStates,
        mylib::DebugString* debugString,
        mylib::InputManager* inputManager
    );

    // タイマーを取得する
    DX::StepTimer* GetStepTimer() const
    {
		return m_stepTimer;
    }

    // デバイスリソースを取得する
    DX::DeviceResources* GetDeviceResources() const
    {
		return m_deviceResources;
    }

    // 共通ステートを取得する
    DirectX::CommonStates* GetCommonStates() const
    {
		return m_commonStates;
    }

    // デバッグ文字列を取得する
    mylib::DebugString* GetDebugString() const
    {
		return m_debugString;
    }

    // 入力管理を取得する
    mylib::InputManager* GetInputManager() const
    {
		return m_inputManager;
    }


    // ------------------
    // メンバ関数（非公開）
    // ------------------
private:
    // コンストラクタ
    CommonResources();


    // ------------------
    // メンバ変数
    // ------------------
private:
    // インスタンス
    static std::unique_ptr<CommonResources> m_resources;
    // タイマー
    DX::StepTimer* m_stepTimer;
    // デバイスリソース
    DX::DeviceResources* m_deviceResources;
    // 共通ステート
    DirectX::CommonStates* m_commonStates;
    // デバッグ文字列表示
    mylib::DebugString* m_debugString;
    // 入力管理
    mylib::InputManager* m_inputManager;
};