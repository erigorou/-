//--------------------------------------------------------------------------------------
// File: Particle.cpp
//
// パーティクルクラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Libraries/MyLib/BinaryFile.h"
#include "Effects/Particle.h"



// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


// コンストラクタ
Particle::Particle()
	:m_pDR(nullptr)
	,m_timer_dustTrail(0.0f)
{
}


// デストラクタ
Particle::~Particle()
{
}


/// <summary>
/// テクスチャリソースの読み込み関数
/// </summary>
/// <param name="path">ファイルパス</param>
void Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}


/// <summary>
/// 生成関数
/// </summary>
void Particle::Create()
{
	// ComonResourcesを取得
	CommonResources* resources = CommonResources::GetInstance();
	// DeviceResourcesを取得
	m_pDR = resources->GetDeviceResources();
	// 1D3D11Deviceを取得
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	シェーダーの作成
	CreateShader();

	//	画像の読み込み（２枚ともデフォルトは読み込み失敗でnullptr)
	LoadTexture(L"Resources/Textures/dust.png");
	LoadTexture(L"Resources/Textures/dust.png");

	//	プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(resources->GetDeviceResources()->GetD3DDeviceContext());
	//  スタッツの作成
	m_states = std::make_unique<DirectX::CommonStates>(device);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTimer">フレーム間の秒数</param>
/// <param name="playerPosition">プレイヤーの座標</param>
/// <param name="playerVelocity">プレイヤーの速度</param>
void Particle::Update(float elapsedTimer, const DirectX::SimpleMath::Vector3 playerPosition, DirectX::SimpleMath::Vector3 playerVelocity)
{
	// 位置と速度を記録する
	m_playerPosition = playerPosition;
	m_playerVelocity = playerVelocity;
	// パーティクルを生成する
	CreateTrailDust(elapsedTimer);

	// イテレータを取得して、削除対象の要素を削除
	m_dustTrailParticle.remove_if([&elapsedTimer](DustTrialParticle& particle)
		{
		// 子クラスからfalseで消す
		return !particle.Update(elapsedTimer);
		}
	);
}


/// <summary>
/// 軌跡をたどる土埃の生成を行う
/// </summary>
/// <param name="elapsedTimer">フレーム間の秒数</param>
void Particle::CreateTrailDust(float elapsedTimer)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// 秒数を計測する
	m_timer_dustTrail += elapsedTimer;

	// 生成時間に満たしてないなら：早期リターン
	if (m_timer_dustTrail < 0.2f)
		return;

	// プレイヤーが移動してなければ：早期ターン
	if (m_playerVelocity == Vector3::Zero)
		return;

	// パーティクル(１つ)を生成
	DustTrialParticle dTP(
		0.5f,																					//	生存時間(s)
		SimpleMath::Vector3(m_playerPosition.x, m_playerPosition.y, m_playerPosition.z),		//	基準座標
		SimpleMath::Vector3(-m_playerVelocity.x, 1.0f, -m_playerVelocity.z),					//	速度
		SimpleMath::Vector3(1.0f, 0.2f, 1.0f),													//	加速度
		SimpleMath::Vector3(0.6f, 0.6f, 0.6f), SimpleMath::Vector3(0.9f, 0.9f, 0.9f),			//	初期スケール、最終スケール
		SimpleMath::Color(1.f, 1.f, 1.f, 1.f), SimpleMath::Color(1.f, 1.f, 1.f, 0.f)			//	初期カラー、最終カラー
	);

	// 配列に追加
	m_dustTrailParticle.push_back(dTP);
	// タイマーをリセット
	m_timer_dustTrail = 0.0f;
}


/// <summary>
/// シェーダーを生成する
/// </summary>
void Particle::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{//	エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	//	ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}


/// <summary>
/// 描画関数
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void Particle::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	頂点情報(板ポリゴンの４頂点の座標情報）

	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	m_vertices.clear();
	for (DustTrialParticle& li : m_dustTrailParticle)
	{
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) {
			//	内積がマイナスの場合はカメラの後ろなので表示する必要なし
			continue;
		}
		VertexPositionColorTexture vPCT;
		//	表示するパーティクルの中心座標のみを入れる。
		vPCT.position = XMFLOAT3(li.GetPosition());
		//	テクスチャの色
		vPCT.color = XMFLOAT4(li.GetNowColor());
		//	現在のテクスチャのスケールを「XMFLOAT2」のXに入れる。
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}


	//	表示する点がない場合は描画を終わる
	if (m_vertices.empty())
	{
		return;
	}

	std::sort(m_vertices.begin(), m_vertices.end(),
		[&](VertexPositionColorTexture lhs, VertexPositionColorTexture rhs)
		{
			//	カメラ正面の距離でソート
			DirectX::SimpleMath::Vector3 lhNowPos = lhs.position;
			DirectX::SimpleMath::Vector3 rhNowPos = rhs.position;
			return cameraDir.Dot(lhNowPos - m_cameraPosition) > cameraDir.Dot(rhNowPos - m_cameraPosition);
		});

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	半透明描画指定		補間アルファ合成
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファの書き込みを無効にする
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	//	カリングはなし
	context->RSSetState(m_states->CullNone());


	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	指定した座標を中心に、シェーダ側で板ポリゴンを生成・描画させる
	m_batch->Begin();

	// 描画処理
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());

	m_batch->End();

	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}



/// <summary>
/// ビルボード作成関数
/// </summary>
/// <param name="target">カメラターゲット（注視点）</param>
/// <param name="eye">カメラアイ（カメラ座標）</param>
/// <param name="up">上向きベクトル（基本はYのみ１のベクトル）</param>
void Particle::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	using namespace DirectX;

	// パーティクルがカメラに向くようにビルボード行列を作成する
	m_billboard =
		SimpleMath::Matrix::CreateBillboard
		(SimpleMath::Vector3::Zero, eye - target, up);

	// ビルボードを水平および垂直に反転させる回転行列を作成する
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1; // x軸を反転
	rot._33 = -1; // z軸を反転

	// カメラの位置とターゲットを保存する（将来的に使用するため）
	m_cameraPosition = eye;
	m_cameraTarget = target;

	// 回転行列をビルボード行列に適用する
	m_billboard = rot * m_billboard;
}