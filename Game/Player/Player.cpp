// --------------------------------------------------
// 名前:	Player.h
// 内容:	プレイヤークラス
//			プレイヤーのステートや衝突判定、描画やアニメーション処理
// 作成:	池田桜輔
// --------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
#include "Libraries/MyLib/KeyboardChacker.h"
#include "Game/GameResources.h"
#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"
#include "Game/Factory/Factory.h"
#include "../Data/HPSystem.h"

#include "Libraries/MyLib/DebugString.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
Player::Player()
	:
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
	m_canHitBoss{ false },
	m_canHitCudgel{ false },
	m_canHitGoblin{ false },
	m_animationRotate{},
	m_isInputMoveKey{ false }
{
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
Player::~Player()
{
	Finalize();
}

// ---------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// ---------------------------------------------------------
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

// ---------------------------------------------------------
/// <summary>
/// 衝突判定の登録
/// </summary>
// ---------------------------------------------------------
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
	EventMessenger::Execute(EventList::AddSphereCollision, &data);
}

// ---------------------------------------------------------
/// <summary>
/// ステートの作成・初期化・登録
/// </summary>
// ---------------------------------------------------------
void Player::CreateState()
{
	// 待機状態の作成・初期化・登録
	m_playerIdling = std::make_unique<PlayerIdling>(this);
	m_playerIdling->Initialize();
	m_states[static_cast<int>(PlayerState::Idling)] = m_playerIdling.get();

	// 回避状態の作成・初期化・登録
	m_playerDodging = std::make_unique<PlayerDodging>(this);
	m_playerDodging->Initialize();
	m_states[static_cast<int>(PlayerState::Dodging)] = m_playerDodging.get();

	// 攻撃状態の作成・初期化・登録
	m_playerAttacking1 = std::make_unique<PlayerAttacking1>(this);
	m_playerAttacking1->Initialize();
	m_states[static_cast<int>(PlayerState::Attacking1)] = m_playerAttacking1.get();

	// 攻撃状態の作成・初期化・登録
	m_playerAttacking2 = std::make_unique<PlayerAttacking2>(this);
	m_playerAttacking2->Initialize();
	m_states[static_cast<int>(PlayerState::Attacking2)] = m_playerAttacking2.get();

	// やられ状態の作成・初期化・登録
	m_playerNockBacking = std::make_unique<PlayerNockBacking>(this);
	m_playerNockBacking->Initialize();
	m_states[static_cast<int>(PlayerState::NockBacking)] = m_playerNockBacking.get();

	// 最初のステートを設定
	m_currentState = m_playerIdling.get();
}

// ---------------------------------------------------------
/// <summary>
/// レンダリング系の初期化
/// </summary>
// ---------------------------------------------------------
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

// ---------------------------------------------------------
/// <summary>
/// イベントの登録
/// </summary>
// ---------------------------------------------------------
void Player::AttachEvent()
{
	// IObject* 型のオブジェクトを取得する
	EventMessenger::AttachGetter(GetterList::GetPlayer, std::bind(&Player::GetObject, this));
	// ボスがプレイヤーに攻撃をしているか
	EventMessenger::Attach(EventList::PlayerCanDamageBoss, std::bind(&Player::CanHitBoss, this, std::placeholders::_1));
	// 金棒がプレイヤーに攻撃をしているか
	EventMessenger::Attach(EventList::PlayerCanDamageCudgel, std::bind(&Player::CanHitCudgel, this, std::placeholders::_1));
	// ゴブリンがプレイヤーに攻撃をしているか
	EventMessenger::Attach(EventList::PlayerCanDamageGoblin, std::bind(&Player::CanHitGoblin, this, std::placeholders::_1));
}


