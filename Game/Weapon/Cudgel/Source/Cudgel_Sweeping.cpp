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

#include "Game/Player/Player.h"
#include "Game/Enemy/Enemy.h"
#include "Libraries/MyLib/EasingFunctions.h"
#include "Game/Weapon/Cudgel/Header/Cudgel_Sweeping.h"
#include "Game/Weapon/Cudgel/Header/Cudgel.h"


// 固定値 ==================================
const float Cudgel_Sweeping::CHARGE_TIME = 1.0f;	// 振りかざし時間
const float Cudgel_Sweeping::WINDUP_TIME = 1.3f;	// 待機
const float Cudgel_Sweeping::ATTACK_TIME = 1.8f;	// 薙ぎ払い
const float Cudgel_Sweeping::END_TIME	 = 2.5f;	// 終了

const float Cudgel_Sweeping::CHARGE_ROTATE_ANGLE = 30.0f;	// 振りかざし角度
const float Cudgel_Sweeping::WINDUP_ROTATE_ANGLE = 240.0f;	// 薙ぎ払い角度

const Vector3 Cudgel_Sweeping::ARM_LENGTH = Vector3(0.0f, 5.0f, 0.0f);
const Vector3 Cudgel_Sweeping::ZERO_DIREC = Vector3(6.0f, 1.0f, 0.0f);


// コンストラクタ
Cudgel_Sweeping::Cudgel_Sweeping(Cudgel* cudgel)
	:m_cudgel(cudgel)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_velocity(0.0f, 0.0f, 0.0f)
	, m_angleRL(0.0f)
	, m_angleUD(0.0f)
	, m_parentAngleRL(0.0f)
	, m_totalSeconds(0.0f)
	, m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
	, m_model(nullptr)
	, m_canGenerateSlamParticles(true)
{
	// パーティクルの取得
	m_particles = cudgel->GetPlayScene()->GetParticle();
}

// デストラクタ
Cudgel_Sweeping::~Cudgel_Sweeping()
{
}


// 初期化処理
void Cudgel_Sweeping::Initialize()
{
	m_worldMatrix = Matrix::Identity;			// ワールド行列の初期化
	m_model = m_cudgel->GetModel();
	m_originalBox = Collision::Get_BoundingOrientedBox_FromMODEL(m_model);	// モデルから大きさを取得
}


// 事前処理
void Cudgel_Sweeping::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.0f;												// 経過時間の初期化
	m_angleUD = DirectX::XMConvertToRadians(90.0f);						// 金棒を横にする
	m_angleRL = 0.0f;													// 横回転の初期化
	m_parentAngleRL = m_cudgel->GetPlayScene()->GetEnemy()->GetAngle();	// 敵の角度を取得（親の角度）

	// 頂点情報の初期化
	m_rootPos.clear();
	m_tipPos.clear();

	m_canGenerateSlamParticles = true;
}



// 更新処理
void Cudgel_Sweeping::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	// 合計時間を計測
	m_totalSeconds		+= elapsedTime;

	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	m_position = enemy->GetPosition();	// 敵の座標を取得

	UpdateCudgelRotation();				// 回転を計算する
	CalculateModelMatrix();				// ワールド行列を計算
	GetCudgelBothEnds(m_totalSeconds);	// 両端を取得する

	m_originalBox.Transform(m_boundingBox, m_worldMatrix);	// 当たり判定を移動させる　※
}


/// <summary>
/// Cudgelの縦軸の回転の更新関数
/// </summary>
void Cudgel_Sweeping::UpdateCudgelRotation()
{
	float t = 0.0f;  // 正規化された経過時間

	// ---- 横方向の回転（左右の回転）----
	if (m_totalSeconds <= CHARGE_TIME) {
				t = m_totalSeconds / CHARGE_TIME;  // 0 ~ 1 に正規化
		m_angleRL = -CHARGE_ROTATE_ANGLE * m_easying->easeOutCirc(t);  // 30度左回転
	}

	else if (m_totalSeconds >= WINDUP_TIME && m_totalSeconds <= ATTACK_TIME) {
		t = (m_totalSeconds - WINDUP_TIME) / (ATTACK_TIME - WINDUP_TIME);  // 0 ~ 1 に正規化
		m_angleRL = CHARGE_ROTATE_ANGLE + WINDUP_ROTATE_ANGLE * m_easying->easeOutBack(t);  // 30度から60度右回転
	}
}


