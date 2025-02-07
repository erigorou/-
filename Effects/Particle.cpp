// --------------------------------------------------------------------
// パーティクルを一括管理するクラス
// --------------------------------------------------------------------

#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/GameResources.h"
#include "Effects/Particle.h"
#include "Libraries/MyLib/CustomShader/CustomShader.h"
#include "Game/Messenger/EventMessenger.h"
#include "Effects/Header/DustTrailParticle.h"
#include "Effects/Header/SwordTrailParticle.h"
#include <random>

// --------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
// --------------------------------------------------------
Particle::Particle()
	:
	m_timerDustTrail{},
	m_timerSwordTrail{}
{
	// 移動時の土埃を生成するイベントを登録
	EventMessenger::Attach(EventList::CreateTrailDust, std::bind(&Particle::CreateTrailDust, this));
	// 土煙を生成するイベントを登録
	EventMessenger::Attach(EventList::CreateBashDust, std::bind(&Particle::CreateBashDust, this, std::placeholders::_1));
	// 武器の残像を出すイベントを登録
	EventMessenger::Attach(EventList::CreateWeaponTrail, std::bind(&Particle::CreateSwordTrail, this, std::placeholders::_1));
}

// --------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
// --------------------------------------------------------
Particle::~Particle()
{
}

// --------------------------------------------------------
/// <summary>
/// 生成処理
/// </summary>
// --------------------------------------------------------
void Particle::Create()
{
	// リソースの取得
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	シェーダーの作成
	CreateShader();

	// 画像を取得
	m_texture = GameResources::GetInstance()->GetTexture("dust");

	//	プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	//  スタッツの作成
	m_states = std::make_unique<DirectX::CommonStates>(device);
}

// --------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTimer">経過時間</param>
/// <param name="playerPosition">プレイヤーの座標</param>
/// <param name="playerVelocity">プレイヤーの速度</param>
// --------------------------------------------------------
void Particle::Update(float elapsedTimer, const DirectX::SimpleMath::Vector3 playerPosition, DirectX::SimpleMath::Vector3 playerVelocity)
{
	// 位置と速度を記録する
	m_playerPosition = playerPosition;
	m_playerVelocity = playerVelocity;

	// イテレータを取得して、削除対象の要素を削除
	m_dustTrail.remove_if([&elapsedTimer](DustTrailParticle& particle)
		{
			// 子クラスからfalseで消す
			return !particle.Update(elapsedTimer);
		}
	);

	// イテレータを取得して、削除対象の要素を削除
	m_swordTrail.remove_if([&elapsedTimer](SwordTrailParticle& particle)
		{
			// 子クラスからfalseで消す
			return !particle.Update(elapsedTimer);
		}
	);
}

// --------------------------------------------------------
/// <summary>
/// 土埃生成処理
/// </summary>
// --------------------------------------------------------
void Particle::CreateTrailDust()
{
	// パーティクル(１つ)を生成
	DustTrailParticle dTP(
		0.5f,																									//	生存時間(s)
		DirectX::SimpleMath::Vector3(m_playerPosition.x, m_playerPosition.y, m_playerPosition.z),				//	基準座標
		DirectX::SimpleMath::Vector3(-m_playerVelocity.x, 0.75f, -m_playerVelocity.z),							//	速度
		DirectX::SimpleMath::Vector3(1.0f, 0.1f, 1.0f),															//	加速度
		DirectX::SimpleMath::Vector3(4.0f, 4.0f, 4.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),			//	初期スケール、最終スケール
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f), DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.0f)	//	初期カラー、最終カラー
	);

	// 配列に追加
	m_dustTrail.push_back(dTP);
	// タイマーをリセット
	m_timerDustTrail = 0.0f;
}


