/// ---------------------------
/// 
///
/// 鬼の金棒の待機状態
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
#include "Game/Weapon/Cudgel/Header/Cudgel_Idling.h"
#include "Game/Weapon/Cudgel/Cudgel.h"


// -----------------------------------------------
// コンストラクタ
// -----------------------------------------------
Cudgel_Idling::Cudgel_Idling(Cudgel* cudgel)
	:
	m_cudgel(cudgel),
	m_position(0.0f, 0.0f, 0.0f),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_angle(0.0f),
	m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
	m_model(nullptr)
{
}


// -----------------------------------------------
// デストラクタ
// -----------------------------------------------
Cudgel_Idling::~Cudgel_Idling()
{
}


// -----------------------------------------------
// 初期化処理
// -----------------------------------------------
void Cudgel_Idling::Initialize()
{
	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;		// ワールド行列の初期化
	m_model = m_cudgel->GetModel();								// モデルを取得
}


// -----------------------------------------------
// 事前処理
// -----------------------------------------------
void Cudgel_Idling::PreUpdate()
{
}


// -----------------------------------------------
// 更新処理
// -----------------------------------------------
void Cudgel_Idling::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	if (!m_cudgel->GetPlayScene()->GetEnemy()) return;

	auto enemy = m_cudgel->GetPlayScene()->GetEnemy();
	m_position = enemy->GetPosition();	// 敵の座標を取得
	m_angle	= enemy->GetAngle();		// 敵の回転を取得

	// ワールド行列を計算する
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(Cudgel::CUDGEL_SCALE)		// 大きさの設定　＆　リセット
		*= DirectX::SimpleMath::Matrix::CreateTranslation(Cudgel::DIRECTION_ENEMY)		// 原点の位置からすこしずらす
		*= DirectX::SimpleMath::Matrix::CreateRotationY(-m_angle)						// 剣全体の回転を行う
		*= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);					// 鬼の座標に移動する

	m_cudgel->SetCollisionPosition(m_worldMatrix);		// 当たり判定の位置を設定する
}


// -----------------------------------------------
// 事後処理
// -----------------------------------------------
void Cudgel_Idling::PostUpdate()
{
}


// -----------------------------------------------
// 描画処理
// -----------------------------------------------
void Cudgel_Idling::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// 敵がいない場合は描画しない
	if (m_cudgel->GetPlayScene()->GetEnemy() == nullptr) return;

	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);


#ifdef _DEBUG
	CommonResources* resources = CommonResources::GetInstance();
	auto debugString = resources->GetDebugString();
	debugString->AddString("");
#endif // !_DEBUG
}


// -----------------------------------------------
// 終了処理
// -----------------------------------------------
void Cudgel_Idling::Finalize()
{
}



// -----------------------------------------------
// 衝突判定
// -----------------------------------------------
void Cudgel_Idling::HitAction(InterSectData data)
{
	UNREFERENCED_PARAMETER(data);

	// Idling状態で衝突しても、特にはない
}
