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
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Scene/PlayScene.h"
#include "../HitStop/HitStop.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Stage/Wall/Wall.h"

#include "Interface/IState.h"
#include "BehaviourTree/Header/BehaviorTree.h"	// ビヘイビアツリー

// ステートパターン用
#include "States/Header/EnemyIdling.h"			// 待機状態
#include "States/Header/Enemy_Attacking.h"		// たたきつけ攻撃
#include "States/Header/Enemy_Sweeping.h"		// 薙ぎ払い攻撃
#include "States/Header/EnemyDashAttacking.h"	// 突撃攻撃
#include "States/Header/EnemyApproaching.h"		// 追尾状態

// 顔のパーツ用
#include "Face/Header/EnemyFaceIdling.h"
#include "Face/Header/EnemyFaceAttacking.h"

// ダメージエフェクト
#include "Effects/EnemyDamageEffect/EnemyDamageEffect.h"



// 固定値
const float Enemy::ENEMY_SPEED	= 0.1f;
const float Enemy::ENEMY_SCALE	= 0.6f;
const float Enemy::COOL_TIME	= 0.3f;

// --------------------------------
//  コンストラクタ
// --------------------------------
Enemy::Enemy(PlayScene* playScene)
	: m_playScene(playScene)
	, m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity }
	, m_currentState()
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
	, m_isTargetLockOn(true)
{
}

// --------------------------------
//  デストラクタ
// --------------------------------
Enemy::~Enemy()
{
}

// --------------------------------
//  イニシャライズ
 // --------------------------------
void Enemy::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	UNREFERENCED_PARAMETER(states);

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Oni/Body/oni.cmo", *fx);

	// HPを設定
	m_hp = std::make_unique<HPSystem>(HP);

	// ビヘイビアツリーを取得
	m_pBT = std::make_unique<BehaviorTree>();

	// ステートの作成
	CreateState();

	// 顔パーツの生成
	CreateFace();

	// ダメージエフェクトの生成
	m_damageEffect = std::make_unique<EnemyDamageEffect>();

	// 当たり判定の作成
	CreateCollision();

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);
	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
}



// --------------------------------
//  状態の生成処理
// --------------------------------
void Enemy::CreateState()
{
	// === 状態の生成 ====
	m_idling		= std::make_unique<EnemyIdling>			(this);	// 待機
	m_attacking		= std::make_unique<Enemy_Attacking>		(this);	// 攻撃
	m_sweeping		= std::make_unique<Enemy_Sweeping>		(this);	// 薙ぎ払い
	m_dashAttacking = std::make_unique<EnemyDashAttacking>	(this);	// 突撃
	m_approaching	= std::make_unique<EnemyApproaching>	(this);	// 追尾

	// === 状態の初期化 ===
	m_idling		-> Initialize(m_model.get());	// 待機
	m_attacking		-> Initialize(m_model.get());	// 攻撃
	m_sweeping		-> Initialize(m_model.get());	// 薙ぎ払い
	m_dashAttacking	-> Initialize(m_model.get());	// 突撃
	m_approaching	-> Initialize(m_model.get());	// 追尾

	// 初期のステートを待機状態に割り当てる
	m_currentState = m_idling.get();
}


// --------------------------------
//  顔パーツの生成処理
// --------------------------------
void Enemy::CreateFace()
{
	// 顔パーツの生成
	m_faceIdling	= std::make_unique<EnemyFaceIdling>		(this);
	m_faceAttacking = std::make_unique<EnemyFaceAttacking>	(this);


	// 初期の顔を待機顔に割り当てる
	m_currentFace = m_faceIdling.get();
}


// --------------------------------
//  衝突判定の生成処理
// --------------------------------
void Enemy::CreateCollision()
{
	// 当たり判定の生成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, ENEMY_SCALE * 16.0f);

	// 衝突判定をMessengerに登録
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Enemy,
		this,
		m_bodyCollision.get());
}


// --------------------------------
//  状態の生成処理
// --------------------------------
void Enemy::ChangeState(IState* newState)
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
void Enemy::Update(float elapsedTime)
{
	// ステータスの更新処理
	m_currentState->Update(elapsedTime);
	// ダメージ情報の更新処理
	m_damageEffect->Update(elapsedTime);
	// ワールド行列の計算
	CalcrationWorldMatrix();
	// 当たり判定の更新
	m_bodyCollision->Center = DirectX::SimpleMath::Vector3(m_position.x, m_position.y + COLISION_POS_Y, m_position.z);
	// 衝突のクールタイムの計測
	CheckHitCoolTime(elapsedTime);



#ifdef _DEBUG

	// キー入力を受け付ける。
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1)	ChangeState(m_attacking.get());
	if (keyboardState.F2)	ChangeState(m_sweeping.get());
	if (keyboardState.F3)	ChangeState(m_dashAttacking.get());

#endif // _DEBUG
}

void Enemy::CalcrationWorldMatrix()
{
	// 回転方向の設定
	m_worldMatrix
		= DirectX::SimpleMath::Matrix::CreateRotationX(m_bodyTilt)	// 回転の設定
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle + DirectX::XMConvertToRadians(180));

	// 移動を行う
	m_velocity *= Enemy::ENEMY_SPEED;
	m_position += DirectX::SimpleMath::Vector3::Transform(m_velocity, m_worldMatrix);

	// ワールド行列の計算
	m_worldMatrix
		*= DirectX::SimpleMath::Matrix::CreateScale(ENEMY_SCALE)			// サイズ計算
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);		// 位置の設定
}



// --------------------------------
//  衝突のクールタイムの計測を行う
// --------------------------------
void Enemy::CheckHitCoolTime(float elapsedTime)
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
void Enemy::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();


	// 深度値を参照して書き込む
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	m_currentState->Render(context,states,view,projection);				// ステート側の描画
	m_currentFace->DrawFace(m_worldMatrix, view, projection);			// 顔の描画

	m_damageEffect->DrawWithDamageEffect(m_model.get(), m_worldMatrix, view, projection);	// ダメージエフェクトの描画

	//m_model->Draw(context, *states, m_worldMatrix, view, projection);	// モデルの描画

#ifdef _DEBUG
#endif // _DEBUG
}


// --------------------------------
//  終了処理
// --------------------------------
void Enemy::Finalize()
{
	GetPlayScene()->GetCollisionManager()->DeleteCollision(CollisionType::Sphere, this);
}


// --------------------------------
//  全体の衝突判定イベント
// --------------------------------
void Enemy::HitAction(InterSectData data)
{
	HitSword(data);
	HitStage(data);
}



// --------------------------------
//  刀との衝突判定
// --------------------------------
void Enemy::HitSword(InterSectData data)
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
		// 体のエフェクトを再生
		m_damageEffect->IsDamaged();
	}
}

// --------------------------------
//  ステージとの衝突判定
// --------------------------------
void Enemy::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		m_pushBackValue = DirectX::SimpleMath::Vector3::Zero;

		// 衝突したオブジェクトの情報を取得
		auto wall = dynamic_cast<Wall*>(data.object);
		DirectX::BoundingSphere* stageCollision = wall->GetCollision();

		// 押し戻し量を計算
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), *stageCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0;
		// 敵の位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}