#include "DamageEffect.hlsli"
#include "Common.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4 pos = float4(input.Position, 1.0);

    // 頂点座標をワールド、ビュー、プロジェクション行列で変換
    output.Position = mul(pos, world);
    output.Position = mul(output.Position, view);
    output.Position = mul(output.Position, projection);

    // 頂点カラーをそのまま出力
    output.Color = input.Color;

    return output;
}