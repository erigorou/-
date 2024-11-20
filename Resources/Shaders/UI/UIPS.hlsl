#include "UI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//	�w�肳�ꂽ�摜�̕\��
	float4 output = tex.Sample(samLinear, input.tex);

	//	�^�����Ȕ|���S��
	float4 outputw = float4(1, 1, 1, 1);

	
    output.a *= lerp(1.0f, 0.0f, smoothstep(drawRatio.x, drawRatio.x + 0.5f, input.tex.x));
	
	return output;
}