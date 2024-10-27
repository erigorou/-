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
#include "Libraries/Microsoft/DebugDraw.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Player/Player.h"
#include "Game/Weapon/Sword/Sword.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Stage/Wall/Wall.h"


// ここで静的メンバー変数を定義する
const DirectX::SimpleMath::Vector3 Player::HOME_POSITION(0.0f);
const float Player::PLAYER_SPEED = 0.01f;
const float Player::PLAYER_SCALE = 0.1f;


// --------------------------------
//  コンストラクタ
// --------------------------------
Player::Player(PlayScene* playScene)
	:
	m_playScene(playScene),
	m_model{},
	m_position{ 0, 0, 40 },
	m_angle{ 0.f },
	m_acceleration{},
	m_pushBackValue{},
	m_worldMatrix{},
	m_currentState{},
	m_keyboardState{},
	m_tracker{},
	m_particleTime{},
	m_isHit{},
	m_coolTime{},
	m_canHit{false}
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
	using namespace DirectX;
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// キー入力を生成（全体で使う）
	m_keyboardState = DirectX::Keyboard::Get().GetState();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/momotaro.cmo", *fx);

	// 当たり判定の作成
	CreateCollision();

	// ステートの作成
	CreateState();

	// HPを管理するクラスの生成
	m_hp = std::make_unique<PlayerHP>();

	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);
	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);

}


// --------------------------------
// 当たり判定の作成関数
// --------------------------------
void Player::CreateCollision()
{
	// 体の当たり判定を作成
	m_bodyCollision = std::make_unique<DirectX::BoundingSphere>
		(
			m_position, PLAYER_SCALE * 12
		);

	
	// 当たり判定を記録する
	m_playScene->GetCollisionManager()->AddCollision(
		ObjectType::Player,		// オブジェクトの種類
		this,					// オブジェクトのアドレス
		m_bodyCollision.get()	// 当たり判定のアドレス
	);
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
	m_playerAttacking_3 = std::make_unique<PlayerAttacking_3>	(this);
	m_playerAttacking_4 = std::make_unique<PlayerAttacking_4>	(this);

	//////////////////////ステートの初期化////////////////////////////
	m_playerIdling		->Initialize(m_model.get());
	m_playerDodging		->Initialize(m_model.get());
	m_playerAttacking_1	->Initialize(m_model.get());
	m_playerAttacking_2	->Initialize(m_model.get());
	m_playerAttacking_3	->Initialize(m_model.get());
	m_playerAttacking_4	->Initialize(m_model.get());

	// 最初のステートを設定
	m_currentState = m_playerIdling.get();
}


/// <summary>
/// ステートを変更する
/// </summary>
/// <param name="newState">変更したいステート</param>
void Player::ChangeState(IPlayer* newState)
{
	// 同じステートで更新しようとすると早期リターン
	if (m_currentState == newState) return;

	m_currentState->PostUpdate();	// 事後更新処理を行う
	m_currentState = newState;		// 現在のステートを変更する
	m_currentState->PreUpdate();	// 新しいステートの事前更新を行う
}


// ---------------------------------------------------------
/// <summary>
/// 行動に対する時間を計算する関数
/// </summary>
/// <param name="nowTime">現在の時間</param>
/// <param name="totalTime">ステートを変更する時間</param>
/// <param name="newState">変更する時間</param>
/// <param name="elapsedTime">経過時間</param>
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
/// <summary>
/// プレイヤーの更新処理
/// </summary>
/// <param name="enemyPos">敵の座標</param>
/// <param name="elapsedTime">経過時間</param>
// ---------------------------------------------
void Player::Update(const DirectX::SimpleMath::Vector3 enemyPos,const float elapsedTime)
{
	m_elapsedTime = elapsedTime;	// 経過時間を保存する

	////////////////////ステートの更新/////////////////////////////
	m_currentState->Update(elapsedTime);

	///////////////////プレイヤーの移動////////////////////////////
	m_angle = Math::CalculationAngle(m_position, enemyPos);
	CalculationMatrix();
	m_pushBackValue = Vector3::Zero;
	///////////////////当たり判定の更新////////////////////////////
	m_bodyCollision->Center = m_position;

	// クールタイムを計測中
	if (m_isHit && m_coolTime < COOL_TIME)	{ m_coolTime += elapsedTime;		 }
	// クールタイム終わり
	else if (m_coolTime >= COOL_TIME)		{ m_isHit = false;m_coolTime = 0.0f; }
}


