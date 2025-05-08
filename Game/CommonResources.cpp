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
    m_inputManager(nullptr),
    m_threadPool(std::thread::hardware_concurrency())
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

    // ディファードコンテキストを作成する
    CreateDeferredContexts(m_deviceResources->GetD3DDevice());
}

// ---------------------------------------------------------
// ディファードコンテキストを作成する
// ---------------------------------------------------------
/// <summary>
/// スレッドごとにディファードコンテキストを作成する
/// </summary>
/// <param name="device">D3D11デバイス</param>
// CommonResources.cpp の CreateDeferredContexts メソッドを修正

void CommonResources::CreateDeferredContexts(ID3D11Device* device)
{
    assert(device);

    // 既存のコンテキストをクリア
    {
        std::lock_guard<std::mutex> lock(m_contextMutex);
        m_deferredContexts.clear();
    }

    // スレッド数を取得
    const unsigned int threadCount = m_threadPool.GetThreadCount();

    // --- デバッグ出力の追加 ---
    char debugMsg[256];
    sprintf_s(debugMsg, "CreateDeferredContexts: スレッド数 = %u\n", threadCount);
    OutputDebugStringA(debugMsg);

    // 各スレッド用のコンテキストをあらかじめ作成
    std::vector<Microsoft::WRL::ComPtr<ID3D11DeviceContext>> tempContexts;
    tempContexts.reserve(threadCount);

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
        HRESULT hr = device->CreateDeferredContext(0, &context);
        if (FAILED(hr))
        {
            sprintf_s(debugMsg, "DeferredContext %u の作成に失敗: 0x%08X\n", i, hr);
            OutputDebugStringA(debugMsg);
            throw std::runtime_error("ディファードコンテキストの作成に失敗しました");
        }
        tempContexts.push_back(context);

        // コンテキストをすぐにマップ（スレッドIDのマッピングを待たずに）
        // 一時的なIDを使用
        std::lock_guard<std::mutex> lock(m_contextMutex);
        char tempThreadIdStr[32];
        sprintf_s(tempThreadIdStr, "TempThreadID_%u", i);
        std::thread::id tempId;
        memcpy(&tempId, tempThreadIdStr, std::min(sizeof(tempId), sizeof(tempThreadIdStr)));
        m_deferredContexts[tempId] = context;
    }

    sprintf_s(debugMsg, "作成したDeferredContextの数: %zu\n", tempContexts.size());
    OutputDebugStringA(debugMsg);

    // 各スレッドが実際に使用する前に正しいスレッドIDにマッピングするためのタスクを登録
    for (unsigned int i = 0; i < threadCount; ++i)
    {
        m_threadPool.Enqueue([this, i, &tempContexts]() {
            // 現在のスレッドIDを取得
            std::thread::id threadId = std::this_thread::get_id();

            char threadDebug[256];
            sprintf_s(threadDebug, "スレッド %u: ID = %zu にコンテキストをマッピング\n",
                i, std::hash<std::thread::id>{}(threadId));
            OutputDebugStringA(threadDebug);

            // 対応するコンテキストをスレッドIDに紐づけて保存
            std::lock_guard<std::mutex> lock(m_contextMutex);
            if (i < tempContexts.size()) {
                m_deferredContexts[threadId] = tempContexts[i];
            }
            });
    }

    // 全タスクの完了を待機
    m_threadPool.WaitAll();

    // 最終的なコンテキスト数を確認
    {
        std::lock_guard<std::mutex> lock(m_contextMutex);
        sprintf_s(debugMsg, "最終的なDeferredContextの数: %zu\n", m_deferredContexts.size());
        OutputDebugStringA(debugMsg);
    }
}

// ---------------------------------------------------------
// ディファードコンテキストを取得する
// ---------------------------------------------------------
ID3D11DeviceContext* CommonResources::GetDeferredContext()
{
    std::lock_guard<std::mutex> lock(m_contextMutex);
    auto it = m_deferredContexts.find(std::this_thread::get_id());
    if (it != m_deferredContexts.end())
    {
        return it->second.Get();
    }

    // 見つからない場合はnullptrを返す（エラーハンドリングを改善）
    OutputDebugStringA("警告: 現在のスレッド用のディファードコンテキストが見つかりません\n");
    return nullptr;
}

// ---------------------------------------------------------
// ゲッターメソッド
// ---------------------------------------------------------

DX::StepTimer* CommonResources::GetStepTimer() const { return m_stepTimer; }
DX::DeviceResources* CommonResources::GetDeviceResources() const { return m_deviceResources; }
DirectX::CommonStates* CommonResources::GetCommonStates() const { return m_commonStates; }
mylib::DebugString* CommonResources::GetDebugString() const { return m_debugString; }
mylib::InputManager* CommonResources::GetInputManager() const { return m_inputManager; }
ThreadPool* CommonResources::GetThreadPool() { return &m_threadPool; }

std::vector<ID3D11DeviceContext*> CommonResources::GetAllDeferredContexts()
{
    std::lock_guard<std::mutex> lock(m_contextMutex);
    std::vector<ID3D11DeviceContext*> result;
    result.reserve(m_deferredContexts.size()); // パフォーマンス向上のため、容量を確保

    for (auto& pair : m_deferredContexts)
    {
        if (pair.second) {  // nullチェックを追加
            result.push_back(pair.second.Get());
        }
    }
    return result;
}