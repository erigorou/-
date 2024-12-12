#include "DamageEffect.hlsli"
#include "Common.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4 pos = float4(input.Position, 1.0);

    // ���_���W�����[���h�A�r���[�A�v���W�F�N�V�����s��ŕϊ�
    output.Position = mul(pos, world);
    output.Position = mul(output.Position, view);
    output.Position = mul(output.Position, projection);

    // ���_�J���[�����̂܂܏o��
    output.Color = input.Color;

    return output;
}