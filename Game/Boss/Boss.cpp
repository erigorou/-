// ----------------
//
// 鬼の挙動
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Scene/PlayScene.h"
#include "../HitStop/HitStop.h"
#include "Game/GameResources.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Game/EnemyManager/EnemyManager.h"
#include "Game/Stage/Wall/Wall.h"
#include "Interface/IState.h"
#include "BehaviourTree/Header/BehaviorTree.h"	// ビヘイビアツリー
#include "Game/Factory/Factory.h"
// ステートパターン用
#include "States/Header/BossIdling.h"// 待機状態
#include "States/Header/BossAttacking.h"// たたきつけ攻撃
#include "States/Header/BossSweeping.h"// 薙ぎ払い攻撃
#include "States/Header/BossDashAttacking.h"// 突撃攻撃
#include "States/Header/BossApproaching.h"// 追尾状態
#include "States/Header/BossDead.h"// 死亡状態
// 顔のパーツ用
#include "Face/Header/BossFaceIdling.h"
#include "Face/Header/BossFaceAttacking.h"
// エフェクト
#include "Effects/EnemyEffect/EnemyEffect.h"


// --------------------------------
//  コンストラクタ
// --------------------------------
Boss::Boss()
	:
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity },
	m_currentState()
	, m_idling()
	, m_attacking()
	, m_approaching()
	, m_coolTime()
	, m_position{ 0.0f, 0.0f, 0.0f }
	, m_angle{ 0.0f }
	, m_bodyTilt{ 0.0f }
	, m_pushBackValue{ 0.0f, 0.0f, 0.0f }
	, m_isHit(false)
	, m_canHit(false)
	, m_shakePower{0.25f}
{
}

// --------------------------------
//  デストラクタ
// --------------------------------
Boss::~Boss()
{
	m_idling->Finalize();
	m_attacking->Finalize();
	m_approaching->Finalize();
	m_dead->Finalize();
	m_dashAttacking->Finalize();
}

// --------------------------------
//  イニシャライズ
 // --------------------------------
void Boss::Initialize()
{
	// 武器の生成
	m_cudgel = Factory::CreateCudgel(this);

	// モデルを取得
	m_model = GameResources::GetInstance()->GetModel("boss");
	// HPを設定
	m_hp = std::make_unique<HPSystem>(HP);
	// ビヘイビアツリーを取得
	m_pBT = std::make_unique<BehaviorTree>();
	// ステートの作成
	CreateState();
	// 顔パーツの生成
	CreateFace();
	// エフェクトの生成
	m_effect = std::make_unique<EnemyEffect>();
	// 当たり判定の作成
	CreateCollision();
}



// --------------------------------
//  状態の生成処理
// --------------------------------
void Boss::CreateState()
{
	// === 状態の生成 ====
	m_starting		= std::make_unique<BossStarting>(this); // 開始
	m_idling		= std::make_unique<BossIdling>(this); // 待機
	m_attacking		= std::make_unique<BossAttacking>(this); // 攻撃
	m_sweeping		= std::make_unique<BossSweeping>(this); // 薙ぎ払い
	m_dashAttacking = std::make_unique<BossDashAttacking>(this); // 突撃
	m_approaching	= std::make_unique<BossApproaching>(this); // 追尾
	m_dead			= std::make_unique<BossDead>(this); // 死亡

	// === 状態の初期化 ===
	m_starting		->Initialize();		// 開始
	m_idling		-> Initialize();	// 待機
	m_attacking		-> Initialize();	// 攻撃
	m_sweeping		-> Initialize();	// 薙ぎ払い
	m_dashAttacking	-> Initialize();	// 突撃
	m_approaching	-> Initialize();	// 追尾
	m_dead			->Initialize();		// 死亡

	// 初期のステートを待機状態に割り当てる
	m_currentState = m_starting.get();
}


// --------------------------------
//  顔パーツの生成処理
// --------------------------------
void Boss::CreateFace()
{
	// 顔パーツの生成
	m_faceIdling	= std::make_unique<BossFaceIdling>(this);
	m_faceAttacking = std::make_unique<BossFaceAttacking>(this);
	// 初期の顔を待機顔に割り当てる
	m_currentFace = m_faceIdling.get();
}


// --------------------------------
//  衝突判定の生成処理
// --------------------------------
void Boss::CreateCollision()
{
	// 当たり判定の生成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, BOSS_SCALE * COLLISION_RADIUS);

	// 衝突データの作成
	CollisionData<DirectX::BoundingSphere> data =
	{
		ObjectType::Boss,		// オブジェクトの種類
		CollisionType::Sphere,	// 当たり判定の種類
		this,					// このクラスのポインタ
		m_bodyCollision.get()	// 当たり判定のポインタ
	};

	// 衝突判定をManagerに登録
	EventMessenger::Execute("AddSphereCollision", &data);
}


