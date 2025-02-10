// ----------------------------------------------------------
// 名前:	Cudgel
// 
// 内容:	ボス鬼の武器の金棒クラス
//			更新処理や描画を行う
// 			ステートマシーンを使用
//
// 制作:	池田桜輔
// ----------------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "header/CudgelIdling.h"
#include "header/CudgelAttacking.h"
#include "header/CudgelSweeping.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="boss"></param>
Cudgel::Cudgel(Boss* boss)
	:
	m_boss(boss),
	m_currentState(),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_isDead(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Cudgel::~Cudgel()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void Cudgel::Initialize()
{
	// 金棒のポインタを取得
	EventMessenger::AttachGetter(GetterList::GetCudgel, std::bind(&Cudgel::GetCudgelObject, this));
	// 金棒のステートを変更
	EventMessenger::Attach(EventList::ChangeCudgelState, std::bind(&Cudgel::ChangeState, this, std::placeholders::_1));

	// モデルを取得する
	m_model = GameResources::GetInstance()->GetModel("cudgel");

	// ステートの作成
	CreateState();
	// 当たり判定の生成
	CreateCollision();
}

/// <summary>
/// 状態の生成
/// </summary>
void Cudgel::CreateState()
{
	// 待機の生成・初期化・登録
	m_idling = std::make_unique<CudgelIdling>(this);
	m_idling->Initialize();
	m_states.push_back(m_idling.get());

	// 攻撃の生成・初期化・登録
	m_attacking = std::make_unique<CudgelAttacking>(this);
	m_attacking->Initialize();
	m_states.push_back(m_attacking.get());

	// 薙ぎ払いの生成・初期化・登録
	m_sweeping = std::make_unique<CudgelSweeping>(this);
	m_sweeping->Initialize();
	m_states.push_back(m_sweeping.get());

	// 初期状態を指定
	m_currentState = m_idling.get();
}

/// <summary>
/// 当たり判定生成：登録
/// </summary>
void Cudgel::CreateCollision()
{
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);
	m_collision = std::make_unique<DirectX::BoundingOrientedBox>(m_originalBox);

	// 衝突データの作成
	CollisionData<DirectX::BoundingOrientedBox> data =
	{
		ObjectType::Cudgel,		// オブジェクトの種類
		CollisionType::OBB,		// 当たり判定の種類
		this,					// このクラスのポインタ
		m_collision.get()		// 当たり判定
	};

	// 当たり判定を記録する
	EventMessenger::Execute(EventList::AddOBBCollision, &data);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Cudgel::Update(float elapsedTime)
{
	// 現在のステートの更新処理
	m_currentState->Update(elapsedTime);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void Cudgel::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// 死亡しているなら早期リターン
	if (m_isDead) return;

	// モデルを描画
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}

/// <summary>
/// 終了処理
/// </summary>
void Cudgel::Finalize()
{
}

/// <summary>
/// ステート変更
/// </summary>
/// <param name="state">新しいステート</param>
void Cudgel::ChangeState(void* state)
{
	int index = *static_cast<int*>(state);

	// ステートの変更
	if (m_currentState == m_states[index]) return;

	// ステートの事後処理
	m_currentState->PostUpdate();
	// 新しいステートに切り替える
	m_currentState = m_states[index];
	// 新しいステートの事前処理を行う
	m_currentState->PreUpdate();
}

/// <summary>
/// 衝突イベント
/// </summary>
/// <param name="data">衝突相手</param>
void Cudgel::HitAction(InterSectData data)
{
	m_currentState->HitAction(data);
}