#include "Common.hlsli"

// 定数バッファ
cbuffer ConstBuffer : register(b1)
{
    float time;
    float3 padding;
};


SamplerState samLinear : register(s0);
Texture2D tex : register(t0);



float4 main(PS_Input input) : SV_Target0
{
    // 射影空間でのUVとして算出
    float2 screenUV = input.PositionPS.xy / input.PositionPS.w /5;
    
    // 指定された画像の表示
    float4 pattern = tex.Sample(samLinear, screenUV);

    // 元の頂点の色
    float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    
    // 消える境界ライン (時間に基づく)
    float boundary_min = 1 - time; // フェードアウト開始時点
    float boundary_max = 1 - time + 0.3f; // フェードアウト終了時点

    // rgbの平均値を取得
    float average = (pattern.r + pattern.g + pattern.b) / 3.0f;

    // グラデーション用の補間値を計算
    float t = smoothstep(boundary_min, boundary_max, average);

    // 黒から元の頂点の色 + 赤み要素 へのグラデーション
    float4 targetColor = color + float4(0.5f, 0.0f, 0.0f, 0.0f); // 頂点の色に赤を追加
    float4 gradColor = lerp(float4(0.0f, 0.0f, 0.0f, 1.0f), targetColor, t);
    
    // 最終的な透明度を設定
    gradColor.a = t; // 透明度もグラデーションに合わせる
    
    gradColor.rgb *= gradColor.a;
    
    return gradColor;
    
    //// 法線を正規化する
    //float3 normal = normalize(input.NormalWS);
    
    //// 入射ベクトルの逆ベクトルを正規化する→ライトがある方向に向かうベクトル
    //float3 toLight = normalize(-LightDirection[0]);
    
    //// 法線ベクトルとライトベクトルから反射光の強さを計算する(0-1)
    //float NdotL = dot(normal, toLight);
    //float intensity1 = max(NdotL, 0.0f);
    ////float intensity1 = max(dot(normal, toLight), 0.0f);

    //// 拡散反射の強さを計算する
    //float3 diffuse = DiffuseColor.rgb * LightDiffuseColor[0] * intensity1 + EmissiveColor;

    
    //diffuse.rgb -= time / 1.5f;
    //diffuse.r += time;
    
    //// 最終的な色を計算する：今回はあまり意味がない代入
    //float3 finalColor = diffuse;
    
    //return float4(finalColor, 0);
}