/// <summary>
/// 描画用ワールド行列を計算する関数
/// </summary>
void Cudgel_Sweeping::CalculateModelMatrix()
{
	m_worldMatrix = Matrix::CreateScale(Cudgel::CUDGEL_SCALE);		// サイズの設定 & 初期化
	m_worldMatrix
		*= Matrix::CreateTranslation(Cudgel_Sweeping::ZERO_DIREC)	// 初期位置への移動
		*= CalculateAttackMatrix();									// 攻撃モーション中の計算
}


/// <summary>
/// Cudgelのワールド行列を計算する関数
/// </summary>
/// <returns>攻撃モーション中のCudgelの回転ワールド行列</returns>
DirectX::SimpleMath::Matrix Cudgel_Sweeping::CalculateAttackMatrix()
{
	return 
			Matrix::CreateRotationX(-m_angleUD)																						// 縦回転を行う
		*=  Matrix::CreateRotationY(DirectX::XMConvertToRadians(-m_angleRL) - m_parentAngleRL - DirectX::XMConvertToRadians(189))	// 横回転を行う
		*=  Matrix::CreateTranslation(Cudgel_Sweeping::ARM_LENGTH)																	// 腕の長さ分移動
		*=  Matrix::CreateTranslation(m_position);																					// 最後に敵の位置に設定
}


/// <summary>
/// エフェクトなどに使用する金棒の根本と頂点の座標を取得する関数
/// </summary>
/// <param name="_elapsedTime">経過時間</param>
void Cudgel_Sweeping::GetCudgelBothEnds(float _totalTime)
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

		m_rootPos.push_back(root);		// 根本座標リストの先端に記録
		m_tipPos .push_back(tip);		// 頂点座標リストの先端に記録

	using namespace DirectX;

	// 2個以上ない場合は処理を抜ける
	float max = m_rootPos.size() - 1;
	if (max >= 1)
	{
		VertexPositionTexture ver[4] =	// 頂点情報の生成（パーティクルの生成に必要）
		{
			VertexPositionTexture(m_tipPos [max]		,Vector2(0, 0)),	// 左上
			VertexPositionTexture(m_tipPos [max - 1]	,Vector2(1, 0)),	// 右上
			VertexPositionTexture(m_rootPos[max - 1]	,Vector2(1, 1)),	// 右下
			VertexPositionTexture(m_rootPos[max]		,Vector2(0, 1)),	// 左下
		};

		m_particles->CreateSwordTrial(ver);	// パーティクルの生成
	}
}


/// <summary>
/// 事後処理
/// </summary>
void Cudgel_Sweeping::PostUpdate()
{
}


/// <summary>
/// 描画処理
/// </summary>
/// <param name="context"></param>
/// <param name="states"></param>
/// <param name="view"></param>
/// <param name="projection"></param>
void Cudgel_Sweeping::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);

#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("%f", m_angleRL);

	// Sphereの生成
	//auto sphere = DirectX::GeometricPrimitive::CreateSphere(context, 1.0f);

	// m_rootPosの各頂点に球体を描画
	//for (const auto& rootPos : m_rootPos)
	//{
	//	DirectX::SimpleMath::Matrix rootSphereMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(rootPos);
	//	sphere->Draw(rootSphereMatrix, view, projection, DirectX::Colors::Red, nullptr, true);
	//}

	// m_tipPosの各頂点に球体を描画
	//for (const auto& tipPos : m_tipPos)
	//{
	//	DirectX::SimpleMath::Matrix tipSphereMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(tipPos);
	//	sphere->Draw(tipSphereMatrix, view, projection, DirectX::Colors::Blue, nullptr, true);
	//}
#endif // _DEBUG
}


// 終了処理
void Cudgel_Sweeping::Finalize()
{
}