// ----------------------------------------------
/// <summary>
/// キー入力を取得する
/// </summary>
/// <param name="key">入力された1キー</param>
// ----------------------------------------------
void Player::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
	m_currentState->OnKeyPressed(key);
}


void Player::OnKeyDown(const DirectX::Keyboard::Keys& key)
{
	if (m_currentState == this->GetPlayerIdlingState())
	{
		// 移動
		if (key == DirectX::Keyboard::Up)		m_inputVelocity += Vector3::Forward;	// 「↑」で前進
		if (key == DirectX::Keyboard::Down)		m_inputVelocity += Vector3::Backward;	// 「↓」で後退
		if (key == DirectX::Keyboard::Left)		m_inputVelocity += Vector3::Left;		// 「←」で左移動
		if (key == DirectX::Keyboard::Right)	m_inputVelocity += Vector3::Right;		// 「→」で右移動
	}

	m_currentState->OnKeyDown(key);
}



// ----------------------------------------------
/// <summary>
/// 回転角の計算関数
/// </summary>
/// <param name="enemyPos"></param>
//　---------------------------------------------
void Player::CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos)
{
	using namespace DirectX::SimpleMath;

	Vector3 forward = m_position - enemyPos;		// 敵の方向をベクトルで取得
	forward.Normalize();							// 正規化

	Vector3 worldForward = Vector3::Forward;		// ワールド座標の前方ベクトルを作成
	float dotProduct = forward.Dot(worldForward);	// 内積を取得
	m_angle = acosf(dotProduct);					// 内積から角度を取得(弧度法)

	Vector3 crossProduct = forward.Cross(worldForward);	// カメラの前方向ベクトルが右方向に向いているかどうかで符号を決定6
	m_angle = (crossProduct.y < 0) ? -m_angle: m_angle;	// -180 ~ 180に収める。
}



// --------------------------------
//  移動の管理
// --------------------------------
void Player::MovePlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// キー入力を受け付ける。
	Keyboard::State keyboardState = Keyboard::Get().GetState();

	Vector3 moveVelocity  = Vector3::Zero;	// 加速用速度


	m_inputVelocity.Normalize();
	m_direction = m_inputVelocity;
	

	///////////////////// 移動キーの入力がない場合の処理 /////////////////
	if (m_inputVelocity == Vector3::Zero)
	{
		float accelerationLength = m_acceleration.Length();				// 速度の長さを取得する
		// 0の近似値より大きい場合
		if (accelerationLength >= FLT_EPSILON)
		{
			Vector3 accelerationNormal = m_acceleration / accelerationLength;// 保持する加速度の正規化ベクトルを取得する

			float friction = 0.1f;			// 摩擦量
			accelerationLength -= friction;	// 摩擦を計算

			// 加速度が（ー）になるときにリセットする
			if (accelerationLength < 0.0f)	accelerationLength = 0.0f;

			m_acceleration = accelerationNormal * accelerationLength;
			moveVelocity += m_acceleration;								// 基本速度に加速度を上書きする
		}
	}

	///////////////////// 移動キーの入力があった場合の処理 /////////////////
	else
	{
		// 基本移動量を計算する
		moveVelocity += m_inputVelocity * PLAYER_SPEED;

		float acceleration = 0.05f;							// 加速度
		m_acceleration += m_inputVelocity * acceleration;		// 加速度の計算を行う

		// 2乗にすることで符号を外す
		if (m_acceleration.LengthSquared() > 1)
		{
			m_acceleration.Normalize(); // 上限を1に設定する
		}

		moveVelocity += m_acceleration;		// 基本移動に加速度を上乗せする
		m_velocity = moveVelocity;			// 速度を保存する
	}

	/////////////////////////// 移動処理 //////////////////////////////////
	m_position += Vector3::Transform(moveVelocity, Matrix::CreateRotationY(-m_angle));

	/////////////////////////// パーティクルの生成 //////////////////////////
	if (moveVelocity != Vector3::Zero)
	{
		m_particleTime += m_elapsedTime;
		if (m_particleTime >= 0.15f)
		{
			m_playScene->GetParticle()->CreateTrailDust();
			m_particleTime = 0.0f;
		}
	}

	m_inputVelocity = Vector3::Zero;	// 基本速度
}


// --------------------------------
//  ワールド行列の計算
// --------------------------------
void Player::CalculationMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;
	// 行列の計算を行う
	m_worldMatrix = Matrix::Identity;		// 更新ごとに初期化を行う
	m_worldMatrix
		*= Matrix::CreateTranslation(Vector3::Zero)							// 原点に移動
		*= Matrix::CreateScale		(PLAYER_SCALE)							// プレイヤーのサイズ変更
		*= Matrix::CreateRotationY	(-m_angle + XMConvertToRadians(180.f))	// 敵の方向を見るように設定する
		*= Matrix::CreateTranslation(m_position);							// 座標を移動させる
}


