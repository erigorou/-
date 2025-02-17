// -----------------------------------------------------
// 名前:	Boss.cpp
// 内容:	ボスを実装するクラス
//			ステートや衝突判定を管理
// 作成:	池田桜輔
// -----------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Math.h"
#include "../HitStop/HitStop.h"
#include "Game/GameResources.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Interface/IState.h"
#include "Game/Factory/Factory.h"
#include "Effects/EnemyEffect/EnemyEffect.h"
// ステート
#include "States/Header/BossStarting.h"
#include "States/Header/BossIdling.h"
#include "States/Header/BossAttacking.h"
#include "States/Header/BossSweeping.h"
#include "States/Header/BossDashAttacking.h"
#include "States/Header/BossApproaching.h"
#include "States/Header/BossDead.h"
// 顔
#include "Face/Header/BossFaceIdling.h"
#include "Face/Header/BossFaceAttacking.h"

// --------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// --------------------------------
Boss::Boss()
	:
	m_worldMatrix{ DirectX::SimpleMath::Matrix::Identity },
	m_currentState{}
	, m_idling{}
	, m_attacking{}
	, m_approaching{}
	, m_position{}
	, m_angle{}
	, m_bodyTilt{}
	, m_pushBackValue{}
	, m_canHit(false)
	, m_shakePower{ SHAKE_POWER }
	, m_currentFace{}
	, m_model{}
{
}

// --------------------------------
/// <summary>
/// デストラクタ
/// </summary>
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
/// <summary>
/// 初期化処理
/// </summary
// --------------------------------
void Boss::Initialize()
{
	// 武器の生成
	m_cudgel = Factory::CreateCudgel(this);
	// モデルを取得
	m_model = GameResources::GetInstance()->GetModel("boss");
	// HPを設定
	m_hp = std::make_unique<HPSystem>(HP);
	// ステートの作成
	CreateState();
	// 顔パーツの生成
	CreateFace();
	// エフェクトの生成
	m_effect = std::make_unique<EnemyEffect>();
	// 当たり判定の作成
	CreateCollision();
	// イベントの登録
	AttachEvent();
}

// --------------------------------
/// <summary>
/// ステートの生成処理
/// </summary>
// --------------------------------
void Boss::CreateState()
{
	// 開始
	m_starting = std::make_unique<BossStarting>(this);
	m_starting->Initialize();
	m_states.push_back(m_starting.get());

	// 待機
	m_idling = std::make_unique<BossIdling>(this);
	m_idling->Initialize();
	m_states.push_back(m_idling.get());

	// 攻撃
	m_attacking = std::make_unique<BossAttacking>(this);
	m_attacking->Initialize();
	m_states.push_back(m_attacking.get());

	// 薙ぎ払い
	m_sweeping = std::make_unique<BossSweeping>(this);
	m_sweeping->Initialize();
	m_states.push_back(m_sweeping.get());

	// 突撃
	m_dashAttacking = std::make_unique<BossDashAttacking>(this);
	m_dashAttacking->Initialize();
	m_states.push_back(m_dashAttacking.get());

	// 追尾
	m_approaching = std::make_unique<BossApproaching>(this);
	m_approaching->Initialize();
	m_states.push_back(m_approaching.get());

	// 死亡
	m_dead = std::make_unique<BossDead>(this);
	m_dead->Initialize();
	m_states.push_back(m_dead.get());

	// 初期のステートを待機状態に割り当てる
	m_currentState = m_starting.get();
}

// --------------------------------
/// <summary>
/// 顔の生成処理
/// </summary>
// --------------------------------
void Boss::CreateFace()
{
	// 通常状態
	m_faceIdling = std::make_unique<BossFaceIdling>(this);
	m_faces.push_back(m_faceIdling.get());
	// 攻撃状態
	m_faceAttacking = std::make_unique<BossFaceAttacking>(this);
	m_faces.push_back(m_faceAttacking.get());
	// 初期の顔を待機顔に割り当てる
	m_currentFace = m_faceIdling.get();
}

// --------------------------------
/// <summary>
/// 当たり判定の生成処理
/// </summary>
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
	EventMessenger::Execute(EventList::AddSphereCollision, &data);
}

// --------------------------------
/// <summary>
/// ステートの更新処理
/// </summary>
/// <param name="state">次のステート</param>
// --------------------------------
void Boss::ChangeState(void* state)
{
	// int型に変換
	int index = *static_cast<int*>(state);

	// ステートの変更
	if (m_currentState == m_states[index]) return;

	// 新規の状態遷移前に事後更新を行う
	m_currentState->PostUpdate();
	// 新規の状態を現在の状態に設定する
	m_currentState = m_states[index];
	// 新規の状態遷移後に事前更新を行う
	m_currentState->PreUpdate();
}

