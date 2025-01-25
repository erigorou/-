/*
    ■DirectXTKで標準的に定義されているパラメータ
    ・レジスタのゼロ番目はここで使用されている
*/
Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);

// 頂点シェーダ用構造体
struct VS_Input
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    //float2 TexCoord : TEXCOORD0;
};


// ピクセルシェーダ用構造体
struct PS_Input
{
    float4 PositionWS : TEXCOORD0;
    float3 NormalWS : TEXCOORD1;
    //float2 TexCoord : TEXCOORD2;
    float4 PositionPS : SV_Position;
};

//  頂点シェーダ用出力構造体
typedef PS_Input VS_Output;

//// ピクセルシェーダ用構造体
//struct PS_Input
//{
//    float4 PositionWS : TEXCOORD0;
//    float3 NormalWS : TEXCOORD1;
//    float4 Diffuse : COLOR0;
//    float2 UV : TEXCOORD2;
//};



cbuffer Parameters : register(b0)
{
    float4 DiffuseColor : packoffset(c0);
    float3 EmissiveColor : packoffset(c1);
    float3 SpecularColor : packoffset(c2);
    float SpecularPower : packoffset(c2.w);

    float3 LightDirection[3] : packoffset(c3);
    float3 LightDiffuseColor[3] : packoffset(c6);
    float3 LightSpecularColor[3] : packoffset(c9);

    float3 EyePosition : packoffset(c12);

    float3 FogColor : packoffset(c13);
    float4 FogVector : packoffset(c14);

    float4x4 World : packoffset(c15);
    float3x3 WorldInverseTranspose : packoffset(c19);
    float4x4 WorldViewProj : packoffset(c22);
};
