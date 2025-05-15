// ----------------------------------------------------------------------
//
// 名前 : BossFace.cpp
// 内容 : ボスの顔クラス
// 作成 : 池田桜輔
// 
// ----------------------------------------------------------------------

// インクルード
#include "pch.h"
#include "BossFace.h"
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/ThreadedRenderer/ThreadedRenderer.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossFace::BossFace()
{
	// 待機顔モデル取得
	m_IdlingFace = GameResources::GetInstance()->GetModel("bossIdlingFace");
	// 攻撃顔モデル取得
	m_AttackingFace = GameResources::GetInstance()->GetModel("bossAttackingFace");

	// モデル設定
	m_model = m_IdlingFace;

	// マルチスレッドに登録
	auto threadedRenderer = ThreadedRenderer::GetInstance();
	threadedRenderer->RegisterRenderable(this);
}


/// <summary>
/// デストラクタ
/// </summary>
BossFace::~BossFace()
{
	// マルチスレッドから削除
	auto threadedRenderer = ThreadedRenderer::GetInstance();
	threadedRenderer->UnregisterRenderable(this);
}



/// <summary>
/// 描画コマンドを登録する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクト行列</param>
/// <param name="deferredContext">ディファードコンテキスト</param>
void BossFace::RecordRenderCommands(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* deferredContext)
{
	// 共通リソースから取得する
	CommonResources* resources = CommonResources::GetInstance();
	auto states = resources->GetCommonStates();

	// ディファードコンテキストに描画コマンドを登録
	m_model->Draw(deferredContext, *states, m_world, view, proj);
}


/// <summary>
/// 顔の切り替えを行う
/// </summary>
/// <param name="index">0が待機　1が攻撃</param>
void BossFace::ChangeFace(const int index)
{
	switch (index)
	{
	case IDLING_FACE:
		// 待機顔に変更
		m_model = m_IdlingFace;
		break;

	case ATTACKING_FACE:
		// 攻撃顔に変更
		m_model = m_AttackingFace;
		break;

	default:
		MessageBoxA(nullptr,
			"BossFace.cpp 85行目 : 存在しない顔に変更しています","Error",
			MB_OK | MB_ICONERROR);
		break;
	}
}
