// -------------------------------------
// 
// 名前;	Sea.h
// 内容:	海の描画を担当するクラス
// 作成:	池田桜輔
// 
// -------------------------------------
// ヘッダーファイル
#include "pch.h"
#include "Sea.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "../../Data/GameData.h"

// ---------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// ---------------------------------------------------------
Sea::Sea()
	:
	m_totalTime{}
{
	Create();
}

// ---------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// ---------------------------------------------------------
Sea::~Sea()
{
}

// ---------------------------------------------------------
/// <summary>
/// 生成を行う
/// </summary>
// ---------------------------------------------------------
void Sea::Create()
{
	CommonResources* resources = CommonResources::GetInstance();
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// カスタムシェーダー群の生成処理
	m_customShader = std::make_unique<CustomShader>
		(device,
			VS_PATH,
			PS_PATH,
			GS_PATH,
			InputElements
		);

	// プリミティブバッチの生成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);

	// コモンステートの生成
	m_states = std::make_unique<DirectX::CommonStates>(device);

	// シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	// エラーチェッカー付き
	HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	if (FAILED(hr)) {
		MessageBox(0, L"コンスタントバッファの生成に失敗しました.", NULL, MB_OK);
	}
}

// ---------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
// ---------------------------------------------------------
void Sea::Render(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	CommonResources* resources = CommonResources::GetInstance();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(0.0f,  0.0f, 0.0f),
			DirectX::SimpleMath::Vector4(0.0f,0.0f,0.0f,1.0f),
			DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	// 渡す用のデータを作成
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;
	world *= DirectX::SimpleMath::Matrix::CreateScale(SEA_SCALE);
	world *= DirectX::SimpleMath::Matrix::CreateRotationX(DirectX::XMConvertToRadians(SEA_RPTATION_X_DEG));
	// 経過時間の更新
	m_totalTime += GameData::GetInstance()->GetElapsedTime() * SEA_WAVE_SPEED;

	// バッファの作成
	ConstBuffer cbuff;
	cbuff.matWorld = world.Transpose();
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
	cbuff.easing = DirectX::SimpleMath::Vector4(0, 0, 0, 0);
	cbuff.time = DirectX::SimpleMath::Vector4(m_totalTime, 0, 0, 0);

	// 受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// 半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// カリングは左周り
	context->RSSetState(m_states->CullCounterClockwise());
	// シェーダーの登録
	m_customShader->BeginSharder(context);

	// 描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
	m_batch->End();

	// シェーダーの登録を解除
	m_customShader->EndSharder(context);
}
