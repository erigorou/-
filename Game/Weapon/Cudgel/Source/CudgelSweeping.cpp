// ----------------------------------------------------------
// 名前:	CudgeSweeping
// 
// 内容:	ボス鬼の武器の金棒　薙ぎ払いのステート
// 			ステートマシーンを使用
//
// 制作:	池田桜輔
// ----------------------------------------------------------
// インクルード
#include "pch.h"
#include "Libraries/MyLib/Math.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Sound/Sound.h"
#include "Game/Weapon/WeaponState.h"
#include "Game/Boss/Boss.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Weapon/Cudgel/Header/CudgelSweeping.h"
#include "Game/Weapon/Cudgel/Cudgel.h"

// -----------------------------
// コンストラクタ
// -----------------------------
CudgelSweeping::CudgelSweeping(Cudgel* cudgel)
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
CudgelSweeping::~CudgelSweeping()
{
}

// ----------------------------
// 初期化処理
// ----------------------------
void CudgelSweeping::Initialize()
{
	// ボスの設定
	m_boss = m_cudgel->GetBoss();
}

// -----------------------------
// 事前処理
// -----------------------------
void CudgelSweeping::PreUpdate()
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
void CudgelSweeping::Update(float elapsedTime)
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
void CudgelSweeping::UpdateCudgelRotation()
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
	EventMessenger::Execute(EventList::PlayerCanDamageCudgel, &m_playerCanHit);
}

// --------------------------------
// ためモーション
// --------------------------------
void CudgelSweeping::ChargeAnimation()
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
void CudgelSweeping::AttackAnimation()
{
	// 攻撃モーション
	if (Math::InTime(WINDUP_TIME, m_totalSeconds, ATTACK_TIME))
	{
		// 攻撃可能を通知
		bool attack = true;
		EventMessenger::Execute(EventList::PlayerCanDamageCudgel, &attack);

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
void CudgelSweeping::EndAnimation()
{
	if (m_totalSeconds > END_TIME)
	{
		// 攻撃不可を通知
		bool attack = false;
		EventMessenger::Execute(EventList::PlayerCanDamageCudgel, &attack);

		// ステートを変更
		CudgelState state = CudgelState::Idle;
		EventMessenger::Execute(EventList::ChangeCudgelState, &state);
	}
}

// ----------------------------------
// 描画用ワールド行列を計算する関数
// ----------------------------------
void CudgelSweeping::CalculateModelMatrix()
{
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// サイズの設定 & 初期化
	m_worldMatrix
		*= Matrix::CreateTranslation(CudgelSweeping::ZERO_DIREC)	// 初期位置への移動
		*= CalculateAttackMatrix();									// 攻撃モーション中の計算
}

// ----------------------------------------
// Cudgelのワールド行列を計算する関数
// ----------------------------------------
DirectX::SimpleMath::Matrix CudgelSweeping::CalculateAttackMatrix()
{
	return
		// 縦回転を行う
		Matrix::CreateRotationX(-m_angleUD)
		// 横回転を行う
		*= Matrix::CreateRotationY(DirectX::XMConvertToRadians(-m_angleRL) - m_parentAngleRL - DirectX::XMConvertToRadians(180))
		// 腕の長さ分移動
		*= Matrix::CreateTranslation(CudgelSweeping::ARM_LENGTH)
		// 最後に敵の位置に設定
		*= Matrix::CreateTranslation(m_position);
}

// ----------------------------------------------------
// 金棒の根本と頂点の座標を取得する関数
// ----------------------------------------------------
void CudgelSweeping::GetCudgelBothEnds()
{
	DirectX::SimpleMath::Vector3 root;
	DirectX::SimpleMath::Vector3 tip;

	// モデルの根本の位置を取得
	Matrix rootMat = Matrix::CreateTranslation(CudgelSweeping::ZERO_DIREC);
	rootMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_HADLE_POS)
		*= CalculateAttackMatrix();
	root = Vector3::Transform(Vector3::Zero, rootMat);

	// モデルの先端の位置を取得
	Matrix tipMat = Matrix::CreateTranslation(CudgelSweeping::ZERO_DIREC);
	tipMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_LENGTH)
		*= CalculateAttackMatrix();
	tip = Vector3::Transform(Vector3::Zero, tipMat);

	// 根本座標リストの先端に記録
	m_rootPos.push_back(root);
	// 頂点座標リストの先端に記録
	m_tipPos.push_back(tip);

	// 2個以上ない場合は処理を抜ける
	int max = static_cast<int>(m_rootPos.size() - 1);
	if (max >= 1)
	{
		// 頂点情報の生成（パーティクルの生成に必要）
		DirectX::VertexPositionTexture ver[4] =
		{
			// 左上
			DirectX::VertexPositionTexture(m_tipPos[max] ,DirectX::SimpleMath::Vector2(0, 0)),
			// 右上
			DirectX::VertexPositionTexture(m_tipPos[max - 1]  ,DirectX::SimpleMath::Vector2(1, 0)),
			// 右下
			DirectX::VertexPositionTexture(m_rootPos[max - 1] ,DirectX::SimpleMath::Vector2(1, 1)),
			// 左下
			DirectX::VertexPositionTexture(m_rootPos[max] ,DirectX::SimpleMath::Vector2(0, 1)),
		};
		// パーティクルの生成
		EventMessenger::Execute(EventList::CreateWeaponTrail, &ver);
	}
}

// ------------------------
// 状態更新処理
// ------------------------
void CudgelSweeping::PostUpdate()
{
}

// ------------------------
// 終了処理
// ------------------------
void CudgelSweeping::Finalize()
{
}

// ------------------------
// 衝突判定
// ------------------------
void CudgelSweeping::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}