// --------------------------------
//  表示処理
// --------------------------------
void Player::Render(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection,
	const CommonResources* resources
	)
{
	using namespace DirectX;

	context->OMSetDepthStencilState(states->DepthDefault(), 0);	// 深度値を参照して書き込む

	// モデルのエフェクト情報を更新する
	m_model->UpdateEffects([](DirectX::IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// ライトを有効化する
				basicEffect->SetLightingEnabled(true);
				// アンビエントライトの設定
				basicEffect->SetAmbientLightColor(Colors::Gray); // 適切なアンビエント色を設定
				// 必要に応じてライトの設定を行う
				basicEffect->SetLightEnabled(0, true);
				basicEffect->SetLightDiffuseColor(0, Colors::White);  // ディフューズ色を設定
				basicEffect->SetLightSpecularColor(0, Colors::White); // スペキュラー色を設定

				basicEffect->SetLightEnabled(1, false); // 追加のライトが不要なら無効化
				basicEffect->SetLightEnabled(2, false); // 追加のライトが不要なら無効化
				// モデルの自発光色を黒に設定して無効化
				basicEffect->SetEmissiveColor(Colors::Black);
			}
		}
	);

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

	// ステートで描画する
	m_currentState->Render(
		context,
		states,
		view,
		projection);

#ifdef _DEBUG
	// 体の境界球の描画
	DrawBoundingSphere(device, context, states, view, projection, m_bodyCollision.get());

	auto debugString = resources->GetDebugString();
	debugString->AddString("push : %f, %f, %f", m_pushBackValue.x, m_pushBackValue.y, m_pushBackValue.z);
	debugString->AddString("PlayerPos : %f, %f, %f", m_position.x, m_position.y, m_position.z);
	
	if (m_canHit)	debugString->AddString("Player_CAN");
	if (!m_isHit)	debugString->AddString("Player_nonHit");

#endif // !_DEBUG

}


// --------------------------------
// 境界球を表示
// --------------------------------
void Player::DrawBoundingSphere(
	ID3D11Device*						device,
	ID3D11DeviceContext*				context,
	DirectX::CommonStates*				states,
	const DirectX::SimpleMath::Matrix&	view,
	const DirectX::SimpleMath::Matrix&	projection,
	const DirectX::BoundingSphere*		boundingSphere)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(device);


	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);
	// 描画
	m_primitiveBatch->Begin();
	DX::Draw(
		m_primitiveBatch.get(),	// プリミティブバッチ
		*boundingSphere,		// 境界球
		Colors::White			// 色
	);
	m_primitiveBatch->End();
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
	//////////////////////敵の体と衝突した時の処理////////////////////////////
	if (data.objType == ObjectType::Enemy && data.colType == CollisionType::Sphere)
	{
		// 衝突したオブジェクトの情報を取得
		DirectX::BoundingSphere playerCollision = *m_bodyCollision.get();
		auto enemy = dynamic_cast<Enemy*>(data.object);
		DirectX::BoundingSphere enemyCollision = enemy->GetBodyCollision();

		// 押し戻し量を計算
		m_pushBackValue += Math::pushBack_BoundingSphere(playerCollision, enemyCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}


	/////////////////////敵が持つ武器と衝突した時の処理////////////////////////
	else if (	! m_isHit							&&
				m_canHit							&&
				data.objType == ObjectType::Cudgel	&&
				data.colType == CollisionType::OBB	&&
				m_currentState != m_playerDodging.get())
	{
		// HPを減らす
		m_hp->Damage(1);
		m_isHit = true;
		m_canHit = false;
		// ノックバックをする
	}


	////////////////////ステージと衝突したときの処理//////////////////////////
	else if (data.objType == ObjectType::Stage && data.colType == CollisionType::Sphere)
	{
		// 衝突したオブジェクトの情報を取得
		auto wall = dynamic_cast<Wall*>(data.object);
		DirectX::BoundingSphere* stageCollision = wall->GetCollision();

		// 押し戻し量を計算
		m_pushBackValue += Math::pushFront_BoundingSphere(*m_bodyCollision.get(), *stageCollision);
		// y座標には反映無しに設定
		m_pushBackValue.y = 0;
		// プレイヤーの位置を押し戻す
		m_position += m_pushBackValue;
		m_bodyCollision->Center = m_position;
	}

}



