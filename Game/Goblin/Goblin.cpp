// -----------------------------------------------------
// 名前:	Goblin.cpp
// 内容:	小鬼を実装するクラス
//			ステートや衝突判定を管理
// 作成:	池田桜輔
// -----------------------------------------------------
// インクルード
#include "pch.h"
#include "Goblin.h"
#include "Libraries/MyLib/Math.h"
#include "Interface/IState.h"
#include "Game/Data/HPSystem.h"
#include "Game/GameResources.h"
#include "../Boss/Boss.h"
#include "Effects/EnemyEffect/EnemyEffect.h"
#include "Game/HitStop/HitStop.h"
#include "Game/Messenger/EventMessenger.h"
#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"
#include "State/Header/GoblinDead.h"
#include "State/Header/GoblinTutorial.h"

// ---------------
// 固定値
// ---------------
const float Goblin::GOBLIN_SPEED = Boss::BOSS_SPEED / 2.0f;	// 移動速
const float Goblin::GOBLIN_SCALE = Boss::BOSS_SCALE / 4.0f;	// サイズ

// -------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// -------------------------------------------------------
Goblin::Goblin()
	:
	m_position{},
	m_velocity{},
	m_angle{},
	m_scale{ 1.0f, 1.0f, 1.0f },
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity },
	m_model{ nullptr },
	m_nowAttacking{ false },
	m_pushBackValue{},
	m_isHit{ false },
	m_canHit{ false },
	m_coolTime{}
{
}

// -------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// -------------------------------------------------------
Goblin::~Goblin()
{
}

// -------------------------------------------------------
/// <summary>
/// 初期化関数
/// </summary>
// -------------------------------------------------------
void Goblin::Initialize()
{
	// モデルの読み込み
	m_model = GameResources::GetInstance()->GetModel("goblin");

	// ステートの作成
	CreateState();
	// 当たり判定の生成
	CreateCollision();
	// HPの生成
	m_hp = std::make_unique<HPSystem>(GOBLIN_HP);
	// エフェクトの生成
	m_enemyEffect = std::make_unique<EnemyEffect>();
}

// -------------------------------------------------------
/// <summary>
/// ステート作成
/// </summary>
// -------------------------------------------------------
void Goblin::CreateState()
{
	// 待機
	m_idling = std::make_unique<GoblinIdling>(this);
	m_idling->Initialize();
	m_states.push_back(m_idling.get());

	// 攻撃
	m_attacking = std::make_unique<GoblinAttacking>(this);
	m_attacking->Initialize();
	m_states.push_back(m_attacking.get());

	// 死亡
	m_dead = std::make_unique<GoblinDead>(this);
	m_dead->Initialize();
	m_states.push_back(m_dead.get());

	// チュートリアル
	m_tutorial = std::make_unique<GoblinTutorial>(this);
	m_tutorial->Initialize();
	m_states.push_back(m_tutorial.get());

	// 初期ステートを待機状態に
	m_currentState = m_idling.get();
}

// -------------------------------------------------------
/// <summary>
/// 当たり判定作成
/// </summary>
// -------------------------------------------------------
void Goblin::CreateCollision()
{
	// 当たり判定の生成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE * COLLISION_RADIUS);

	// 衝突データの作成
	CollisionData<DirectX::BoundingSphere> data =
	{
		ObjectType::Goblin,		// オブジェクトの種類
		CollisionType::Sphere,	// 当たり判定の種類
		this,					// このクラスのポインタ
		m_bodyCollision.get()	// 当たり判定のポインタ
	};

	// 衝突判定をManagerに登録
	EventMessenger::Execute(EventList::AddSphereCollision, &data);
}

// -------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// -------------------------------------------------------
void Goblin::Update(const float elapsedTime)
{
	// ワールド行列の初期化
	CalcWorldMatrix();
	// エフェクトの更新
	m_enemyEffect->Update(elapsedTime);
	// 衝突判定の移動
	MoveCollision();
	// クールタイムのカウント
	CountCoolTime(elapsedTime);
	// 生存確認
	CheckAlive();

	// ステートの更新処理
	m_currentState->Update(elapsedTime);
}

// -------------------------------------------------------
/// <summary>
/// ワールド行列の計算
/// </summary>
// -------------------------------------------------------
void Goblin::CalcWorldMatrix()
{
	// 回転の作成
	Quaternion rotation = Quaternion::CreateFromYawPitchRoll(
		m_angle,
		0.0f,
		0.0f
	);

	// 回転行列の作成
	m_worldMatrix = Matrix::CreateFromQuaternion(rotation);

	// スケールと平行移動を適用
	m_worldMatrix *= Matrix::CreateScale(GOBLIN_SCALE * m_scale);	// スケール適用
	m_worldMatrix *= Matrix::CreateTranslation(m_position);		// 位置適用
}

// -------------------------------------------------------
/// <summary>
/// 衝突判定の移動
/// </summary>
// -------------------------------------------------------
void Goblin::MoveCollision()
{
	// 衝突判定の座標を動いた座標に合わせる
	DirectX::SimpleMath::Vector3 pos = m_position;
	pos.y = COLLISION_POS_Y;
	m_bodyCollision->Center = pos;
}

// -------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
// -------------------------------------------------------
void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_enemyEffect->DrawWithEffect(m_model, m_worldMatrix, view, projection);
}

// -------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// -------------------------------------------------------
void Goblin::Finalize()
{
	m_idling->Finalize();
	m_attacking->Finalize();
	m_dead->Finalize();

	// 削除用衝突判定のデータを作成
	DeleteCollisionData data = { CollisionType::Sphere, this };

	// 削除イベントを実行
	EventMessenger::Execute(EventList::DeleteCollision, &data);
}

