// -----------------------------------------------------------------
// * 天球によって違和感が出ないようにする為のステージ
// -----------------------------------------------------------------
#include "pch.h"
#include <Model.h>
#include <cassert>
// ヘッダー
#include "Game/CommonResources.h"	
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include "DeviceResources.h"

#include "Wall.h"

// 固定値
const float Wall::WALL_SCALE = 2.0f;

// コンストラクタ
Wall::Wall()
	:m_worldMatrix()
	,m_model()
{
}

// デストラクタ
Wall::~Wall()
{
}

// 初期化処理
void Wall::Initialize()
{
	CommonResources* resources = CommonResources::GetInstance();

	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = resources->GetCommonStates();

	// モデルを生成
	m_model = std::make_unique<DirectX::Model>();

	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/wall.cmo", *fx);
}


// ワールド座標更新処理
void Wall::UpdateWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_worldMatrix = Matrix::Identity;									// 初期化
	m_worldMatrix *= Matrix::CreateScale(WALL_SCALE);		// 拡大　縮小
}


// 描画
void Wall::Render(ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection)
{
	// ワールド行列の更新処理
	UpdateWorldMatrix();
	// モデルを描画する
	m_model->Draw(context, *states, m_worldMatrix, view, projection);
}


// 終了処理
void Wall::Finalize()
{
}