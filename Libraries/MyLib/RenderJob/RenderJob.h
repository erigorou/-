// ----------------------------------------------------------------
//
// ファイル：RenderJob.h
// 機能：個別のレンダリングタスクを表現するクラス
// 作成：2025/05/08
//
// ----------------------------------------------------------------

#pragma once
#include "pch.h"
#include "Interface/IRenderable.h"
#include <wrl/client.h>

/// <summary>
/// レンダリングジョブクラス - 1つのオブジェクトの描画コマンドをカプセル化
/// </summary>
class RenderJob {
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="renderable">描画対象のオブジェクト</param>
    /// <param name="view">ビュー行列</param>
    /// <param name="proj">プロジェクション行列</param>
    /// <param name="deferredContext">遅延コンテキスト</param>
    RenderJob(IRenderable* renderable,
        const DirectX::SimpleMath::Matrix& view,
        const DirectX::SimpleMath::Matrix& proj,
        ID3D11DeviceContext* deferredContext);

    /// <summary>
    /// 描画コマンドの実行
    /// </summary>
    void Execute();

    /// <summary>
    /// コマンドリストの取得
    /// </summary>
    /// <returns>コマンドリスト</returns>
    ID3D11CommandList* GetCommandList() const { return m_commandList.Get(); }

private:
    // レンダラブルオブジェクトへの参照
    IRenderable* m_renderable;

    // ビュー行列
    DirectX::SimpleMath::Matrix m_view;

    // プロジェクション行列
    DirectX::SimpleMath::Matrix m_proj;

    // 遅延コンテキスト
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deferredContext;

    // コマンドリスト
    Microsoft::WRL::ComPtr<ID3D11CommandList> m_commandList;
};