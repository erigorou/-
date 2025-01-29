/// ---------------------------
///
/// 鬼の金棒の攻撃状態
/// 
/// ---------------------------

#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"
#include "Libraries/MyLib/Collision.h"
#include "Game/Messenger/EventMessenger.h"

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Attacking.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Sound/Sound.h"


// 固定値 ==================================
const float Cudgel_Attacking::CHARGE_TIME = 0.8f;	// 振りかざし時間
const float Cudgel_Attacking::WINDUP_TIME = 0.9f;	// 待機
const float Cudgel_Attacking::ATTACK_TIME = 1.5f;	// 降り下ろし
const float Cudgel_Attacking::STOP_TIME	  = 2.3f;	// 待機
const float Cudgel_Attacking::RETURN_TIME = 2.8f;	// 元に戻る
const float Cudgel_Attacking::END_TIME    = 3.3f;	// 終了時間

const Vector3 Cudgel_Attacking::ARM_LENGTH = Vector3(0.0f, 4.0f, 0.0f);
const Vector3 Cudgel_Attacking::ZERO_DIREC = Vector3(8.0f, 1.0f, 0.0f);


// コンストラクタ
Cudgel_Attacking::Cudgel_Attacking(Cudgel* cudgel)
	:m_cudgel(cudgel)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angleRL(0.0f)
	, m_angleUD(0.0f)
	, m_totalSeconds(0.0f)
	, m_recordPointTimer(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
	, m_canGenerateSlamParticles(true)
{
	// パーティクルの取得
	m_particles = cudgel->GetPlayScene()->GetParticle();
}

// デストラクタ
Cudgel_Attacking::~Cudgel_Attacking()
{
}


// 初期化処理
void Cudgel_Attacking::Initialize()
{
	m_worldMatrix = Matrix::Identity;			// ワールド行列の初期化
}


// 事前処理
void Cudgel_Attacking::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	m_angleUD = 0.0f;
	m_canHit = false;
	m_recordPointTimer = 0.0f;

	// 頂点情報の初期化
	m_rootPos.clear();
	m_tipPos.clear();

	// パーティクルを生成可能に
	m_canGenerateSlamParticles = true;
	// 音声サウンドを可能に
	m_playSound = false;
}



// 更新処理
void Cudgel_Attacking::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	// 合計時間を計測
	m_totalSeconds		+= elapsedTime;
	m_recordPointTimer	+= elapsedTime;

	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	m_position = enemy->GetPosition();	// 敵の座標を取得
	m_enemyAngle = enemy->GetAngle();	// 敵の回転を取得
	m_angleRL = m_enemyAngle;			// 敵の回転を設定

	UpdateCudgelRotation();				// 回転を計算する
	CalculateModelMatrix();				// ワールド行列を計算
	GetCudgelBothEnds(m_totalSeconds);	// 両端を取得する

	m_cudgel->SetWorldMatrix(m_worldMatrix);		// ワールド行列を設定する
	m_cudgel->SetCollisionPosition(m_worldMatrix);	// 当たり判定の位置を設定する
}


/// <summary>
/// Cudgelの縦軸の回転の更新関数
/// </summary>
void Cudgel_Attacking::UpdateCudgelRotation()
{
	UpdateAttackState();	// 攻撃状態の更新
}


/// <summary>
/// 振りかざしの処理。武器を20度持ち上げる。
/// </summary>
/// <param name="t">0から1に正規化された時間。</param>
void Cudgel_Attacking::HandleChargePhase(float t)
{
	// 20度上げる（0.5秒間で、イージング使用）
	m_angleUD = DirectX::XMConvertToRadians(-40.0f * Easing::easeInOutBack(t));
}


/// <summary>
/// 待機状態の処理。武器を20度の角度で保持する。
/// </summary>
void Cudgel_Attacking::HandleWindoupPhase()
{
	m_canHit = true;

	// 振りかざしの角度を保持（-40度の状態を維持）
	m_angleUD = DirectX::XMConvertToRadians(-40.0f);
}


/// <summary>
/// 攻撃フェーズの処理。武器を20度から95度まで振り下ろす。
/// </summary>
/// <param name="t">0から1に正規化された時間。</param>
void Cudgel_Attacking::HandleAttackPhase(float t)
{
	// 20度から115度振り下ろす（0.3秒間で、イージング使用）
	m_angleUD = DirectX::XMConvertToRadians(-40.0f + 135.0f * Easing::easeInQuint(t));
	m_angleRL = DirectX::XMConvertToRadians(-ATTACK_ANGLE_UD * Easing::easeInQuint(t)) + m_angleRL;

	if (t > 0.9f && m_playSound == false)
	{
		Sound::PlaySE(Sound::SE_TYPE::ENEMY_ATTACK);
		m_playSound = true;
	}
}

/// <summary>
/// たたきつけてから待機するモーション
/// </summary>
void Cudgel_Attacking::KeepStampPhase()
{
	m_canHit = false;

	m_angleUD = DirectX::XMConvertToRadians(95.0f);
	m_angleRL = DirectX::XMConvertToRadians(-ATTACK_ANGLE_UD) + m_angleRL;

	// たたきつけた瞬間にパーティクルの生成
	HandleSlamParticles();
}


