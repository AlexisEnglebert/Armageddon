cbuffer Constant : register(b0)
{
    row_major float4x4 WorldMat;
    row_major float4x4 ProjectionMat;
    row_major float4x4 ViewMat;
    float Yoffset; //4 bytes
    float Xoffset; //4 bytes
    float color;
    float3 LightDir;
};

struct PSinput
{
    float4 position : SV_Position;
    float3 textCoord : TEXCOORD0;


};
TextureCube cSkyMap : register(t1);
//Texture2D SkyMap : register(t1);

SamplerState colorSampler : register(s1);

float4 ps_main(PSinput input) : SV_Target   
{
 
    
    float4 SkyBoxColor = cSkyMap.Sample(colorSampler, input.textCoord);

    return SkyBoxColor;


}