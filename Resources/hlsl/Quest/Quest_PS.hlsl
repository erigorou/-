#include "Quest.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);

SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // 指定された画像の表示
    float4 quest = tex.Sample(samLinear, input.Tex);
    float4 pattern = tex2.Sample(samLinear, input.Tex);
    
    // テクスチャが無効な場合は何も描画しない
    float validTexture = step(0.01f, dot(quest.rgb, float3(1.0f, 1.0f, 1.0f)));
    
    // ディゾルブが 0 の場合はそのまま quest を出力
    float dissolveActive = step(0.01f, dissolve);
    
    // 消える境界ライン (時間に基づく)
    float boundary_min = dissolve; // フェードアウト開始時点
    float boundary_max = dissolve + 0.3f; // フェードアウト終了時点

    // rgbの平均値を取得
    float average = (pattern.r + pattern.g + pattern.b) / 3.0f;

    // グラデーション用の補間値を計算
    float t = smoothstep(boundary_min, boundary_max, average);


    float4 gradientColor = lerp(
        lerp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(1.0f, 0.5f, 0.0f, 1.0f), t),
        quest, t * 2 );
    
    // ディゾルブ適用（dissolve.x が 0 の場合は適用しない）
    float dissolveFactor = lerp(1.0f, t, dissolveActive);
    
    // 色情報なしか、クエストを表示するか
    float4 gradColor = lerp(gradientColor, quest, dissolveFactor);
    
    // 最終的な透明度を設定
    gradColor.a = dissolveFactor * validTexture;
    
    // 透明度をかける
    gradColor *= alpha;
    
    return gradColor;
}
