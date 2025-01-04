// -------------------------------------------------------
// 小鬼の親クラス
// -------------------------------------------------------

#include "pch.h"
#include "Goblin.h"
#include "Game/Player/Player.h"
#include "Game/Scene/PlayScene.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/DebugString.h"
#include "Interface/IState.h"
#include "Game/Data/HPSystem.h"
#include "../Enemy/Enemy.h"
#include "Effects/EnemyDamageEffect/EnemyDamageEffect.h"
#include "Game/HitStop/HitStop.h"

#include "State/Header/GoblinIdling.h"
#include "State/Header/GoblinAttacking.h"


// ---------------
// 固定値
// ---------------
const float Goblin::GOBLIN_SPEED = Enemy::ENEMY_SPEED / 2.0f;	// 移動速
const float Goblin::GOBLIN_SCALE = Enemy::ENEMY_SCALE / 4.0f;	// サイズ


// -------------------------------
// コンストラクタ
// -------------------------------
Goblin::Goblin(PlayScene* playScene)
	: m_playScene(playScene)
	, m_position{ 80.0f, 0.0f, 0.0f } // ★　仮置き
	, m_velocity{}
	, m_angle{}
	, m_scale{1.0f, 1.0f, 1.0f}
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_model(nullptr)
	, m_nowAttacking(false)
	, m_pushBackValue{}
	, m_isHit(false)
	, m_coolTime(0.0f)
{
}

// -------------------------------
// デストラクタ
// -------------------------------
Goblin::~Goblin()
{
}

// --------------------------------
// 初期化関数
// --------------------------------
void Goblin::Initialize()
{
	// リソースの取得
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Goblin/goblin.cmo", *fx);

	// ステートの作成
	CreateState();
	// 当たり判定の生成
	CreateCollision();
	// HPの生成
	m_hp = std::make_unique<HPSystem>(GOBLIN_HP);
	// ダメージエフェクトの生成
	m_damageEffect = std::make_unique<EnemyDamageEffect>();

}

// --------------------------------
// ステートの作成
// --------------------------------
void Goblin::CreateState()
{
	m_idling = std::make_unique<GoblinIdling>(this);		// 待機
	m_attacking = std::make_unique<GoblinAttacking>(this);	// 攻撃

	m_idling->Initialize();
	m_attacking->Initialize();

	m_currentState = m_idling.get();
}


// --------------------------------
// 当たり判定の生成
// --------------------------------
void Goblin::CreateCollision()
{
	// 当たり判定の生成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, GOBLIN_SCALE * COLLISION_RADIUS);

	// 当たり判定をCollisionManagerに登録する
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Goblin,
		CollisionType::Sphere,
		this,
		m_bodyCollision.get()
	);
}


// --------------------------------
// 更新処理
// --------------------------------
void Goblin::Update(const float elapsedTime)
{
	// ワールド行列の初期化
	CalcWorldMatrix();
	// ステートの更新処理
	m_currentState->Update(elapsedTime);
	// ダメージエフェクトの更新
	m_damageEffect->Update(elapsedTime);
	// 衝突判定の移動
	MoveCollision();
	// クールタイムのカウント
	CountCoolTime(elapsedTime);
}


// --------------------------------
// ワールド行列の計算
// --------------------------------
void Goblin::CalcWorldMatrix()
{
	m_worldMatrix =
		DirectX::SimpleMath::Matrix::CreateRotationY(m_angle) *
		DirectX::SimpleMath::Matrix::CreateScale(GOBLIN_SCALE * m_scale) *
		DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}


// --------------------------------
// 衝突判定の移動
// --------------------------------
void Goblin::MoveCollision()
{
	DirectX::SimpleMath::Vector3 pos = m_position;
	pos.y = COLLISION_POS_Y;
	m_bodyCollision->Center = pos;
}