// ---------------------------------------------------------
/// <summary>
/// プレイヤーのステートを変更する
/// </summary>
/// <param name="state">変更後のステート</param>
// ---------------------------------------------------------
void Player::ChangeState(PlayerState state)
{
	int stateIndex = static_cast<int>(state);

	// 現在のステートと同じなら処理なし
	if (m_currentState == m_states[stateIndex]) return;

	// 事後更新処理を行う
	m_currentState->PostUpdate();
	// 現在のステートを変更する
	m_currentState = m_states[stateIndex];
	// 新しいステートの事前更新を行う
	m_currentState->PreUpdate();
}

// ---------------------------------------------------------
/// <summary>
/// 一定時間でステートを変更する
/// </summary>
/// <param name="nowTime">現在の時間</param>
/// <param name="totalTime">変更する時間</param>
/// <param name="state">変更後のステート</param>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void Player::TimeComparison(float& nowTime, const float totalTime, PlayerState state, const float elapsedTime)
{
	// 定められた時間になったら
	if (nowTime >= totalTime)
	{
		// 新しいステートに変更する
		ChangeState(state);
		return;
	}
	// 時間の計測を行う
	nowTime += elapsedTime;
}

// ---------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void Player::Update(const float elapsedTime)
{
	// 経過時間を保存するg
	m_elapsedTime = elapsedTime;
	// ステートの更新
	m_currentState->Update(elapsedTime);
	// プレイヤーの移動
	CalculationMatrix();
	// 当たり判定の更新
	m_bodyCollision->Center = m_position;
	m_bodyCollision->Center.y = 0.0f;
	// 武器の更新処理
	m_sword->Update(elapsedTime);
	// クールタイムを計測する
	CalculationCoolTime(elapsedTime);
}

// ---------------------------------------------------------
/// <summary>
/// クールタイムを計測する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------------------
void Player::CalculationCoolTime(const float elapsedTime)
{
	// クールタイムが起動中で、クールタイムが終わっていない場合
	if (m_isHit && m_coolTime < COOL_TIME) {
		m_coolTime += elapsedTime;
	}
	// クールタイム経過後にリセット
	else if (m_coolTime >= COOL_TIME) { 
		m_isHit = false; m_coolTime = 0.0f;
	}
}

// ---------------------------------------------------------
/// <summary>
/// キー入力を取得する(押した瞬間)
/// </summary>
/// <param name="key">押されたキー</param>
// // ---------------------------------------------------------
void Player::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	// ステートにもキー入力を伝達
	m_currentState->OnKeyPressed(key);
}

// ---------------------------------------------------------
/// <summary>
/// キー入力を取得する(押されている間)
/// </summary>
/// <param name="key">押されたキー</param>
// // ---------------------------------------------------------
void Player::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	// 移動ベクトルをリセット
	m_velocity = DirectX::SimpleMath::Vector3::Zero;

	// 矢印キーの入力がある場合
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
	// ステートにもキー入力を伝達
	m_currentState->OnKeyDown(key);
}

// ---------------------------------------------------------
/// <summary>
/// プレイヤーの回転の計算を行う
/// </summary>
/// <param name="enemyPos">ターゲットの座標</param>
/// <returns>計算結果</returns>
// ---------------------------------------------------------
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

