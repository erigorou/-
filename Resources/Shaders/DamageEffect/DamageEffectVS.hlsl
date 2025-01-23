#include "Common.hlsli"

//struct PS_Output
//{
//    float4 color : SV_Target0;
//    float4 normal : SV_Target1;
//    float4 depth : SV_Target2;
//};

//PS_Output main(PS_Input input)
//{
//    PS_Output output;
        
//    output.color = float4(1, 1, 1, 1);

//    return output;
//}


VS_Output main(VS_Input input)
{
    VS_Output output = (VS_Output) 0;
   
    // ���[�J�����烏�[���h�ɕϊ�
    output.PositionWS = mul(input.Position, World);
    
    // �@���Ƀ��[���h��K�p
    output.NormalWS = normalize(mul(input.Normal, (float3x3) World));
    
    // �ˉe��ԏ�ɍ��W��ϊ�
    output.PositionPS = mul(input.Position, WorldViewProj);
    
    // UV���W�����̂܂܎g��
    // output.TexCoord = input.TexCoord;
   
    return output;
}