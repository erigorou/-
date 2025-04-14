// ---------------------------------------------
// 
// 名前		:SelectSceneObject.cpp
// 内容		:セレクトシーンで表示するオブジェクト
// 作成		:池田桜輔
// 
// ---------------------------------------------
// インクルード
#include "pch.h"
#include "SelectSceneObject.h"
#include "Game/GameResources.h"
#include "Game/Data/GameData.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

// -----------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
SelectSceneObject::SelectSceneObject()
	: 
	m_models(),
	m_stageNo(),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
{
}

// -----------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
SelectSceneObject::~SelectSceneObject()
{
	Finalize();
}

// -----------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
void SelectSceneObject::Initialize()
{
	// モデルの受け取り
	LoadModel();
}

// -----------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
void SelectSceneObject::Update(float elapsedTime)
{
	// ゲームデータにアクセス
	auto gameData = GameData::GetInstance();
	// ステージ番号の取得
	m_stageNo = gameData->GetSelectStage();

	// モデルの回転
	m_rotation.y -= ROTATE_SPEED * elapsedTime;

	// ワールド行列の計算
	CalcWorldMatrix();
}

// -----------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// /// <param name="proj">射影行列</param>
void SelectSceneObject::Render(const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj)
{
	if (m_models[m_stageNo] == nullptr) return;

	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// エフェクトの設定
	m_models[m_stageNo]->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetLightingEnabled(false); // ライト有効化
				basicEffect->SetPerPixelLighting(true); // ピクセル単位のライティング有効化
				basicEffect->SetTextureEnabled(false); // テクスチャの無効化
				basicEffect->SetVertexColorEnabled(false); // 頂点カラーの無効化
				basicEffect->SetFogEnabled(false); // フォグの無効化
			}
		}
	);

	m_models[m_stageNo]->Draw(context, *states, m_worldMatrix, view, proj, false, [&]
		{
			// ブレンドステートを設定
			context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
		}
	);
}

// -----------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
void SelectSceneObject::Finalize()
{
}

// -----------------------------------------------
/// <summary>
/// モデルの受け取り
/// </summary>
void SelectSceneObject::LoadModel()
{
	// モデルの取得
	m_models.push_back(GameResources::GetInstance()->GetModel("selectTutorial"));
	m_models.push_back(GameResources::GetInstance()->GetModel("selectBoss"));
}

// -----------------------------------------------
/// <summary>
/// ワールド行列の計算
/// </summary>
void SelectSceneObject::CalcWorldMatrix()
{
	// ワールド行列の計算
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;
	// 回転行列の計算(ヨーピッチロール)
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll
	(
		m_rotation.y,
		m_rotation.x,
		m_rotation.z
	);

	// 移動行列の計算
	m_worldMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(
		MODEL_POSITION.x,
		MODEL_POSITION.y,
		MODEL_POSITION.z
	);
}