// ---------------------------------------------------------
/// <summary>
/// 移動方向を計算する
/// </summary>
// ---------------------------------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 加速用速度
	Vector3 moveVelocity = Vector3::Zero;

	///////////////////// 移動キーの入力がない場合の処理 /////////////////
	if (m_isInputMoveKey == false)
	{
		// 速度の長さを取得する
		float accelerationLength = m_acceleration.Length();
		// 0の近似値より大きい場合
		if (accelerationLength >= FLT_EPSILON)
		{
			// 保持する加速度の正規化ベクトルを取得する
			Vector3 accelerationNormal = m_acceleration / accelerationLength;
			// 摩擦を計算
			accelerationLength -= PLAYER_FRICTION;
			// 加速度が（ー）になるときにリセットする
			if (accelerationLength < 0.0f)	accelerationLength = 0.0f;
			// 加速度を再計算する
			m_acceleration = accelerationNormal * accelerationLength;
			// 速度を計算する
			moveVelocity += m_acceleration;
		}
	}

	///////////////////// 移動キーの入力があった場合の処理 /////////////////
	else
	{
		// 左右キー同時押しのような場合は、処理なし
		if (m_inputVector.Length() <= FLT_EPSILON)
			return;

		// 基本移動量を計算する
		moveVelocity += Vector3::Forward * PLAYER_SPEED;
		// 加速度の計算を行う
		m_acceleration += Vector3::Forward * PLAYER_ACCELERATION;

		// 2乗にすることで符号を外す
		if (m_acceleration.LengthSquared() > 1.0f){
			// 上限を1に設定する
			m_acceleration.Normalize();
		}
		// 基本移動に加速度を上乗せする
		moveVelocity += m_acceleration;
		// 速度を保存する
		m_velocity = moveVelocity;
	}

	/////////////////////////// 移動処理 //////////////////////////////////
	// 加速度を計算する
	moveVelocity *= PLAYER_SPEED;
	// 移動後の座標を計算する
	m_position += Vector3::Transform(moveVelocity, Matrix::CreateRotationY(-m_angle));

	/////////////////////////// パーティクルの生成 //////////////////////////
	if (moveVelocity != Vector3::Zero)
	{
		// 経過時間を記録
		m_particleTime += m_elapsedTime;
		// 一定時間経過後にパーティクルを生成する
		if (m_particleTime >= PARTICLE_INTERVAL)
		{
			// パーティクルの生成イベント
			EventMessenger::Execute(EventList::CreateTrailDust, nullptr);
			// 時間をリセット
			m_particleTime = 0.0f;
		}
	}
	// 移動キーの入力をリセットする
	m_isInputMoveKey = false;
}

// ---------------------------------------------------------
/// <summary>
/// ワールド行列の計算を行う
/// </summary>
// ---------------------------------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX::SimpleMath;

	// ワールド行列を初期化
	m_worldMatrix = Matrix::Identity;
	// スケールの適用
	m_worldMatrix *= Matrix::CreateScale(PLAYER_SCALE);
	// アニメーション回転の適用（ヨー・ピッチ・ロール）
	Quaternion animationRotation = Quaternion::CreateFromYawPitchRoll(
		m_animationRotate.y,
		m_animationRotate.x,
		m_animationRotate.z
	);
	// プレイヤーの方向調整（敵の方向を見る回転）
	Quaternion directionRotation = Quaternion::CreateFromYawPitchRoll(
		-m_angle + DirectX::XMConvertToRadians(180.0f),
		0.0f,
		0.0f
	);
	// 回転を適用
	Quaternion totalRotation = animationRotation * directionRotation;
	m_worldMatrix *= Matrix::CreateFromQuaternion(totalRotation);

	// 位置を適用（最後に平行移動）
	m_worldMatrix *= Matrix::CreateTranslation(m_position);
	// 押し戻し量をリセット
	m_pushBackValue = Vector3::Zero;
}

// ---------------------------------------------------------
/// <summary>
/// モデルを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
// ---------------------------------------------------------
void Player::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
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
}

// ---------------------------------------------------------
/// <summary>
/// 終了処理を行う
/// </summary>
// ---------------------------------------------------------
void Player::Finalize()
{
	// 衝突判定を削除するための構造体
	DeleteCollisionData data = { CollisionType::Sphere, this };
	// 当たり判定の削除
	EventMessenger::Execute(EventList::DeleteCollision, &data);

	// ステート配列の解放
	for (int i = 0; i < STATE_MAX; i++)
	{
		m_states[i]->Finalize();
		m_states[i] = nullptr;
	}
}

// ---------------------------------------------------------
/// <summary>
/// 衝突したときのイベント
/// </summary>
/// <param name="data">相手のデータ</param>
// ---------------------------------------------------------
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

