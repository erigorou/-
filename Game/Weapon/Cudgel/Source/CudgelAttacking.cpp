// ----------------------------------------------------------
// 名前:	CudgelAttacking
// 
// 内容:	ボス鬼の武器の金棒　たたきつけ攻撃のステート
// 			ステートマシーンを使用
//
// 制作:	池田桜輔
// ----------------------------------------------------------
// インクルード
#include "pch.h"
#include "Game/Messenger/EventMessenger.h"
#include "Game/Boss/Boss.h"
#include "Game/Weapon/Cudgel/Header/CudgelAttacking.h"
#include "Game/Weapon/Cudgel/Cudgel.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Sound/Sound.h"

// -----------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="cudgel">ステートの所有者</param>
// -----------------------------------------------
CudgelAttacking::CudgelAttacking(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position{},
	m_velocity{},
	m_angleRL{},
	m_angleUD{},
	m_totalSeconds{},
	m_recordPointTimer{},
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_canGenerateSlamParticles(true)
{
}

// -----------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// -----------------------------------------------
CudgelAttacking::~CudgelAttacking()
{
}

// -----------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
// -----------------------------------------------
void CudgelAttacking::Initialize()
{
	// ワールド行列の初期化
	m_worldMatrix = Matrix::Identity;
}

// -----------------------------------------------
/// <summary>
/// ステート変更処理(in)
/// </summary>
// -----------------------------------------------
void CudgelAttacking::PreUpdate()
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

// -----------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
// -----------------------------------------------
void CudgelAttacking::Update(float elapsedTime)
{
	// 合計時間を計測
	m_totalSeconds += elapsedTime;
	m_recordPointTimer += elapsedTime;

	// ボスのポインタを取得
	auto boss = m_cudgel->GetBoss();
	// 敵の座標を取得
	m_position = boss->GetPosition();
	// 敵の回転を取得
	m_enemyAngle = boss->GetAngle();
	// 敵の回転を設定
	m_angleRL = m_enemyAngle;
	// アニメーションの更新
	UpdateAnimation();
}

// -----------------------------------------------
/// <summary>
/// アニメーションの更新処理
/// </summary>
// -----------------------------------------------
void CudgelAttacking::UpdateAnimation()
{
	// 金棒の回転を更新
	UpdateCudgelRotation();
	// 金棒のワールド行列を計算
	CalculateModelMatrix();
	// 金棒の根本と先端の座標を取得
	GetCudgelBothEnds(m_totalSeconds);

	// 計算したワールド行列を設定
	m_cudgel->SetWorldMatrix(m_worldMatrix);
	m_cudgel->SetCollisionPosition(m_worldMatrix);
}

// -----------------------------------------------
/// <summary>
/// Cudgelの縦軸の回転の更新関数
/// </summary>
// -----------------------------------------------
void CudgelAttacking::UpdateCudgelRotation()
{
	UpdateAttackState();	// 攻撃状態の更新
}

// -----------------------------------------------
/// <summary>
/// 振りかざしの処理。武器を20度持ち上げる。
/// </summary>
/// <param name="t">正規化した時間</param>
// -----------------------------------------------
void CudgelAttacking::HandleChargePhase(float t)
{
	// 20度上げる（0.5秒間で、イージング使用）
	m_angleUD = DirectX::XMConvertToRadians(-CHARGE_ANGLE * Easing::easeInOutBack(t));
}

// -----------------------------------------------
/// <summary>
/// 待機状態の処理。武器を20度の角度で保持する。
/// </summary>
// -----------------------------------------------
void CudgelAttacking::HandleWindoupPhase()
{
	// 振りかざしの角度を保持（-40度の状態を維持）
	m_angleUD = DirectX::XMConvertToRadians(-CHARGE_ANGLE);
}

// -----------------------------------------------
/// <summary>
/// 攻撃フェーズの処理。武器を20度から95度まで振り下ろす。
/// </summary>
/// <param name="t">0から1に正規化された時間。</param>
// -----------------------------------------------
void CudgelAttacking::HandleAttackPhase(float t)
{
	m_canHit = true;

	// 振り下ろす角度を計算する
	m_angleUD = DirectX::XMConvertToRadians(-CHARGE_ANGLE + END_ANGLE * Easing::easeInQuint(t));
	// 横回転を計算する
	m_angleRL = DirectX::XMConvertToRadians(-INIT_ANGLE * Easing::easeInQuint(t)) + m_angleRL;

	// SEを再生する
	if (t > SOUND_TIME && m_playSound == false)
	{
		Sound::PlaySE(Sound::SE_TYPE::BOSS_ATTACK);
		m_playSound = true;
	}
}

// -----------------------------------------------
/// <summary>
/// たたきつけてから待機するモーション
/// </summary>
// -----------------------------------------------
void CudgelAttacking::KeepStampPhase()
{
	m_canHit = false;

	m_angleUD = DirectX::XMConvertToRadians(ATTACK_ANGLE);
	m_angleRL = DirectX::XMConvertToRadians(-INIT_ANGLE) + m_angleRL;

	// たたきつけた瞬間にパーティクルの生成
	HandleSlamParticles();
}

// -----------------------------------------------
/// <summary>
/// 待機状態に戻る処理。
/// </summary>
/// <param name="t">正規化した時間</param>
// -----------------------------------------------
void CudgelAttacking::ReturnToOriginalPhase(float t)
{
	// 横と縦の回転を元に戻す
	m_angleUD = DirectX::XMConvertToRadians(ATTACK_ANGLE - ATTACK_ANGLE * t);
	m_angleRL = DirectX::XMConvertToRadians(-INIT_ANGLE + INIT_ANGLE * t) + m_angleRL;
}

// -----------------------------------------------
/// <summary>
/// 後隙のパーティクル生成処理
/// 武器の先端に塵を発生させ、カメラを振動させる。
/// </summary>
// -----------------------------------------------
void CudgelAttacking::HandleSlamParticles()
{
	if (m_canGenerateSlamParticles)
	{
		m_canGenerateSlamParticles = false;

		// パーティクルの生成
		EventMessenger::Execute(EventList::CreateBashDust, (&m_tipPos[m_tipPos.size() - 1]));

		// カメラの振動を設定
		float m_shakePower = 1.5f;
		EventMessenger::Execute(EventList::ShakeCamera, &m_shakePower);
	}
}

// -----------------------------------------------
/// <summary>
/// 更新する処理
/// </summary>
// -----------------------------------------------
void CudgelAttacking::UpdateAttackState()
{
	// たたきつけた後の待機モーション
	if (m_totalSeconds < CHARGE_TIME) HandleChargePhase(m_totalSeconds / CHARGE_TIME);
	// 攻撃前のモーション
	else if (Math::InTime(CHARGE_ANGLE, m_totalSeconds, WINDUP_TIME)) HandleWindoupPhase();
	// 攻撃モーション
	else if (Math::InTime(WINDUP_TIME, m_totalSeconds, ATTACK_TIME)) HandleAttackPhase((m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME));
	// たたきつけた後の待機モーション
	else if (Math::InTime(ATTACK_ANGLE, m_totalSeconds, STOP_TIME))	KeepStampPhase();
	// 戻りアニメーション
	else if (Math::InTime(STOP_TIME, m_totalSeconds, RETURN_TIME)) ReturnToOriginalPhase((m_totalSeconds - STOP_TIME) / (RETURN_TIME - STOP_TIME));

	// プレイヤーに攻撃可能かを通知
	EventMessenger::Execute(EventList::PlayerCanDamageCudgel, &m_canHit);
}

// -----------------------------------------------
/// <summary>
/// 描画用ワールド行列を計算する関数
/// </summary>
// -----------------------------------------------
void CudgelAttacking::CalculateModelMatrix()
{
	// サイズの設定 & 初期化
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);
	m_worldMatrix
		// 初期位置への移動
		*= Matrix::CreateTranslation(CudgelAttacking::ZERO_DIREC)
		// 攻撃モーション中の計算
		*= CalculateAttackMatrix();
}