// --------------------------------
/// <summary>
/// 顔の変更処理
/// </summary>
/// <param name="face">次の顔</param>
// --------------------------------
void Boss::ChangeFace(void* face)
{
	// int型に変換
	int index = *static_cast<int*>(face);
	// 新規の状態を現在の状態に設定する
	m_currentFace = m_faces[index];
}

// --------------------------------
/// <summary>
/// イベントの登録
/// </summary>
// --------------------------------
void Boss::AttachEvent()
{
	// 状態変更のイベント
	EventMessenger::Attach(EventList::ChangeBossState, std::bind(&Boss::ChangeState, this, std::placeholders::_1));
	// 顔変更のイベント
	EventMessenger::Attach(EventList::ChangeBossFace, std::bind(&Boss::ChangeFace, this, std::placeholders::_1));
}

// --------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
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
	// 生存確認
	CheckAlive();
	// 武器の更新
	m_cudgel->Update(elapsedTime);

#ifdef _DEBUG

	// デバッグ中なら F1~F3でステートを変更
	DirectX::Keyboard::State keyboardState = DirectX::Keyboard::Get().GetState();

	if (keyboardState.F1) {
		BossState state = BossState::Attacking;
		ChangeState(&state);
	}
	if (keyboardState.F2) {
		BossState state = BossState::Sweeping;
		ChangeState(&state);
	}
	if (keyboardState.F3) {
		BossState state = BossState::DashAttacking;
		ChangeState(&state);
	}

#endif // _DEBUG
}

// --------------------------------
/// <summary>
/// ワールド行列の計算
/// </summary>
// --------------------------------
void Boss::CalcrationWorldMatrix()
{
	using namespace DirectX::SimpleMath;

	// ボスの回転（X軸の傾き + Y軸回転）
	Quaternion bodyTiltRotation = Quaternion::CreateFromYawPitchRoll(
		-m_angle + DirectX::XMConvertToRadians(180.f),
		m_bodyTilt,
		0.0f
	);

	// 回転行列の適用
	m_worldMatrix = Matrix::CreateFromQuaternion(bodyTiltRotation);

	// 移動を適用（速度を回転後の方向に変換）
	m_velocity *= Boss::BOSS_SPEED;
	m_position += Vector3::Transform(m_velocity, m_worldMatrix);

	// スケールと平行移動を適用
	m_worldMatrix *= Matrix::CreateScale(BOSS_SCALE);	// サイズ調整
	m_worldMatrix *= Matrix::CreateTranslation(m_position);	// 位置設定
}

// --------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
// --------------------------------
void Boss::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
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
/// <summary>
/// 終了処理
/// </summary>
// --------------------------------
void Boss::Finalize()
{
	// 削除用衝突判定のデータを作成
	DeleteCollisionData data = { CollisionType::Sphere, this };
	// 削除イベントを実行
	EventMessenger::Execute(EventList::DeleteCollision, &data);
}

// --------------------------------
/// <summary>
/// 衝突判定
/// </summary>
/// <param name="data">衝突データ</param>
// --------------------------------
void Boss::HitAction(InterSectData data)
{
	HitSword(data);
	HitStage(data);
}


// --------------------------------
/// <summary>
/// 剣との衝突判定
/// </summary>
/// <param name="data">衝突データ</param>
// --------------------------------
void Boss::HitSword(InterSectData data)
{
	if (
		m_canHit &&
		data.objType == ObjectType::Sword &&
		data.colType == CollisionType::OBB)
	{
		m_hp->Damage(1);
		m_canHit = false;

		// ヒットストップを有効にする
		HitStop::GetInstance()->SetActive();
		// エフェクトを再生
		m_effect->SetEffect(EnemyEffect::ENEMY_EFFECT::DAMAGE);

		// 画面を揺らす
		EventMessenger::Execute(EventList::ShakeCamera, &m_shakePower);
	}
}


// --------------------------------
/// <summary>
/// ステージとの衝突判定
/// </summary>
/// <param name="data">衝突データ</param>
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
/// <summary>
/// 生存判定
/// </summary>
// --------------------------------
void Boss::CheckAlive()
{
	// HPが0以下になったら
	if (m_hp->GetHP() <= 0)
	{
		// 死亡状態に変更
		BossState state = BossState::Dead;
		ChangeState(&state);

		// 金棒を非表示に
		m_cudgel->BossDead();
	}
}

// --------------------------------
/// <summary>
/// 死亡時の処理
/// </summary>
// --------------------------------
void Boss::DeadAction()
{
}