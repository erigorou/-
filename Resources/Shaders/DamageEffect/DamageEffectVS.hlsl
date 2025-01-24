#include "Common.hlsli"


VS_Output main(VS_Input input)
{
    VS_Output output = (VS_Output) 0;
   
    // ローカルからワールドに変換
    output.PositionWS = mul(input.Position, World);
    
    // 法線にワールドを適用
    output.NormalWS = normalize(mul(input.Normal, (float3x3) World));
    
    // 射影空間上に座標を変換
    output.PositionPS = mul(input.Position, WorldViewProj);
   
    return output;
}