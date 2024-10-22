#include "../Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);

    // 元のアルファ値に対して変動を加える
    float alphaVariation = sin(time.x * 2) * 0.5f;    // 0～1にスケーリング
    output.a *= alphaVariation / 2;                   // 元のアルファ値に影響を与える
    output.r = 0.7f;
    
    return output;
}6