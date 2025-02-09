// ---------------------------------------------------------
//
// 名前:	TitleEnemy.cpp
// 内容:	タイトルシーンの敵を実装している
// 作成;	池田桜輔
//
// ---------------------------------------------------------
// インクルード
#include "pch.h"
#include <Model.h>
#include "Game/CommonResources.h"
#include "Game/GameResources.h"
#include "DeviceResources.h"
#include "TitleEnemy.h"
#include "EnemyTitleMoving.h"
#include "EnemyTitleIdling.h"
#include "Interface/IState.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
TitleEnemy::TitleEnemy()
	:
	m_currentState{},
	m_position{},
	m_angle{},
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------

TitleEnemy::~TitleEnemy()
{
	Finalize();
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------

void TitleEnemy::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// モデルの取得
	m_model = GameResources::GetInstance()->GetModel("titleOni");

	// ステートの作成
	CreateState();
}

// ---------------------------------------------------------
/// <summary>
/// ステートの作成処理
/// </summary>
// ---------------------------------------------------------
void TitleEnemy::CreateState()
{
	// 待機状態の作成・初期化・保存
	m_titleIdling = std::make_unique<EnemyTitleIdling>(this);
	m_titleIdling->Initialize();
	m_states[static_cast<int>(BossState::IDLING)] = m_titleIdling.get();

	// 移動状態の作成・初期化・保存
	m_titleMoving = std::make_unique<EnemyTitleMoving>(this);
	m_titleMoving->Initialize();
	m_states[static_cast<int>(BossState::MOVING)] = m_titleMoving.get();

	// 初期のステートを待機状態に割り当てる
	m_currentState = m_titleIdling.get();
}

// ---------------------------------------------------------
/// <summary>
/// 状態遷移処理
/// </summary>
/// <param name="state">遷移先の状態</param>
// ---------------------------------------------------------
void TitleEnemy::ChangeState(BossState state)
{
	// ステートのインデックスを取得
	int index = static_cast<int>(state);

	// おんなじステートを更新しようとしたら戻る
	if (m_currentState == m_states[index]) return;
	// 新規の状態遷移前に事後更新を行う
	m_currentState->PostUpdate();
	// 新規の状態を現在の状態に設定する
	m_currentState = m_states[index];
	// 新規の状態遷移後に事前更新を行う
	m_currentState->PreUpdate();
}


// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------

void TitleEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;

	// ステータスを更新
	m_currentState->Update(elapsedTime);

	// 回転角の計算
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));

	// 移動処理
	m_velocity *= TitleEnemy::TITLE_ENEMY_SPEED;
	// 位置の更新
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ワールド行列の計算
	m_worldMatrix
		// サイズ計算
		*= DirectX::SimpleMath::Matrix::CreateScale(TITLE_ENEMY_SCALE)
		// 位置の設定
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}

// ---------------------------------------------------------
/// <summary>
/// 表示処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">射影行列</param>
// ---------------------------------------------------------
void TitleEnemy::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// デバイスコンテキストを取得
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	// コモンステートを取得
	auto states = CommonResources::GetInstance()->GetCommonStates();

	// 深度値を参照して書き込む
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	// モデルの描画
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

}

// ---------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// ---------------------------------------------------------
void TitleEnemy::Finalize()
{
}