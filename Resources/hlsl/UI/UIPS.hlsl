#include "UI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{	
    
	//	指定された画像の表示
    float4 output = tex.Sample(samLinear, input.tex);

    // 色情報を変更する
    output.r *= uiColor.r;
    output.g *= uiColor.g;
    output.b *= uiColor.b;
    
    return output;
}