// -------------------------------------------------------
/// <summary>
/// 当たったときの処理
/// </summary>
/// <param name="data">衝突データ</param>
// -------------------------------------------------------
void Goblin::HitAction(InterSectData data)
{
	switch (data.objType)
	{
	case ObjectType::Player:	HitPlayer(data);	break;
	case ObjectType::Goblin:	HitGoblin(data);	break;
	case ObjectType::Boss:		HitBoss(data);		break;
	case ObjectType::Stage:		HitStage(data);		break;
	case ObjectType::Sword:		HitSword(data);		break;
	}
}

// -------------------------------------------------------
/// <summary>
/// プレイヤーに当たったときの処理
/// </summary>
/// <param name="data">衝突データ</param>
// -------------------------------------------------------
void Goblin::HitPlayer(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}

// -------------------------------------------------------
/// <summary>
/// 小鬼に当たったときの処理
/// </summary>
/// <param name="data">衝突データ</param>
// -------------------------------------------------------
void Goblin::HitGoblin(InterSectData data)
{
	// 衝突したオブジェクトの情報を取得
	auto goblinCollision = *m_bodyCollision.get();
	auto goblinCollision2 = *data.collision;

	// 押し戻し量を計算
	m_pushBackValue = Math::pushBack_BoundingSphere(goblinCollision, goblinCollision2) / 2.0f;
	m_pushBackValue.y = 0.0f;
	// プレイヤーの位置を押し戻す
	m_position += m_pushBackValue;
	m_bodyCollision->Center = m_position;
}

// -------------------------------------------------------
/// <summary>
/// 敵に当たったときの処理
/// </summary>
/// <param name="data">衝突データ</param>
// -------------------------------------------------------
void Goblin::HitBoss(InterSectData data)
{
	m_pushBackValue = DirectX::SimpleMath::Vector3::Zero;

	// 衝突したオブジェクトの情報を取得
	auto goblinCollision = *m_bodyCollision.get();
	auto enemyCollision = *data.collision;

	// 押し戻し量を計算
	m_pushBackValue += Math::pushBack_BoundingSphere(goblinCollision, enemyCollision);
	m_pushBackValue.y = 0.0f;

	m_position += m_pushBackValue;
	m_bodyCollision->Center = m_position;
}

// -------------------------------------------------------
/// <summary>
/// ステージに当たったときの処理
/// </summary>
/// <param name="data">衝突データ</param>
// -------------------------------------------------------
void Goblin::HitStage(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}

// -------------------------------------------------------
/// <summary>
/// 剣に当たったときの処理
/// </summary>
/// <param name="data">衝突データ</param>
// -------------------------------------------------------
void Goblin::HitSword(InterSectData data)
{
	// 衝突可能でない場合は処理を行わない
	if (!m_canHit) return;
	m_canHit = false;

	UNREFERENCED_PARAMETER(data);
	Damaged(1);
}

// -------------------------------------------------------
/// <summary>
/// ステートの変更
/// </summary>
/// <param name="state">変更するステート</param>
// -------------------------------------------------------
void Goblin::ChangeState(GoblinState state)
{
	// 引数からステートのインデックスを取得
	int index = static_cast<int>(state);

	// ステートが存在しない場合は処理を行わない
	if (m_currentState == m_states[index]) return;

	// ステートの終了処理
	m_currentState->PostUpdate();
	// ステートの変更
	m_currentState = m_states[index];
	// ステートの初期化
	m_currentState->PreUpdate();
}

// -------------------------------------------------------
/// <summary>
/// ダメージを受けたときの処理
/// </summary>
/// <param name="damage">受けるダメージ量</param>
// -------------------------------------------------------
void Goblin::Damaged(float damage)
{
	// すでにダメージを受けている場合は処理を行わない
	if (m_isHit)return;
	// HPを減らす
	m_hp->Damage(damage);
	// ヒットストップを有効にする
	HitStop::GetInstance()->SetActive();
	// エフェクトを再生
	m_enemyEffect->SetEffect(EnemyEffect::ENEMY_EFFECT::DAMAGE);

	// 画面を揺らす
	float shakePower = 0.25f;
	EventMessenger::Execute(EventList::ShakeCamera, &shakePower);

	// クールタイムを有効にする
	m_isHit = true;
}

// -------------------------------------------------------
/// <summary>
/// クールタイムのカウント
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// -------------------------------------------------------
void Goblin::CountCoolTime(float elapsedTime)
{
	// 過去に攻撃を受けていない場合は処理を行わない
	if (!m_isHit)return;

	// クールタイムをカウント
	m_coolTime += elapsedTime;

	if (m_coolTime > COOL_TIME)
	{
		m_isHit = false;
		m_coolTime = 0.0f;
	}
}

// -------------------------------------------------------
/// <summary>
/// 生存確認
/// </summary>
// -------------------------------------------------------
void Goblin::CheckAlive()
{
	// 死亡している場合は処理を行わない
	if (m_currentState == m_dead.get()) return;

	if (m_hp->GetHP() <= 0)
	{
		// ステートを変更
		ChangeState(GoblinState::DEAD);
		// 死亡エフェクトを再生
		m_enemyEffect->SetEffect(EnemyEffect::ENEMY_EFFECT::DEAD);
	}
}

// -------------------------------------------------------
/// <summary>
/// ゴブリンを消す
/// </summary>
// -------------------------------------------------------
void Goblin::DeleteGoblin()
{
	EventMessenger::Execute(EventList::DeleteEnemy, this);
}