// --------------------------------
//  状態の生成処理
// --------------------------------
void Boss::ChangeState(IState* newState)
{
	// おんなじステートを更新しようとしたら戻る
	if (m_currentState == newState) return;
	// 新規の状態遷移前に事後更新を行う
	m_currentState->PostUpdate();
	// 新規の状態を現在の状態に設定する
	m_currentState = newState;
	// 新規の状態遷移後に事前更新を行う
	m_currentState->PreUpdate();
}


// --------------------------------
//  更新処理
// --------------------------------
void Boss::Update(float elapsedTime)
{
	// ステータスの更新処理
	m_currentState->Update(elapsedTime);
	// エフェクトの更新
	m_effect->Update(elapsedTime);
	// ワールド行列の計算
	CalcrationWorldMatrix();
	// 当たり判定の更新
	m_bodyCollision->Center = DirectX::SimpleMath::Vector3(m_position.x, m_position.y + COLISION_POS_Y, m_position.z);
	// 衝突のクールタイムの計測
	CheckHitCoolTime(elapsedTime);
	// 生存確認
	CheckAlive();
	// 武器の更新
	m_cudgel->Update(elapsedTime);

#ifdef _DEBUG

	// キー入力を受け付ける。
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1)	ChangeState(m_attacking.get());
	if (keyboardState.F2)	ChangeState(m_sweeping.get());
	if (keyboardState.F3)	ChangeState(m_dashAttacking.get());

#endif // _DEBUG
}


// --------------------------------
//  ワールド行列の計算
// --------------------------------
void Boss::CalcrationWorldMatrix()
{
	// 回転方向の設定
	m_worldMatrix
		= DirectX::SimpleMath::Matrix::CreateRotationX(m_bodyTilt)	// 回転の設定
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));

	// 移動を行う
	m_velocity *= Boss::BOSS_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ワールド行列の計算
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(BOSS_SCALE)			// サイズ計算
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// 位置の設定
}



// --------------------------------
//  衝突のクールタイムの計測を行う
// --------------------------------
void Boss::CheckHitCoolTime(float elapsedTime)
{	
	// クールタイムの計測を行う
	if (m_isHit && m_coolTime < COOL_TIME)
	{
		m_coolTime += elapsedTime;
	}

	// クールタイムが終了したら
	else if (m_coolTime >= COOL_TIME)
	{
		m_isHit = false;
		m_coolTime = 0.0f;
	}
}



// --------------------------------
//  表示処理
// --------------------------------
void Boss::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// 描画に必要なデータを取得する
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// 深度値を参照して書き込む
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	// 顔の描画
	m_currentFace->DrawFace(m_worldMatrix, view, projection);

	// ダメージのエフェクトを付与
	m_effect->DrawWithEffect(m_model, m_worldMatrix, view, projection);

	// 金棒の描画
	m_cudgel->Render(view, projection);
}


// --------------------------------
//  終了処理
// --------------------------------
void Boss::Finalize()
{
	// 削除用衝突判定のデータを作成
	DeleteCollisionData data = { CollisionType::Sphere, this };

	// 削除イベントを実行
	EventMessenger::Execute("DeleteCollision", &data);
}


// --------------------------------
//  全体の衝突判定イベント
// --------------------------------
void Boss::HitAction(InterSectData data)
{
	HitSword(data);
	HitStage(data);
}



// --------------------------------
//  刀との衝突判定
// --------------------------------
void Boss::HitSword(InterSectData data)
{
	if (
		!m_isHit &&
		m_canHit &&
		data.objType == ObjectType::Sword &&
		data.colType == CollisionType::OBB)
	{
		m_hp->Damage(1);
		m_isHit = true;
		m_canHit = false;

		// ヒットストップを有効にする
		HitStop::GetInstance()->SetActive();
		// エフェクトを再生
		m_effect->SetEffect(EnemyEffect::ENEMY_EFFECT::DAMAGE);

		// 画面を揺らす
		EventMessenger::Execute("CameraShake", &m_shakePower);
	}
}

// --------------------------------
//  ステージとの衝突判定
// --------------------------------
void Boss::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		m_pushBackValue = DirectX::SimpleMath::Vector3::Zero;

		// 押し戻し量を計算
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), *data.collision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0.0f;
		// 敵の位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}

// --------------------------------
//  生存確認
// --------------------------------
void Boss::CheckAlive()
{
	// HPが0以下になったら
	if (m_hp->GetHP() <= 0)
	{
		ChangeState(m_dead.get());
	}
}



// --------------------------------
// 死亡処理
// --------------------------------
void Boss::DeadAction()
{
}
