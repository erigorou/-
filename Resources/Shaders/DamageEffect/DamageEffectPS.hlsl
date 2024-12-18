#include "Common.hlsli"

// 定数バッファ
cbuffer ConstBuffer : register(b1)
{
    float time;
    float3 padding;
};

// ピクセルシェーダ用構造体
struct PS_Input
{
    float4 PositionWS   : TEXCOORD0;
    float3 NormalWS     : TEXCOORD1;
    float4 Diffuse      : COLOR0;
};

float4 main(PS_Input input) : SV_Target0
{
    // 法線を正規化する
    float3 normal = normalize(input.NormalWS);
    
    // 入射ベクトルの逆ベクトルを正規化する→ライトがある方向に向かうベクトル
    float3 toLight = normalize(-LightDirection[0]);
    
    // 法線ベクトルとライトベクトルから反射光の強さを計算する(0-1)
    float NdotL = dot(normal, toLight);
    float intensity1 = max(NdotL, 0.0f);
    //float intensity1 = max(dot(normal, toLight), 0.0f);

    // 拡散反射の強さを計算する
    float3 diffuse = DiffuseColor.rgb * LightDiffuseColor[0] * intensity1 + EmissiveColor;

    
    diffuse.rgb -= time / 1.5f;
    diffuse.r += time * 2;
    
    // 最終的な色を計算する：今回はあまり意味がない代入
    float3 finalColor = diffuse;
    
    return float4(finalColor, 1.0f);
}