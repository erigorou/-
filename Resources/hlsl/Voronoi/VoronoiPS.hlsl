#include "../Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);



// 乱数生成用の関数
float2 random2(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

// Voronoiノイズ生成用の関数
float voronoiNoise(float2 uv)
{
    float2 p = floor(uv);
    float2 f = frac(uv);
    
    float minDist = 1.0;
    
    for (int j = -1; j <= 1; ++j)
    {
        for (int i = -1; i <= 1; ++i)
        {
            float2 g = float2(i, j);
            float2 randomPoint = random2(p + g);
            float2 diff = g + randomPoint - f;
            float dist = length(diff);
            
            minDist = min(minDist, dist);
        }
    }
    
    return minDist;
}

// 柔らかい歪み用の関数
float2 subtleDistortUV(float2 uv, float time)
{
    float distortionStrengthX = 0.08; // 横方向の歪みの強さ
    float distortionStrengthY = 0.06; // 縦方向の歪みの強さ

    uv.x += sin(uv.y * 8.0 + time * 1.5) * distortionStrengthX;
    uv.y += cos(uv.x * 8.0 + time * 1.2) * distortionStrengthY;
    return uv;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // ブロックの数
    float scale = 20.0;
    float2 uv = input.Tex * scale;

    // 基本のUV歪み
    uv = subtleDistortUV(uv, time.x);

    // 追加の動き
    uv += float2(time.x * 0.5, time.x * 0.3);

    // 複数のVoronoiノイズレイヤー
    float noiseValue1 = voronoiNoise(uv);
    float noiseValue2 = voronoiNoise(uv * 0.6 + float2(time.x * 0.02, time.x * 0.015));
    float noiseValue3 = voronoiNoise(uv * 1.2 + float2(time.x * 0.01, time.x * 0.005));

    // ノイズの合成
    float noiseValue = (noiseValue1 + noiseValue2 + noiseValue3) / 3.0;

    // 色のグラデーション（深い色と浅い色を調整）
    float3 deepColor = float3(0.4, 0.6, 0.9);
    float3 lightColor = float3(0.4, 0.7, 0.9);
    float3 oceanColor = lerp(deepColor, lightColor, smoothstep(0.3, 0.7, noiseValue));

    // ハイライトの追加（光の反射） - 範囲と強度をさらに広げる
    float highlight = smoothstep(0.5, 0.95, noiseValue) * 1.2; // ハイライト範囲と強度の調整
    oceanColor += highlight * float3(0.9, 1.1, 1.3); // より白っぽく

    return float4(oceanColor, 1.0);
}