// --------------------------------
// 描画関数
// --------------------------------
void Goblin::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// 描画に必要なデータを取得する
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// ダメージエフェクト付きでモデルを描画する
	m_damageEffect->DrawWithDamageEffect(m_model.get(), m_worldMatrix, view, projection);
}


// --------------------------------
// 終了関数
// --------------------------------
void Goblin::Finalize()
{
	m_idling->Finalize();
	m_attacking->Finalize();

	// 当たり判定の削除
	m_playScene->GetCollisionManager()->DeleteCollision(CollisionType::Sphere, this);
}


// --------------------------------
// 当たったときの処理
// --------------------------------
void Goblin::HitAction(InterSectData data)
{
	switch (data.objType)
	{
	case ObjectType::Player:	HitPlayer(data);	break;
	case ObjectType::Goblin:	HitGoblin(data);	break;
	case ObjectType::Enemy:		HitEnemy(data);		break;
	case ObjectType::Stage:		HitStage(data);		break;
	case ObjectType::Sword:		HitSword(data);		break;
	}
}

// --------------------------------
// プレイヤーに当たったときの処理
// --------------------------------
void Goblin::HitPlayer(InterSectData data)
{
}


// --------------------------------
// 小鬼に当たったときの処理
// --------------------------------
void Goblin::HitGoblin(InterSectData data)
{
	auto goblin = static_cast<Goblin*>(data.object);

	// 衝突したオブジェクトの情報を取得
	auto goblinCollision = *m_bodyCollision.get();
	auto goblinCollision2 = goblin->GetCollision();

	// 押し戻し量を計算
	m_pushBackValue = Math::pushBack_BoundingSphere(goblinCollision, goblinCollision2);
	m_pushBackValue.y = 0.0f;
	// プレイヤーの位置を押し戻す
	m_position += m_pushBackValue;
	m_bodyCollision->Center = m_position;
}


// --------------------------------
// 敵に当たったときの処理
// --------------------------------
void Goblin::HitEnemy(InterSectData data)
{
	auto enemy = static_cast<Enemy*>(data.object);

	// 衝突したオブジェクトの情報を取得
	auto goblinCollision = *m_bodyCollision.get();
	auto enemyCollision = enemy->GetBodyCollision();

	// 押し戻し量を計算
	m_pushBackValue = Math::pushBack_BoundingSphere(goblinCollision, enemyCollision);
	m_pushBackValue.y = 0.0f;
	// プレイヤーの位置を押し戻す
	m_position += m_pushBackValue;
	m_bodyCollision->Center = m_position;
}


// --------------------------------
// ステージに当たったときの処理
// --------------------------------
void Goblin::HitStage(InterSectData data)
{
}


// --------------------------------
// 剣に当たったときの処理
// --------------------------------
void Goblin::HitSword(InterSectData data)
{
	auto sword = static_cast<Sword*>(data.object);
	// 剣が待機中でない場合
	if (sword->GetAttackFlag())
	{
		// ダメージを受ける
		Damaged(1);
	}
}


// --------------------------------
// ステートの変更
// --------------------------------
void Goblin::ChangeState(IState* state)
{
	// すでに同じステートの場合は変更しない
	if (m_currentState == state) return;
	// 変更前ステートの最終処理
	m_currentState->PostUpdate();
	// ステートの変更
	m_currentState = state;
	// 変更後ステートの初期処理
	m_currentState->PreUpdate();
}


// --------------------------------
// ダメージを受けたときの処理
// --------------------------------
void Goblin::Damaged(float damage)
{
	// すでにダメージを受けている場合は処理を行わない
	if (m_isHit)return;
	// HPを減らす
	m_hp->Damage(damage);
	// ヒットストップを有効にする
	HitStop::GetInstance()->SetActive();
	// ダメージエフェクトを再生
	m_damageEffect->IsDamaged();
	// クールタイムを有効にする
	m_isHit = true;
}


// --------------------------------
// クールタイムのカウント
// --------------------------------
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
