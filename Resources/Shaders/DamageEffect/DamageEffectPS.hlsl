#include "Common.hlsli"

// �萔�o�b�t�@
cbuffer ConstBuffer : register(b1)
{
    float time;
    float3 padding;
};

// �s�N�Z���V�F�[�_�p�\����
struct PS_Input
{
    float4 PositionWS   : TEXCOORD0;
    float3 NormalWS     : TEXCOORD1;
    float4 Diffuse      : COLOR0;
};

float4 main(PS_Input input) : SV_Target0
{
    // �@���𐳋K������
    float3 normal = normalize(input.NormalWS);
    
    // ���˃x�N�g���̋t�x�N�g���𐳋K�����遨���C�g����������Ɍ������x�N�g��
    float3 toLight = normalize(-LightDirection[0]);
    
    // �@���x�N�g���ƃ��C�g�x�N�g�����甽�ˌ��̋������v�Z����(0-1)
    float NdotL = dot(normal, toLight);
    float intensity1 = max(NdotL, 0.0f);
    //float intensity1 = max(dot(normal, toLight), 0.0f);

    // �g�U���˂̋������v�Z����
    float3 diffuse = DiffuseColor.rgb * LightDiffuseColor[0] * intensity1 + EmissiveColor;

    
    diffuse.rgb -= time / 1.5f;
    diffuse.r += time * 2;
    
    // �ŏI�I�ȐF���v�Z����F����͂��܂�Ӗ����Ȃ����
    float3 finalColor = diffuse;
    
    return float4(finalColor, 1.0f);
}