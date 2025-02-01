// ----------------
//
// 桃太郎の挙動
//
// ----------------

#include "pch.h"
#include <Model.h>
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
#include "Libraries/MyLib/KeyboardChacker.h"
#include "Game/GameResources.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"
#include "Game/Factory/Factory.h"
#include "../Data/HPSystem.h"
#include "Game/Boss/Boss.h"
#include "Game/Goblin/Goblin.h"
#include "Game/Stage/Wall/Wall.h"


// --------------------------------
//  コンストラクタ
// --------------------------------
Player::Player(PlayScene* playScene)
	:
	m_playScene(playScene),
	m_elapsedTime{},
	m_tilt{},
	m_model{},
	m_position{ HOME_POSITION },
	m_angle{},
	m_inputVector{},
	m_acceleration{},
	m_pushBackValue{},
	m_worldMatrix{},
	m_currentState{},
	m_particleTime{},
	m_isHit{},
	m_coolTime{},
	m_canHit{false},
	m_animationRotate{},
	m_isInputMoveKey{ false }
{
}


// --------------------------------
//  デストラクタ
// --------------------------------
Player::~Player()
{
}



// --------------------------------
//  イニシャライズ
// --------------------------------
void Player::Initialize()
{
	// 描画関連の初期化
	InitializeRender();
	// 当たり判定の作成
	CreateCollision();
	// ステートの作成
	CreateState();
	// HPを管理するクラスの生成
	m_hp = std::make_unique<HPSystem>(PLAYER_HP);
	// イベントを登録
	AttachEvent();
	// 武器の生成
	m_sword = Factory::CreateSword(this);
}


// --------------------------------
// 当たり判定の作成関数
// --------------------------------
void Player::CreateCollision()
{
	// 体の当たり判定を作成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>(m_position, PLAYER_SCALE * COLLISION_RADIUS);
	
	// 衝突データの作成
	CollisionData<DirectX::BoundingSphere> data =
	{
		ObjectType::Player,		// オブジェクトの種類
		CollisionType::Sphere,	// 当たり判定形状の種類
		this,					// オブジェクトのアドレス
		m_bodyCollision.get()	// 当たり判定のアドレス
	};

	// 当たり判定を記録する
	EventMessenger::Execute("AddSphereCollision", &data);
}


// --------------------------------
//  ステートを作成関数
// --------------------------------
void Player::CreateState()
{
	//////////////////////ステートの作成////////////////////////////
	m_playerIdling		= std::make_unique<PlayerIdling>		(this);
	m_playerDodging		= std::make_unique<PlayerDodging>		(this);
	m_playerAttacking_1 = std::make_unique<PlayerAttacking_1>	(this);
	m_playerAttacking_2 = std::make_unique<PlayerAttacking_2>	(this);
	m_playerNockBacking = std::make_unique<PlayerNockBacking>	(this);

	//////////////////////ステートの初期化////////////////////////////
	m_playerIdling		->Initialize();
	m_playerDodging		->Initialize();
	m_playerAttacking_1	->Initialize();
	m_playerAttacking_2	->Initialize();
	m_playerNockBacking	->Initialize();

	// 最初のステートを設定
	m_currentState = m_playerIdling.get();
}


// --------------------------------
//  レンダリングの初期化
// --------------------------------
void Player::InitializeRender()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// モデルの取得
	m_model = GameResources::GetInstance()->GetModel("player");
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
// イベントの登録
// --------------------------------
void Player::AttachEvent()
{
	// IObject* 型のオブジェクトを取得する
	EventMessenger::AttachGetter("GetPlayerObject", std::bind(&Player::GetObject, this));
	// 衝突可能かを設定
	EventMessenger::Attach("PlayerCanHit", std::bind(&Player::CanHit, this, std::placeholders::_1));
}


// -----------------------------------------------------
// ステートを変更する
// -----------------------------------------------------
void Player::ChangeState(IPlayer* newState)
{
	// 同じステートで更新しようとすると早期リターン
	if (m_currentState == newState) return;

	m_currentState->PostUpdate();	// 事後更新処理を行う
	m_currentState = newState;		// 現在のステートを変更する
	m_currentState->PreUpdate();	// 新しいステートの事前更新を行う
}


// ---------------------------------------------------------
// 行動に対する時間を計算する関数
// ---------------------------------------------------------
void Player::TimeComparison(float& nowTime, const float totalTime, IPlayer* newState, const float elapsedTime)
{
	// 定められた時間になったら
	if (nowTime >= totalTime)
	{
		// シーンを変更
		ChangeState(newState);
		return;
	}
	// 時間の計測を行う
	nowTime += elapsedTime;
}


