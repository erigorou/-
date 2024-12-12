#include "DamageEffect.hlsli"
#include "Common.hlsli"

Texture2D shaderTexture : register(t0);
SamplerState sampleState : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = DiffuseColor;
    
    color.rgb += 0.2f;
    
    color.r += Time;
    
    return color;
}
