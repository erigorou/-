#include "../Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`�����T���v�����O
    float4 output = tex.Sample(samLinear, input.Tex);

    //// �����ȕ����i�A���t�@��0�j�̏ꍇ�́A���̂܂ܕԂ�
    //if (output.a < 0.2f)
    //{
    //    output.a = 0;
    //    return output;
    //}

    // ���̃A���t�@�l�ɑ΂��ĕϓ���������
    float alphaVariation = (0.2 - sin(time.x * 2)) * 0.5f; // 0�`1�ɃX�P�[�����O
    output.a *= alphaVariation; // ���̃A���t�@�l�ɉe����^����
    output.r = 1;
    
    return output;
}