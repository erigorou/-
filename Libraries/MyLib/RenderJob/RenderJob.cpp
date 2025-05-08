// ----------------------------------------------------------------
//
// ファイル：RenderJob.cpp
// 機能：個別のレンダリングタスクを表現するクラス
// 作成：2025/05/08
//
// ----------------------------------------------------------------

#include "pch.h"
#include "RenderJob.h"

/// <summary>
/// レンダリングジョブのコンストラクタ
/// </summary>
/// <param name="renderable">描画対象のオブジェクト</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
/// <param name="deferredContext">遅延コンテキスト</param>
RenderJob::RenderJob(IRenderable* renderable,
    const DirectX::SimpleMath::Matrix& view,
    const DirectX::SimpleMath::Matrix& proj,
    ID3D11DeviceContext* deferredContext)
    : m_renderable(renderable),
    m_view(view),
    m_proj(proj),
    m_deferredContext(deferredContext)
{
}

/// <summary>
/// 描画コマンドの実行 - IRenderableを使用して描画コマンドを記録
/// </summary>
void RenderJob::Execute() 
{
    // パラメータの検証
    if (!m_renderable || !m_deferredContext) 
    {
        return;
    }

    // レンダラブルオブジェクトに描画コマンドを記録させる
    m_renderable->RecordRenderCommands(m_view, m_proj, m_deferredContext.Get());

    // コマンドリストの生成
    HRESULT hr = m_deferredContext->FinishCommandList(FALSE, &m_commandList);
    if (FAILED(hr)) 
    {
        // エラー処理 - デバッグ出力
        MessageBoxA(nullptr, "RenderJob: コマンドリストの作成に失敗しました", "エラー", MB_OK | MB_ICONERROR);
    }
}