// --------------------------------------------------------
/// <summary>
/// 土煙生成処理
/// </summary>
/// <param name="center">中心座標</param>
// --------------------------------------------------------
void Particle::CreateBashDust(void* center)
{
	// 中心座標を取得
	DirectX::SimpleMath::Vector3 centerPos = *static_cast<DirectX::SimpleMath::Vector3*>(center);

	// 25個生成
	for (int i = 0; i < MAX_SMASH_ATTACK_DUST; i++)
	{
		// 完全なランダムをハードウェア的に生成するためのクラスの変数
		std::random_device seed;
		// 上記の完全なランダムは動作が遅いため、seed値の決定のみに使用する
		// ※「default_random_engine」はusingで「mt19937」となっている
		std::default_random_engine engine(seed());
		// とばして欲しいランダムの範囲をDistributionに任せる。今回は0～2PI
		std::uniform_real_distribution<> dist(0, DirectX::XM_2PI);
		// ランダムな角度を生成する
		float randAngle = static_cast<float>(dist(engine));

		// 速度のランダム生成
		std::uniform_real_distribution<> dist2(MIN_SMASH_DUST_SPEED_Y, MAX_SMASH_DUST_SPEED_Y);
		float Yspeed = static_cast<float>(dist2(engine) / 2.5f);
		float XZspeed = static_cast<float>(dist2(engine));

		// 中心からのベクトルを生成
		DirectX::SimpleMath::Vector3 vectorFromCenter = centerPos + DirectX::SimpleMath::Vector3
		(
			SMASH_DUST_RADIUS * cosf(randAngle),
			0.0f,
			SMASH_DUST_RADIUS * sinf(randAngle)
		) - centerPos;

		// ベクトルの長さを取得（距離）
		float distanceFromCenter = vectorFromCenter.Length();
		// 中心からのベクトルを正規化して方向を保持
		DirectX::SimpleMath::Vector3 normalizedVectorFromCenter = vectorFromCenter / distanceFromCenter;
		// ベクトルを外側に広げるためのスケールを適用
		float scaleFactor = 2.0f + (distanceFromCenter / SMASH_DUST_RADIUS);
		DirectX::SimpleMath::Vector3 adjustedVelocity = normalizedVectorFromCenter * scaleFactor;
		// 速度ベクトルを生成
		DirectX::SimpleMath::Vector3 velocity = -adjustedVelocity;

		// 生成したダストの座標を取得する
		DirectX::SimpleMath::Vector3 dustPosition = centerPos + DirectX::SimpleMath::Vector3
		(
			SMASH_DUST_RADIUS * cosf(randAngle),
			0.0f,
			SMASH_DUST_RADIUS * sinf(randAngle)
		);

		// パーティクル生成
		DustTrailParticle pB(
			0.9f, // 生存時間(s)
			dustPosition, // 基準座標
			DirectX::SimpleMath::Vector3{ -velocity.x * XZspeed, Yspeed , -velocity.z * XZspeed } *2, // 速度
			DirectX::SimpleMath::Vector3(0.1f, 0.1f, 0.1f),	// 加速度
			DirectX::SimpleMath::Vector3::One, // 初期スケール
			DirectX::SimpleMath::Vector3{ 10.0f, 25.0f, 10.0f }, // 最終スケール
			DirectX::SimpleMath::Color(1.f, 1.f, 1.f, 1.f), // 初期カラー
			DirectX::SimpleMath::Color(1.f, 1.f, 1.f, -1.f) // 最終カラー
		);

		m_dustTrail.push_back(pB);
	}
}

// --------------------------------------------------------
/// <summary>
/// 剣の残像生成処理
/// </summary>
/// <param name="ver">頂点情報</param>
// --------------------------------------------------------
void Particle::CreateSwordTrail(void* ver)
{
	// void* を適切な型にキャスト
	DirectX::VertexPositionTexture* vertices = static_cast<DirectX::VertexPositionTexture*>(ver);

	// SwordTrailParticleを生成
	SwordTrailParticle sTP(
		vertices,//	頂点情報
		0.2f,//	生存時間(s)
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f),	// 初期カラー
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.0f)	// 最終カラー
	);

	// 配列に追加
	m_swordTrail.push_back(sTP);
}


// --------------------------------------------------------
/// <summary>
/// シェーダーの生成処理
/// </summary>
// --------------------------------------------------------
void Particle::CreateShader()
{
	auto device = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// 斬撃用のシェーダーを生成
	m_swordShader = std::make_unique<CustomShader>
		(
			device,
			SWORD_VS,
			SWORD_PS,
			nullptr,
			InputElements
		);

	// 土煙用のシェーダーを生成
	m_dustShader = std::make_unique<CustomShader>
		(
			device,
			DUST_VS,
			DUST_PS,
			DUST_GS,
			InputElements
		);

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

// --------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
// --------------------------------------------------------
void Particle::Render(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj
)
{
	using namespace DirectX;
	CommonResources* resources = CommonResources::GetInstance();
	auto states = resources->GetCommonStates();
	auto context = resources->GetDeviceResources()->GetD3DDeviceContext();

	SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize(); // カメラの方向を正規化

	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };// サンプラーステートの設定
	context->PSSetSamplers(0, 1, sampler);

	ID3D11BlendState* blendstate = m_states->NonPremultiplied();// 半透明描画指定
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);// 透明判定処理
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);// 深度バッファはなし
	context->RSSetState(m_states->CullNone());// カリングなし

	//	ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	DrawSwordParticle(view, proj);				// 剣の残像の描画
	DrawDustParticle(view, proj, cameraDir);	// 土埃の描画
}

