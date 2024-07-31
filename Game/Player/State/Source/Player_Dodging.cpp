#include "pch.h"
#include <cassert>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/Math.h"

#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Player/State/Header/Player_Dodging.h"

// 固定値
const float PlayerDodging::DODGING_TIME = 0.5f;
const float PlayerDodging::DODGE_FUNCTION = 0.9f;
const int   PlayerDodging::TRUNCATION_DIGIT = 3;

// コンストラクタ
PlayerDodging::PlayerDodging(Player* player)
	:
	 m_player(player)
	,m_model(nullptr)
	,m_totalSeconds(0.0f)
	,m_direction(0.0f, 0.0f, 0.0f)
	,m_finishTime(0.0f)
{
}

// デストラクタ
PlayerDodging::~PlayerDodging()
{
}


// 初期化処理
void PlayerDodging::Initialize(DirectX::Model* model)
{
	// モデルを取得する
	m_model = model;
	// 体の当たり判定の生成
	m_boundingSphereBody = DirectX::BoundingSphere();
	// 体の当たり判定のサイズや座標を設定
	m_boundingSphereBody.Radius = Player::PLAYER_SCALE * 12.f;
}

// 事前更新処理
void PlayerDodging::PreUpdate()
{
	// 経過時間の初期化
	m_totalSeconds = 0.f;
	// 回避する方向を取得
	m_velocity = m_player->GetVelocity() * 4.f;
	// 当たり判定をプレイヤーの位置に修正
	m_boundingSphereBody.Center = m_player->GetPosition();
}

// 更新処理
void PlayerDodging::Update(const float& elapsedTime, DirectX::SimpleMath::Vector3& parentPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// ステート開始から時間を計測、一定時間で別のStateへ遷移させる
	m_player->TimeComparison(m_totalSeconds, DODGING_TIME, m_player->GetPlayerIdlingState(), elapsedTime);

	// プレイヤーの回転を取得
	Matrix angle = Matrix::CreateRotationY(-m_player->GetAngle());
	// 速度を設定
	m_velocity *= DODGE_FUNCTION;
	// 端数を消し飛ばす。
	m_velocity = Math::truncate_vector(m_velocity, TRUNCATION_DIGIT);
	// 移動量を座標に反映させながら座標を移動させる。
	parentPos +=Vector3::Transform(m_velocity,angle);

	// 埋め込み量の計算をした後にそれを反映させる
	parentPos += CalculatingPushBack();
	m_boundingSphereBody.Center = parentPos;
}


// 事後更新処理
void PlayerDodging::PostUpdate()
{
	// 修正点があればここに記載
}


// 描画処理
void PlayerDodging::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
	)
{
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(states);
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(projection);
	UNREFERENCED_PARAMETER(m_model);




	// コモンリソースを取得する
	CommonResources* resources = CommonResources::GetInstance();

	// デバッグ文字の描画
	auto debugString = resources->GetDebugString();
	debugString->AddString("PlayerDodging");

}


// 終了処理
void PlayerDodging::Finalize()
{
}


// 体に当たったときに押し戻しをする
DirectX::SimpleMath::Vector3 PlayerDodging::CalculatingPushBack()
{
	// プレイヤーを一度変換する
	auto player = dynamic_cast<Player*>(m_player);
	// ボディを取得						プレイヤー → シーン → エネミー → 現在のステート → 体の当たり判定
	DirectX::BoundingSphere enemyBody = player->GetPlayScene()->GetEnemy()->GetCurrentState()->GetBoundingSphereBody();
	// 押し戻し量の計測
	return Math::pushBack_BoundingSphere(m_boundingSphereBody, enemyBody);
}