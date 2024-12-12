cbuffer ConstBuffer : register(b1)
{
    matrix world;
    matrix view;
    matrix projection;
    float Time;
    float3 pudding;
}

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float3 Tangetnt : TANGENT;
    float4 Color    : COLOR;
    float2 TexCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
    float2 TexCoord : TEXCOORD;
};