// ---------------------------------------------------------
/// <summary>
/// ダメージ処理
/// </summary>
/// <param name="damage">受けたダメージ量</param>
// ---------------------------------------------------------
void Player::Damage(float damage)
{
	// HPを減らす
	m_hp->Damage(damage);
	m_isHit = true;
	m_canHitBoss = false;
	m_canHitCudgel = false;
	m_canHitGoblin = false;
	// ノックバックをする
	ChangeState(PlayerState::NockBacking);
	// 効果音を鳴らす
	Sound::GetInstance()->PlaySE(Sound::SE_TYPE::PLAYER_DAMAGED);
}

// ---------------------------------------------------------
/// <summary>
/// ボスとの衝突イベント
/// </summary>
/// <param name="data">衝突相手のデータ</param>
// ---------------------------------------------------------
void Player::HitBossBody(InterSectData data)
{
	if (data.objType == ObjectType::Boss && data.colType == CollisionType::Sphere)
	{
		// 敵のステートがダッシュ攻撃の場合で相手が攻撃中の場合
		if (! m_isHit && m_canHitBoss){
			Damage(BOSS_ATTACK_POWER);
		}

		// 衝突したオブジェクトの情報を取得
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		DirectX::BoundingSphere enemyCollision  = *data.collision;

		// 押し戻し量を計算
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, enemyCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0.0f;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}

// ---------------------------------------------------------
/// <summary>
/// 小鬼の衝突イベント
/// </summary>
/// <param name="data">衝突相手のデータ</param>
// ---------------------------------------------------------
void Player::HitGoblin(InterSectData data)
{
	if (data.objType == ObjectType::Goblin && data.colType == CollisionType::Sphere)
	{
		// 敵のステートがダッシュ攻撃の場合で相手が攻撃中の場合
		if (!m_isHit && m_canHitGoblin) {
			Damage(GOBLIN_ATTACK_POWER);
		}

		// 衝突したオブジェクトの情報を取得
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		DirectX::BoundingSphere goblinCollision = *data.collision;

		// 押し戻し量を計算
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, goblinCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0.0f;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}
// ---------------------------------------------------------
/// <summary>
/// ボスの武器(金棒)の衝突イベント
/// </summary>
/// <param name="data">衝突相手のデータ</param>
// ---------------------------------------------------------
void Player::HitCudgel(InterSectData data)
{
	// 金棒が攻撃状態かつ、クールタイム中でない場合
	if (!m_isHit &&
		m_canHitCudgel &&
		data.objType == ObjectType::Cudgel &&
		data.colType == CollisionType::OBB &&
		m_currentState != m_playerDodging.get()
		)
	{
		Damage(BOSS_CUDGEL_POWER);
	}
}

// ---------------------------------------------------------
/// <summary>
/// ステージとの衝突イベント
/// </summary>
/// <param name="data">衝突相手のデータ</param>
// ---------------------------------------------------------
void Player::HitStage(InterSectData data)
{
	if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		// 衝突したオブジェクトの情報を取得
		DirectX::BoundingSphere stageCollision = *data.collision;
		// 押し戻し量を計算
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), stageCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0.0f;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}
}

// ---------------------------------------------------------
/// <summary>
/// ボスがプレイヤーに対して攻撃ができるのか
/// </summary>
/// <param name="flag">フラグ</param>
// ---------------------------------------------------------
void Player::CanHitBoss(void* flag)
{
	m_canHitBoss = *(bool*)flag;
}

// ---------------------------------------------------------
/// <summary>
/// ボスの武器(金棒)がプレイヤーに対して攻撃ができるのか
/// </summary>
/// <param name="flag">フラグ</param>
// ---------------------------------------------------------
void Player::CanHitCudgel(void* flag)
{
	m_canHitCudgel = *(bool*)flag;
}

// ---------------------------------------------------------
/// <summary>
/// 小鬼がプレイヤーに対して攻撃ができるのか
/// </summary>
/// <param name="flag">フラグ</param>
// ---------------------------------------------------------
void Player::CanHitGoblin(void* flag)
{
	m_canHitCudgel = *(bool*)flag;
}