#include "Common.hlsli"


VS_Output main(VS_Input input)
{
    VS_Output output = (VS_Output) 0;

    // ローカルからワールドに変換（float4でmulする場合はw=1.0を明示）
    output.PositionWS = mul(float4(input.Position, 1.0f), World);

    // 法線にワールド行列（回転部分のみ）を適用
    output.NormalWS = normalize(mul(input.Normal, (float3x3) World));

    // 射影空間上に座標を変換
    output.PositionPS = mul(float4(input.Position, 1.0f), WorldViewProj);

    return output;
}