/// <summary>
/// 待機状態に戻る処理。
/// </summary>
/// <param name="t"></param>
void Cudgel_Attacking::ReturnToOriginalPhase(float t)
{

	// 横と縦の回転を元に戻す
	m_angleUD = DirectX::XMConvertToRadians(95.0f - 95.0f * t);
	m_angleRL = DirectX::XMConvertToRadians(- ATTACK_ANGLE_UD + ATTACK_ANGLE_UD * t) + m_angleRL;
}

/// <summary>
/// 後隙のパーティクル生成処理。武器の先端に塵を発生させ、カメラを振動させる。
/// </summary>
void Cudgel_Attacking::HandleSlamParticles()
{
	if (m_canGenerateSlamParticles)
	{
		m_canGenerateSlamParticles = false;
		m_cudgel->GetPlayScene()->GetParticle()->CreateSlamDust(&(m_tipPos[m_tipPos.size() - 1]));

		float m_shakePower = 1.5f;

		// カメラの振動を設定
		EventMessenger::Execute("CameraShake", &m_shakePower);
	}
}



/// <summary>
/// 更新する処理
/// </summary>
void Cudgel_Attacking::UpdateAttackState()
{
	if		(m_totalSeconds < CHARGE_TIME)									HandleChargePhase(m_totalSeconds / CHARGE_TIME);
	else if (m_totalSeconds > CHARGE_TIME && m_totalSeconds <= WINDUP_TIME)	HandleWindoupPhase();
	else if (m_totalSeconds > WINDUP_TIME && m_totalSeconds <= ATTACK_TIME)	HandleAttackPhase((m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME));
	else if (m_totalSeconds > ATTACK_TIME && m_totalSeconds <= STOP_TIME)	KeepStampPhase();
	else if (m_totalSeconds > STOP_TIME && m_totalSeconds <= RETURN_TIME)	ReturnToOriginalPhase((m_totalSeconds - STOP_TIME) / (RETURN_TIME - STOP_TIME));

	// プレイヤーに攻撃可能状態を通知
	m_cudgel->GetPlayScene()->GetPlayer()->CanHit(m_canHit);
}




/// <summary>
/// 描画用ワールド行列を計算する関数
/// </summary>
void Cudgel_Attacking::CalculateModelMatrix()
{
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// サイズの設定 & 初期化
	m_worldMatrix
		*= Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC)	// 初期位置への移動
		*= CalculateAttackMatrix();									// 攻撃モーション中の計算
}


/// <summary>
/// Cudgelのワールド行列を計算する関数
/// </summary>
/// <returns>攻撃モーション中のCudgelの回転ワールド行列</returns>
DirectX::SimpleMath::Matrix Cudgel_Attacking::CalculateAttackMatrix()
{
	return 
			Matrix::CreateRotationX(-m_angleUD)							// 縦回転を行う
		*=  Matrix::CreateRotationY(-m_angleRL)							// 横回転を行う
		*=  Matrix::CreateTranslation(Cudgel_Attacking::ARM_LENGTH)		// 腕の長さ分移動
		*=  Matrix::CreateTranslation(m_position);						// 最後に敵の位置に設定
}


/// <summary>
/// エフェクトなどに使用する金棒の根本と頂点の座標を取得する関数
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
void Cudgel_Attacking::GetCudgelBothEnds(float _totalTime)
{
	UNREFERENCED_PARAMETER(_totalTime);

	DirectX::SimpleMath::Vector3 root;
	DirectX::SimpleMath::Vector3 tip;

	Matrix rootMat = Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC);
	rootMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_HADLE_POS)
		*= CalculateAttackMatrix();
	root = Vector3::Transform(Vector3::Zero, rootMat);		// モデルの先端の位置を取得

	
	Matrix tipMat = Matrix::CreateTranslation(Cudgel_Attacking::ZERO_DIREC);
	tipMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_LENGTH)
		*= CalculateAttackMatrix();
	tip = Vector3::Transform(Vector3::Zero, tipMat);		// モデルの先端の位置を取得

	m_recordPointTimer = 0.0f;
	m_rootPos.push_back(root);		// 根本座標リストの先端に記録
	m_tipPos .push_back(tip);		// 頂点座標リストの先端に記録

	using namespace DirectX;

	// 2個以上ない場合は処理を抜ける
	size_t max = m_rootPos.size() - 1;
	if (max >= 1)
	{
		VertexPositionTexture ver[4] =	// 頂点情報の生成（パーティクルの生成に必要）
		{
			VertexPositionTexture(m_tipPos [max]		,Vector2(0, 0)),	// 左上
			VertexPositionTexture(m_tipPos [max - 1]	,Vector2(1, 0)),	// 右上
			VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// 右下
			VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// 左下
		};

		m_particles->CreateSwordTrail(ver);	// パーティクルの生成
	}
}


/// <summary>
/// 事後処理
/// </summary>
void Cudgel_Attacking::PostUpdate()
{
}


// 終了処理
void Cudgel_Attacking::Finalize()
{
}


void Cudgel_Attacking::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
