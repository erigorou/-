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

    // ディファードコンテキストを取得する
    ID3D11DeviceContext* GetDeferredContext();

    // スレッドプールを取得する
    ThreadPool* GetThreadPool();

    // タイマーを取得する
    DX::StepTimer* GetStepTimer() const;

    // デバイスリソースを取得する
    DX::DeviceResources* GetDeviceResources() const;

    // 共通ステートを取得する
    DirectX::CommonStates* GetCommonStates() const;

    // デバッグ文字列を取得する
    mylib::DebugString* GetDebugString() const;

    // 入力管理を取得する
    mylib::InputManager* GetInputManager() const;

    // 全てのディファードコンテキストを取得する
    std::vector<ID3D11DeviceContext*> GetAllDeferredContexts();

    // ------------------
    // メンバ関数（非公開）
    // ------------------
private:
    // コンストラクタ
    CommonResources();

    // ディファードコンテキストを作成する
    void CreateDeferredContexts(ID3D11Device* device);

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
    // スレッドプール
    ThreadPool m_threadPool;
    // ディファードコンテキスト
    std::map<std::thread::id, Microsoft::WRL::ComPtr<ID3D11DeviceContext>> m_deferredContexts;
    // コンテキストへのアクセスを保護するミューテックス
    std::mutex m_contextMutex;
};