// ----------------------------------------------
// プレイヤーの更新処理
// ---------------------------------------------
void Player::Update(const float elapsedTime)
{
	m_elapsedTime = elapsedTime;	// 経過時間を保存する

	// ステートの更新
	m_currentState->Update(elapsedTime);

	// プレイヤーの移動

	CalculationMatrix();
	m_pushBackValue = Vector3::Zero;

	// 当たり判定の更新
	m_bodyCollision->Center = m_position;
	m_bodyCollision->Center.y = 0;

	// 武器の更新処理
	m_sword->Update(elapsedTime);

	// クールタイムを計測中
	if (m_isHit && m_coolTime < COOL_TIME)	{ m_coolTime += elapsedTime;		 }
	// クールタイム終わり
	else if (m_coolTime >= COOL_TIME)		{ m_isHit = false;m_coolTime = 0.0f; }
}


// ----------------------------------------------
// キー入力を取得する
// ----------------------------------------------
void Player::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	m_currentState->OnKeyPressed(key);
}


// ----------------------------------------------
// キー入力を取得する
// ----------------------------------------------
void Player::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	m_velocity = DirectX::SimpleMath::Vector3::Zero;

	if (KeyboardChacker::IsInputArrowKey(key))
	{
		if (!m_isInputMoveKey)
		{
			// 移動キーの入力がある場合
			m_isInputMoveKey = true;
			// リセット
			m_inputVector = DirectX::SimpleMath::Vector2::Zero;
		}

		// 移動キーに応じて入力ベクトルを設定する
		if (key == DirectX::Keyboard::Up)		m_inputVector += INPUT_FLONT;
		if (key == DirectX::Keyboard::Down)		m_inputVector += INPUT_BACK;
		if (key == DirectX::Keyboard::Left)		m_inputVector += INPUT_LEFT;
		if (key == DirectX::Keyboard::Right)	m_inputVector += INPUT_RIGHT;
	}

	m_currentState->OnKeyDown(key);
}


// ----------------------------------------------
// 回転角の計算関数
//　---------------------------------------------
float Player::CalucratePlayerRotation(DirectX::SimpleMath::Vector3 const enemyPos)
{
	// 入力がない場合は0を返す
	if (m_inputVector.LengthSquared() < FLT_EPSILON) 
	{
		return 0.0f;
	}

	// 敵の方向を取得（ラジアン単位で計算）
	float lookEnemy = Math::CalculationAngle(m_position, enemyPos);

	// 入力ベクトルを正規化
	Vector2 normalizedInput = m_inputVector;
	normalizedInput.Normalize();

	// atan2で入力ベクトルの角度を計算（ラジアン単位）
	float inputAngle = std::atan2(normalizedInput.x, normalizedInput.y);

	// 敵の方向と入力方向の差分角度を計算
	float resultAngle = lookEnemy + inputAngle;

	// 必要なら角度を0～2πの範囲に正規化
	while (resultAngle < 0) 
	{
		resultAngle += DirectX::XM_2PI; // 2πを加えて正の範囲に
	}
	
	while (resultAngle > DirectX::XM_2PI) 
	{
		resultAngle -= DirectX::XM_2PI; // 2πを引いて範囲内に
	}

	return resultAngle; // 計算結果（ラジアン単位）を返す
}




// --------------------------------
//  移動の管理
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	Vector3 moveVelocity = Vector3::Zero; // 加速用速度

	///////////////////// 移動キーの入力がない場合の処理 /////////////////
	if (m_isInputMoveKey == false)
	{
		float accelerationLength = m_acceleration.Length(); // 速度の長さを取得する
		// 0の近似値より大きい場合
		if (accelerationLength >= FLT_EPSILON)
		{
			Vector3 accelerationNormal = m_acceleration / accelerationLength; // 保持する加速度の正規化ベクトルを取得する

			float friction = 0.05f; // 摩擦量
			accelerationLength -= friction;	// 摩擦を計算

			// 加速度が（ー）になるときにリセットする
			if (accelerationLength < 0.0f)	accelerationLength = 0.0f;

			m_acceleration = accelerationNormal * accelerationLength;
			moveVelocity += m_acceleration;	// 基本速度に加速度を上書きする
		}
	}

	///////////////////// 移動キーの入力があった場合の処理 /////////////////
	else
	{
		// 基本移動量を計算する
		moveVelocity += Vector3::Forward * PLAYER_SPEED;

		float acceleration = 0.05f; // 加速度
		m_acceleration += Vector3::Forward * acceleration; // 加速度の計算を行う

		// 2乗にすることで符号を外す
		if (m_acceleration.LengthSquared() > 1.0f)
		{
			m_acceleration.Normalize(); // 上限を1に設定する
		}

		moveVelocity += m_acceleration; // 基本移動に加速度を上乗せする
		m_velocity = moveVelocity; // 速度を保存する
	}

	moveVelocity *= PLAYER_SPEED;
	/////////////////////////// 移動処理 //////////////////////////////////
	m_position += Vector3::Transform(moveVelocity, Matrix::CreateRotationY(-m_angle));


	/////////////////////////// パーティクルの生成 //////////////////////////
	if (moveVelocity != Vector3::Zero)
	{
		m_particleTime += m_elapsedTime;
		if (m_particleTime >= 0.15f)
		{
			EventMessenger::Execute("CreateTrailDust", nullptr);
			m_particleTime = 0.0f;
		}
	}

	m_isInputMoveKey = false;	// 移動キーの入力をリセットする
}