// -----------------------------------------------
/// <summary>
/// Cudgelのワールド行列を計算する関数
/// </summary>
/// <returns>攻撃モーション中のCudgelの回転ワールド行列</returns>
// -----------------------------------------------
DirectX::SimpleMath::Matrix CudgelAttacking::CalculateAttackMatrix()
{
	return
		// 縦回転を行う
		Matrix::CreateRotationX(-m_angleUD)
		// 横回転を行う
		*= Matrix::CreateRotationY(-m_angleRL)
		// 腕の長さ分移動
		*= Matrix::CreateTranslation(CudgelAttacking::ARM_LENGTH)
		// 最後に敵の位置に設定
		*= Matrix::CreateTranslation(m_position);
}

// -----------------------------------------------
/// <summary>
/// エフェクトなどに使用する金棒の根本と頂点の座標を取得する関数
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
// -----------------------------------------------
void CudgelAttacking::GetCudgelBothEnds(float _totalTime)
{
	UNREFERENCED_PARAMETER(_totalTime);

	DirectX::SimpleMath::Vector3 root;
	DirectX::SimpleMath::Vector3 tip;

	// モデルの根本の位置を取得
	Matrix rootMat = Matrix::CreateTranslation(CudgelAttacking::ZERO_DIREC);
	rootMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_HADLE_POS)
		*= CalculateAttackMatrix();
	root = Vector3::Transform(Vector3::Zero, rootMat);

	// モデルの先端の位置を取得
	Matrix tipMat = Matrix::CreateTranslation(CudgelAttacking::ZERO_DIREC);
	tipMat
		*= Matrix::CreateTranslation(Cudgel::CUDGEL_LENGTH)
		*= CalculateAttackMatrix();
	tip = Vector3::Transform(Vector3::Zero, tipMat);

	m_recordPointTimer = 0.0f;
	// 根本座標リストの先端に記録
	m_rootPos.push_back(root);
	// 頂点座標リストの先端に記録
	m_tipPos.push_back(tip);

	using namespace DirectX;

	// 2個以上ない場合は処理を抜ける
	int max = static_cast<int>(m_rootPos.size()) - 1;
	if (max >= 1)
	{
		// 頂点情報の生成（パーティクルの生成に必要）
		VertexPositionTexture ver[4] =
		{
			// 左上
			VertexPositionTexture(m_tipPos[max] ,Vector2(0, 0)),
			// 右上
			VertexPositionTexture(m_tipPos[max - 1]	,Vector2(1, 0)),
			// 右下
			VertexPositionTexture(m_rootPos[max - 1] ,Vector2(1, 1)),
			// 左下
			VertexPositionTexture(m_rootPos[max] ,Vector2(0, 1)),
		};
		// パーティクルの生成
		EventMessenger::Execute(EventList::CreateWeaponTrail, &ver);
	}
}

// -----------------------------------------------
/// <summary>
/// 事後処理
/// </summary>
// -----------------------------------------------
void CudgelAttacking::PostUpdate()
{
}

// -----------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
// -----------------------------------------------
void CudgelAttacking::Finalize()
{
}

// -----------------------------------------------
/// <summary>
/// 衝突イベント
/// </summary>
/// <param name="data">相手のデータ</param>
// -----------------------------------------------
void CudgelAttacking::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);
}