//--------------------------------------------------------------------------------------
// File: Particle.cpp
//
// パーティクルクラス
// 　メモ　シーングラフ
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Libraries/MyLib/BinaryFile.h"
#include "Effects/Particle.h"
#include "Effects/Header/DustTrailParticle.h"
#include "Effects/Header/SwordTrialParticle.h"

#include <random>



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
	,m_timerDustTrail(0.0f)
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

	// イテレータを取得して、削除対象の要素を削除
	m_dustTrail.remove_if([&elapsedTimer](DustTrialParticle& particle)
		{
			// 子クラスからfalseで消す
			return !particle.Update(elapsedTimer);
		}
	);

	// イテレータを取得して、削除対象の要素を削除
	m_swordTrial.remove_if([&elapsedTimer](SwordTrialParticle& particle)
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

	// パーティクル(１つ)を生成
	DustTrialParticle dTP(
		0.5f,																		//	生存時間(s)
		Vector3(m_playerPosition.x, m_playerPosition.y, m_playerPosition.z),		//	基準座標
		Vector3(-m_playerVelocity.x, 0.75f, -m_playerVelocity.z),					//	速度
		Vector3(1.0f, 0.1f, 1.0f),													//	加速度
		Vector3(2.0f, 2.0f, 2.0f), SimpleMath::Vector3(4.0f, 4.0f, 4.0f),			//	初期スケール、最終スケール
		Color(0.7f, 0.7f, 0.7f, 1.0f), SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.0f)	//	初期カラー、最終カラー
	);

	// 配列に追加
	m_dustTrail.push_back(dTP);
	// タイマーをリセット
	m_timerDustTrail = 0.0f;
}



/// <summary>
/// たたきつけの時に出る土埃を生成する
/// </summary>
/// <param name="center">たたきつけの位置</param>
void Particle::CreateSlamDust(DirectX::SimpleMath::Vector3 center)
{
	using namespace DirectX;

	for (int i = 0; i < 25; i++)
	{
		// 完全なランダムをハードウェア的に生成するためのクラスの変数
		std::random_device seed;
		// 上記の完全なランダムは動作が遅いため、seed値の決定のみに使用する
		// ※「default_random_engine」はusingで「mt19937」となっている
		std::default_random_engine engine(seed());
		// とばして欲しいランダムの範囲をDistributionに任せる。今回は0～2PI
		std::uniform_real_distribution<> dist(0, XM_2PI);
		float range = 5.0f;
		// ランダムな角度を生成する
		float randAngle = static_cast<float>(dist(engine));

		std::uniform_real_distribution<> dist2(0.5f, 2.0f);
		float Yspeed = static_cast<float>(dist2(engine));
		float XZspeed = static_cast<float>(dist2(engine));

		// 中心からのベクトルを生成
		SimpleMath::Vector3 vectorFromCenter = center + SimpleMath::Vector3(range * cosf(randAngle), 0.0f, range * sinf(randAngle)) - center;
		// ベクトルの長さを取得（距離）
		float distanceFromCenter = vectorFromCenter.Length();
		// 中心からのベクトルを正規化して方向を保持
		SimpleMath::Vector3 normalizedVectorFromCenter = vectorFromCenter / distanceFromCenter;
		// ベクトルを外側に広げるためのスケールを適用
		float scaleFactor = 1.0f + (distanceFromCenter / range); // rangeは円の半径
		SimpleMath::Vector3 adjustedVelocity = normalizedVectorFromCenter * scaleFactor;
		// 速度ベクトルを生成
		SimpleMath::Vector3 velocity = -adjustedVelocity;

		// 生成したダストの座標を取得する
		SimpleMath::Vector3 dustPosition = center + SimpleMath::Vector3(range * cosf(randAngle), 0.0f, range * sinf(randAngle));    // 基準座標

		// パーティクル生成
		DustTrialParticle pB(
			1.0f,																				// 生存時間(s)
			dustPosition,																		// 基準座標
			SimpleMath::Vector3{ -velocity.x * XZspeed, Yspeed, -velocity.z * XZspeed } * 2,	// 速度
			SimpleMath::Vector3(0.1f, 0.1f, 0.1f),												// 加速度
			SimpleMath::Vector3::One, SimpleMath::Vector3{ 10.0f, 25.0f, 10.0f },				// 初期スケール、最終スケール
			SimpleMath::Color(1.f, 1.f, 1.f, 1.f),												// 初期カラー
			SimpleMath::Color(1.f, 1.f, 1.f, 0.f)												// 最終カラー
		);

		m_dustTrail.push_back(pB);
	}
}