// --------------------------------------------------------
/// <summary>
/// 武器の残像パーティクルの描画
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
// --------------------------------------------------------
void Particle::DrawSwordParticle(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 剣のパーティクルのためのコンスタントバッファの作成
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity; // ビルボードは適用しない
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	// コンスタントバッファの更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// シェーダーの開始
	m_swordShader->BeginSharder(context);

	// ブレンドステートの設定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied(); //  半透明描画指定
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF); // 透明判定処理
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);// 深度バッファはなし
	context->RSSetState(m_states->CullNone());// カリングなし

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// 剣の残像パーティクルを描画
	m_batch->Begin();

	// 色を変化させるためのイージング変数
	float t = 0.0f;
	for (auto& sTP : m_swordTrail)
	{
		// 剣のパーティクルの4つの頂点を仮定
		DirectX::VertexPositionColorTexture ver[4];
		sTP.GetVertices(ver);

		float value1 = 1 - static_cast<float>(pow(1 - t, 2));	// イージング1
		t += 1.0f / m_swordTrail.size();
		float value2 = 1 - static_cast<float>(pow(1 - t, 2));	// イージング2

		ver[1].color = DirectX::SimpleMath::Color(1, 1, 1, value1);		// 右上
		ver[2].color = DirectX::SimpleMath::Color(1, 0.8, 0.8, 0);		// 右下

		ver[0].color = DirectX::SimpleMath::Color(1, 1, 1, value2);		// 左上
		ver[3].color = DirectX::SimpleMath::Color(1, 0.8, 0.8, 0);		// 左下

		m_batch->DrawQuad(ver[0], ver[1], ver[2], ver[3]);
	}

	m_batch->End();

	m_swordShader->EndSharder(context);
}

// --------------------------------------------------------
/// <summary>
/// 土埃パーティクルの描画
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
// --------------------------------------------------------
void Particle::DrawDustParticle(
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Vector3 cameraDir
)
{
	// 土埃パーティクルのためのコンスタントバッファの作成
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose(); // 土埃パーティクルではビルボードを適用する
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	// コンスタントバッファの更新
	auto context = CommonResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// シェーダーの開始
	m_dustShader->BeginSharder(context);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	// 土埃パーティクルの頂点リストをクリア
	m_dustVertices.clear();
	for (DustTrailParticle& li : m_dustTrail)
	{
		// カメラの方向に向かないパーティクルはスキップ
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f)
			continue;

		// パーティクルの現在の情報を設定
		DirectX::VertexPositionColorTexture vPCT;
		vPCT.position = DirectX::XMFLOAT3(li.GetPosition());
		vPCT.color = DirectX::XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = DirectX::XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_dustVertices.push_back(vPCT);
	}

	// パーティクルが存在する場合、描画を行う
	if (!m_dustVertices.empty())
	{
		// カメラとの距離に基づいてパーティクルをソート
		std::sort(m_dustVertices.begin(), m_dustVertices.end(),
			[&](DirectX::VertexPositionColorTexture lhs, DirectX::VertexPositionColorTexture rhs)
			{
				return cameraDir.Dot(lhs.position - m_cameraPosition) > cameraDir.Dot(rhs.position - m_cameraPosition);
			});

		// パーティクルの描画
		m_batch->Begin();
		m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_dustVertices[0], m_dustVertices.size());
		m_batch->End();
	}

	// シェーダーの終了
	m_dustShader->EndSharder(context);
}

// --------------------------------------------------------
/// <summary>
/// ビルボード行列の作成
/// </summary>
/// <param name="target">ターゲット</param>
/// <param name="eye">カメラの位置</param>
/// <param name="up">上方向</param>
// --------------------------------------------------------
void Particle::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	// パーティクルがカメラに向くようにビルボード行列を作成する
	m_billboard =
		DirectX::SimpleMath::Matrix::CreateBillboard
		(DirectX::SimpleMath::Vector3::Zero, eye - target, up);

	// ビルボードを水平および垂直に反転させる回転行列を作成する
	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::Identity;
	rot._11 = -1; // x軸を反転
	rot._33 = -1; // z軸を反転

	// カメラの位置とターゲットを保存する（将来的に使用するため）
	m_cameraPosition = eye;
	m_cameraTarget = target;

	// 回転行列をビルボード行列に適用する
	m_billboard = rot * m_billboard;
}