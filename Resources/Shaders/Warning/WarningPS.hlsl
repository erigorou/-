#include "../Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);

    //// 透明な部分（アルファが0）の場合は、そのまま返す
    //if (output.a < 0.2f)
    //{
    //    output.a = 0;
    //    return output;
    //}

    // 元のアルファ値に対して変動を加える
    float alphaVariation = (0.2 - sin(time.x * 2)) * 0.5f; // 0～1にスケーリング
    output.a *= alphaVariation; // 元のアルファ値に影響を与える
    output.r = 1;
    
    return output;
}