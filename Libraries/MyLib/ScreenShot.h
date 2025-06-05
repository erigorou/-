// ------------------------------------------------------------
//
// インゲームのスクリーンショットを取るためのクラス
// リザルトシーンで討伐時の瞬間を撮影するために使用
//
// ------------------------------------------------------------

#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include "Game/Data/GameData.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

class ScreenShot
{
// -----------------------
// アクセサ
// ------------------------
public:
	// スクリーンショットを取得する
	static void TakeScreenshot()
	{
		// スクリーンショットがもうすでにある場合は実行しない
		if (GameData::GetInstance()->GetScreenShot()) return;
		// スクショ許可フラグが立っていない場合は実行しない
		if (!GameData::GetInstance()->IsCaptureFlag()) return;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> captureSRV;

		// 共通リソースの取得
		auto deviceResources = CommonResources::GetInstance()->GetDeviceResources();
		ID3D11Device* device = deviceResources->GetD3DDevice();
		ID3D11DeviceContext* context = deviceResources->GetD3DDeviceContext();

		// 現在のレンダーターゲットビューを取得
		ID3D11RenderTargetView* currentRTV = deviceResources->GetRenderTargetView();
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		currentRTV->GetResource(&backBuffer);

		// バックバッファテクスチャを取得
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferTex;
		backBuffer.As(&backBufferTex);

		// バックバッファのテクスチャ設定を取得
		D3D11_TEXTURE2D_DESC desc = {};
		backBufferTex->GetDesc(&desc);

		// GPU 用の DEFAULT テクスチャを作成
		desc.Usage = D3D11_USAGE_DEFAULT; // GPU で使用可能にする
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // シェーダーリソースとしてバインド
		desc.CPUAccessFlags = 0; // CPU からのアクセスは不要
		Microsoft::WRL::ComPtr<ID3D11Texture2D> gpuTexture;
		HRESULT hr = device->CreateTexture2D(&desc, nullptr, &gpuTexture);
		if (FAILED(hr)) return;

		// CPU 読み取り用の STAGING テクスチャを作成
		desc.Usage = D3D11_USAGE_STAGING; // CPU 読み取り専用
		desc.BindFlags = 0; // バインド不要
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU で読み取り可能に設定
		Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
		hr = device->CreateTexture2D(&desc, nullptr, &stagingTexture);
		if (FAILED(hr)) return;

		// GPU のレンダーターゲットから STAGING テクスチャにデータをコピー
		context->CopyResource(stagingTexture.Get(), backBufferTex.Get());

		// STAGING テクスチャから DEFAULT テクスチャに再コピー
		context->CopyResource(gpuTexture.Get(), stagingTexture.Get());

		// シェーダーリソースビューを作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format; // フォーマットを設定
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; // 2D テクスチャとして設定
		srvDesc.Texture2D.MipLevels = 1; // ミップマップレベル数を指定
		srvDesc.Texture2D.MostDetailedMip = 0; // 最初のミップマップレベルを指定

		hr = device->CreateShaderResourceView(gpuTexture.Get(), &srvDesc, &captureSRV);
		if (FAILED(hr)) {
			MessageBoxA(nullptr, "スクショ作成失敗！", "Error", MB_OK | MB_ICONERROR);
			return;
		}

		// スクショの保存
		GameData::GetInstance()->SetScreenShot(std::move(captureSRV));
	}

};