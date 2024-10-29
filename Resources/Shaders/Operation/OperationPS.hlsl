#include "../Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);
    float v = 0.5f;
    float4 push = (v, v, v, v);
    
    // キー入力があったときに少し暗くする
    output -= push * time.x;
    
    return output;
}