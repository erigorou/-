/// ---------------------------
///
/// 鬼の金棒の攻撃状態(薙ぎ払い)
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
#include "Game/Sound/Sound.h"
#include "Game/Weapon/WeaponState.h"

#include "Game/Boss/Boss.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Sweeping.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// -----------------------------
// コンストラクタ
// -----------------------------
Cudgel_Sweeping::Cudgel_Sweeping(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position{},
	m_velocity{},
	m_angleRL{},
	m_angleUD{},
	m_parentAngleRL{},
	m_totalSeconds{},
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_playSound(false),
	m_playerCanHit(false)
{
}


// ----------------------------
// デストラクタ
// ----------------------------
Cudgel_Sweeping::~Cudgel_Sweeping()
{
}


// ----------------------------
// 初期化処理
// ----------------------------
void Cudgel_Sweeping::Initialize()
{
	// ボスの設定
	m_boss = m_cudgel->GetBoss();
}


// -----------------------------
// 事前処理
// -----------------------------
void Cudgel_Sweeping::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;
	// 金棒を横にする
	m_angleUD = DirectX::XMConvertToRadians(INITIAL_ROTATE_UD);
	// 横回転の初期化
	m_angleRL = 0.0f;
	// 親の角度を取得
	m_parentAngleRL = m_boss->GetAngle();
	// エフェクト用の頂点情報を初期化
	m_rootPos.clear();
	// エフェクト用の頂点情報を初期化
	m_tipPos.clear();
	// サウンドの再生フラグをfalseにする
	m_playSound = false;
}


// --------------------------------
// 更新処理
// --------------------------------
void Cudgel_Sweeping::Update(float elapsedTime)
{
	// 合計時間を計測
	m_totalSeconds += elapsedTime;

	// アニメーションを更新する
	UpdateCudgelRotation();

	// ワールド行列を計算
	CalculateModelMatrix();
	// 両端を取得する
	GetCudgelBothEnds();


	// ワールド行列を設定する
	m_cudgel->SetWorldMatrix(m_worldMatrix);
	// 当たり判定用の行列を取得
	m_collMatrix = m_worldMatrix;
	// 当たり判定の位置を調整する
	m_collMatrix._42 = 0.0f;
	m_cudgel->SetCollisionPosition(m_collMatrix);
}


// ------------------------------
// Cudgelの縦軸の回転の更新関数
// ------------------------------
void Cudgel_Sweeping::UpdateCudgelRotation()
{
	// 敵の座標を取得
	m_position = m_boss->GetPosition();
	// 毎度頭でフラグをfalseにする
	m_playerCanHit = false;

	// 貯めモーション
	ChargeAnimation();
	// 攻撃モーション
	AttackAnimation();
	// 終了モーション
	EndAnimation();
	// プレイヤーに攻撃可能かを通知
	EventMessenger::Execute(EventList::PlayerCanHit, &m_playerCanHit);
}



// --------------------------------
// ためモーション
// --------------------------------
void Cudgel_Sweeping::ChargeAnimation()
{
	// 攻撃前の貯めるアニメーションを行う
	if (m_totalSeconds <= CHARGE_TIME)
	{
		// 秒数を正規化
		float easing = m_totalSeconds / CHARGE_TIME;
		// イージングを掛けた回転角を計算
		m_angleRL = -CHARGE_ROTATE_ANGLE * Easing::easeOutCirc(easing);
	}
}


// --------------------------------
// 攻撃モーション
// --------------------------------
void Cudgel_Sweeping::AttackAnimation()
{
	// 攻撃モーション
	if (Math::InTime(WINDUP_TIME, m_totalSeconds, ATTACK_TIME))
	{
		// 秒数の正規化
		float easing = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);
		// イージングを掛けた回転を計算
		m_angleRL = CHARGE_ROTATE_ANGLE + WINDUP_ROTATE_ANGLE * Easing::easeOutBack(easing);
		// プレイヤーに攻撃可能かを通知
		m_playerCanHit = true;
		// SEの再生
		if (m_playSound == false)
		{
			// 1度だけ再生する
			Sound::PlaySE(Sound::SE_TYPE::BOSS_SWEEP);
			m_playSound = true;
		}
	}
}