/// <summary>
/// 剣を降ったときの残像を出す
/// </summary>
/// <param name="ver"></param>
void Particle::CreateSwordTrial(DirectX::VertexPositionTexture ver[4])
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// SwordTrialParticleを生成
	SwordTrialParticle sTP(
		ver,															//	頂点情報
		0.4f,															//	生存時間(s)
		Color(1.0f, 1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 0.0f)	//	初期カラー、最終カラー
	);

	// 配列に追加
	m_swordTrial.push_back(sTP);
}





/// <summary>
/// シェーダーを生成する
/// </summary>
void Particle::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	コンパイルされたシェーダファイルを読み込み
	BinaryFile VSDataDust = BinaryFile::LoadFile(L"Resources/Shaders/Dust/DustVS.cso");
	BinaryFile GSDataDust = BinaryFile::LoadFile(L"Resources/Shaders/Dust/DustGS.cso");
	BinaryFile PSDataDust = BinaryFile::LoadFile(L"Resources/Shaders/Dust/DustPS.cso");

	BinaryFile VSDataSword = BinaryFile::LoadFile(L"Resources/Shaders/Sword/SwordTrialVS.cso");
	BinaryFile PSDataSword = BinaryFile::LoadFile(L"Resources/Shaders/Sword/SwordTrialPS.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSDataDust.GetData(), VSDataDust.GetSize(),
		m_inputLayout.ReleaseAndGetAddressOf());

	//device->CreateInputLayout(&INPUT_LAYOUT[1],
	//	static_cast<UINT>(INPUT_LAYOUT.size()),
	//	VSDataSword.GetData(), VSDataSword.GetSize(),
	//	m_inputLayout.ReleaseAndGetAddressOf());


	// 頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダーの生成
	if (FAILED(device->CreateVertexShader(VSDataDust.GetData(), VSDataDust.GetSize(), NULL, m_vertexShaderDust.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreateVS (Dust) Failed.", NULL, MB_OK);	return; }
	if (FAILED(device->CreateGeometryShader(GSDataDust.GetData(), GSDataDust.GetSize(), NULL, m_geometryShaderDust.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreateGS (Dust) Failed.", NULL, MB_OK);	return; }
	if (FAILED(device->CreatePixelShader(PSDataDust.GetData(), PSDataDust.GetSize(), NULL, m_pixelShaderDust.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreatePS (Dust) Failed.", NULL, MB_OK);	return; }

	// Sword用のシェーダーの生成
	if (FAILED(device->CreateVertexShader(VSDataSword.GetData(), VSDataSword.GetSize(), NULL, m_vertexShaderSword.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreateVS (Sword) Failed.", NULL, MB_OK);return; }
	if (FAILED(device->CreatePixelShader(PSDataSword.GetData(), PSDataSword.GetSize(), NULL, m_pixelShaderSword.ReleaseAndGetAddressOf())))
	{	MessageBox(0, L"CreatePS (Sword) Failed.", NULL, MB_OK);return; }



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
/// Render共通処理およびSwordTrialParticleとDustParticleの描画呼び出し
/// </summary>
/// <param name="states"></param>
/// <param name="view"></param>
/// <param name="proj"></param>
void Particle::Render(DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();												// カメラの方向を正規化

	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };			//	サンプラーステートの設定
	context->PSSetSamplers(0, 1, sampler);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();	//  半透明描画指定
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);		//	透明判定処理
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);		//	深度バッファはなし
	context->RSSetState(m_states->CullNone());						//	カリングなし

	//	ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)	{ context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf()); }

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	DrawSwordParticle(view, proj, cameraDir);	// 剣の残像の描画
	DrawDustParticle(view, proj, cameraDir);	// 土埃の描画
}



/// <summary>
/// 剣の残像パーティクルの描画
/// ジオメトリシェーダーやビルボードを使用せずに4頂点を使用してQuadを描画します。
/// </summary>
/// <param name="view"></param>
/// <param name="proj"></param>
/// <param name="cameraDir"></param>
void Particle::DrawSwordParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 cameraDir)
{
	using namespace DirectX;

	// 剣のパーティクルのためのコンスタントバッファの作成と更新
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = SimpleMath::Matrix::Identity; // 剣のパーティクルではビルボードを適用しない
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	context->VSSetShader(m_vertexShaderSword.Get(), nullptr,0);	// 頂点シェーダーを設定
	context->GSSetShader(nullptr, nullptr, 0);					// ジオメトリシェーダーは使用しない
	context->PSSetShader(m_pixelShaderSword.Get(), nullptr, 0);	// ピクセルシェーダーを設定

	// 剣の残像パーティクルを描画
	m_batch->Begin();

	float t = 0.0f;
	for (auto& sTP : m_swordTrial)
	{
		// 剣のパーティクルの4つの頂点を仮定
		DirectX::VertexPositionColorTexture ver[4];
		sTP.GetVertices(ver);
		
		float value1 = 1 - pow(1 - t, 2);	// イージング
		t += 1.0f / m_swordTrial.size();
		float value2 = 1 - pow(1 - t, 2);	// イージング	// t が 1に近づくほど色が薄くなる

		ver[1].color = DirectX::SimpleMath::Color(1, 0, 0, value1);
		ver[2].color = DirectX::SimpleMath::Color(1, 0, 0, value1);

		ver[0].color = DirectX::SimpleMath::Color(1, 0, 0, value2);
		ver[3].color = DirectX::SimpleMath::Color(1, 0, 0, value2);

		m_batch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	}
	m_batch->End();

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}



/// <summary>
/// 土埃パーティクルの描画
/// ジオメトリシェーダーとビルボードを適用して描画します。
/// </summary>
/// <param name="view"></param>
/// <param name="proj"></param>
/// <param name="cameraDir"></param>
void Particle::DrawDustParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 cameraDir)
{
	using namespace DirectX;

	// 土埃パーティクルのためのコンスタントバッファの作成と更新
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose(); // 土埃パーティクルではビルボードを適用する
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	context->VSSetShader(m_vertexShaderDust  .Get(), nullptr, 0);
	context->GSSetShader(m_geometryShaderDust.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShaderDust   .Get(), nullptr, 0);

	// 土埃パーティクルの頂点リストをクリア
	m_dustVertices.clear();
	for (DustTrialParticle& li : m_dustTrail)
	{
		// カメラの方向に向かないパーティクルはスキップ
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f)
			continue;

		// パーティクルの現在の情報を設定
		VertexPositionColorTexture vPCT;
		vPCT.position = XMFLOAT3(li.GetPosition());
		vPCT.color = XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_dustVertices.push_back(vPCT);
	}

	// パーティクルが存在する場合、描画を行う
	if (!m_dustVertices.empty())
	{
		// カメラとの距離に基づいてパーティクルをソート
		std::sort(m_dustVertices.begin(), m_dustVertices.end(),
			[&](VertexPositionColorTexture lhs, VertexPositionColorTexture rhs)
			{
				return cameraDir.Dot(lhs.position - m_cameraPosition) > cameraDir.Dot(rhs.position - m_cameraPosition);
			});

		// パーティクルの描画
		m_batch->Begin();
		m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_dustVertices[0], m_dustVertices.size());
		m_batch->End();
	}

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