// --------------------------------
//  ワールド行列の計算
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	// 行列の計算を行う
	m_worldMatrix = Matrix::Identity;	// 更新ごとに初期化を行う
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3::Zero)							// 原点に移動
		*= Matrix::CreateScale		(PLAYER_SCALE)							// プレイヤーのサイズ変更

		*= Matrix::CreateRotationZ(m_animationRotate.z)
		*= Matrix::CreateRotationX(m_animationRotate.x)
		*= Matrix::CreateRotationY(m_animationRotate.y)

		*= Matrix::CreateRotationY	(-m_angle + XMConvertToRadians(180.f))	// 敵の方向を見るように設定する
		*= Matrix::CreateTranslation(m_position);							// 座標を移動させる

}


// --------------------------------
//  表示処理
// --------------------------------
void Player::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルのエフェクト情報を更新する
	m_model->UpdateEffects([](DirectX::IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// ライトを有効化する
				basicEffect->SetLightingEnabled(true);
				// アンビエントライトの設定
				basicEffect->SetAmbientLightColor(DirectX::Colors::Gray); // 適切なアンビエント色を設定
				// 必要に応じてライトの設定を行う
				basicEffect->SetLightEnabled(0, true);
				basicEffect->SetLightDiffuseColor(0, DirectX::Colors::White);  // ディフューズ色を設定
				basicEffect->SetLightSpecularColor(0, DirectX::Colors::White); // スペキュラー色を設定

				basicEffect->SetLightEnabled(1, false); // 追加のライトが不要なら無効化
				basicEffect->SetLightEnabled(2, false); // 追加のライトが不要なら無効化
				// モデルの自発光色を黒に設定して無効化
				basicEffect->SetEmissiveColor(DirectX::Colors::Black);
			}
		}
	);

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
	
	// 武器を描画する
	m_sword->Render(view, projection);
#ifdef _DEBUG
#endif // !_DEBUG

}


// --------------------------------
//  終了処理
// --------------------------------
void Player::Finalize()
{
}


// --------------------------------
//  衝突処理
// --------------------------------
void Player::HitAction(InterSectData data)
{
	// 体との当たり判定
	HitBossBody(data);

	// 武器との当たり判定
	HitCudgel(data);

	// ステージとの当たり判定
	HitStage(data);

	// ゴブリンとの当たり判定
	HitGoblin(data);
}


// --------------------------------
// ダメージ処理
// --------------------------------
void Player::Damage(float damage)
{
	// HPを減らす
	m_hp->Damage(damage);
	m_isHit = true;
	m_canHit = false;
	// ノックバックをする
	ChangeState(m_playerNockBacking.get());
	// 効果音を鳴らす
	Sound::GetInstance()->PlaySE(Sound::SE_TYPE::PLAYER_DAMAGED);
}


// --------------------------------
//  敵の体との衝突判定
// --------------------------------
void Player::HitBossBody(InterSectData data)
{
	if (data.objType == ObjectType::Boss && data.colType == CollisionType::Sphere)
	{
		// 敵のステートがダッシュ攻撃の場合で相手が攻撃中の場合
		if (!m_isHit &&
			m_canHit)
		{
			Damage(1);
		}

		// 衝突したオブジェクトの情報を取得
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		DirectX::BoundingSphere enemyCollision = *data.collision;

		// 押し戻し量を計算
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, enemyCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}


// ---------------------------------
// ゴブリンとの衝突
// ---------------------------------
void Player::HitGoblin(InterSectData data)
{
	if (data.objType == ObjectType::Goblin && data.colType == CollisionType::Sphere)
	{
		// 敵のステートがダッシュ攻撃の場合で相手が攻撃中の場合
		if (!m_isHit &&
			m_canHit)
		{
			Damage(1);
		}

		// 衝突したオブジェクトの情報を取得
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		DirectX::BoundingSphere goblinCollision = *data.collision;

		// 押し戻し量を計算
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, goblinCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}


// --------------------------------
// 敵の武器（金棒）との衝突判定
// --------------------------------
void Player::HitCudgel(InterSectData data)
{
	if (!m_isHit &&
		m_canHit &&
		data.objType == ObjectType::Cudgel &&
		data.colType == CollisionType::OBB &&
		m_currentState != m_playerDodging.get()
		)
	{
		Damage(1);
	}
}


// --------------------------------
// ステージとの衝突判定
// --------------------------------
void Player::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		// 衝突したオブジェクトの情報を取得
		DirectX::BoundingSphere stageCollision = *data.collision;

		// 押し戻し量を計算
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), stageCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}


// --------------------------------
//  衝突可能か
// --------------------------------
void Player::CanHit(void* flag)
{
	m_canHit = *(bool*)flag;
}