// ---------------------------------
// アニメーション終了
// ---------------------------------
void Cudgel_Sweeping::EndAnimation()
{
	if (m_totalSeconds > END_TIME)
	{
		// ステートを変更
		CudgelState state = CudgelState::Idle;
		EventMessenger::Execute(EventList::ChangeCudgelState, &state);
	}
}


// ----------------------------------
// 描画用ワールド行列を計算する関数
// ----------------------------------
void Cudgel_Sweeping::CalculateModelMatrix()
{
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// サイズの設定 & 初期化
	m_worldMatrix
		*= Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC)	// 初期位置への移動
		*= CalculateAttackMatrix();									// 攻撃モーション中の計算
}


// ----------------------------------------
// Cudgelのワールド行列を計算する関数
// ----------------------------------------
DirectX::SimpleMath::Matrix Cudgel_Sweeping::CalculateAttackMatrix()
{
	return 
			Matrix::CreateRotationX(-m_angleUD)																						// 縦回転を行う
		*=  Matrix::CreateRotationY(DirectX::XMConvertToRadians(-m_angleRL) - m_parentAngleRL - DirectX::XMConvertToRadians(180))	// 横回転を行う
		*=  Matrix::CreateTranslation(Cudgel_Sweeping::ARM_LENGTH)																	// 腕の長さ分移動
		*=  Matrix::CreateTranslation(m_position);																					// 最後に敵の位置に設定
}


// ----------------------------------------------------
// 金棒の根本と頂点の座標を取得する関数
// ----------------------------------------------------
void Cudgel_Sweeping::GetCudgelBothEnds()
{
	DirectX::SimpleMath::Vector3 root;
	DirectX::SimpleMath::Vector3 tip;

	Matrix rootMat = Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC);
	rootMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_HADLE_POS)
		*= CalculateAttackMatrix();
	root = Vector3::Transform(Vector3::Zero, rootMat);		// モデルの先端の位置を取得

	
	Matrix tipMat = Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC);
	tipMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_LENGTH)
		*= CalculateAttackMatrix();
	tip = Vector3::Transform(Vector3::Zero, tipMat);		// モデルの先端の位置を取得

		m_rootPos.push_back(root);							// 根本座標リストの先端に記録
		m_tipPos .push_back(tip);							// 頂点座標リストの先端に記録


	// 2個以上ない場合は処理を抜ける
	int max = static_cast<int>(m_rootPos.size() - 1);
	if (max >= 1)
	{
		DirectX::VertexPositionTexture ver[4] =	// 頂点情報の生成（パーティクルの生成に必要）
		{
			DirectX::VertexPositionTexture(m_tipPos [max]		,DirectX::SimpleMath::Vector2(0, 0)),	// 左上
			DirectX::VertexPositionTexture(m_tipPos [max - 1]	,DirectX::SimpleMath::Vector2(1, 0)),	// 右上
			DirectX::VertexPositionTexture(m_rootPos[max - 1]	,DirectX::SimpleMath::Vector2(1, 1)),	// 右下
			DirectX::VertexPositionTexture(m_rootPos[max]		,DirectX::SimpleMath::Vector2(0, 1)),	// 左下
		};

		EventMessenger::Execute(EventList::CreateWeaponTrail, &ver);	// パーティクルの生成
	}
}


// ------------------------
// 状態更新処理
// ------------------------
void Cudgel_Sweeping::PostUpdate()
{
}


// ------------------------
// 終了処理
// ------------------------
void Cudgel_Sweeping::Finalize()
{
}


// ------------------------
// 衝突判定
// ------------------------
void Cudgel_Sweeping::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}
