cbuffer Constant : register(b0)
{
    row_major float4x4 WorldMat;
    row_major float4x4 ProjectionMat;
    row_major float4x4 ViewMat;
    row_major float4x4 MVP;
    float Yoffset; //4 bytes
    float Xoffset; //4 bytes
    float3 LightDir;
    float3 CamPos;

};


struct VSinput
{
    float3 pos : POSITION;
    float2 textCoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 color : COLOR0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};
struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 DirToCam : TEXCOORD2;
    float3 color : COLOR0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    float VertexDeph : TEXCOORD3;

};
PSinput vs_main(VSinput input)
{
    PSinput output = (PSinput) 0;
    
    //   row_major float4x4 WorldMat;
    //row_major float4x4 ProjectionMat;
  //  row_major float4x4 ViewMat;
   // input.pos.y += Yoffset;
    float4x4 mat = mul(WorldMat, MVP);
    output.position = /*float4(input.pos, 0.0f, 1.0f); */mul(float4(input.pos, 1.0), mat);
    output.DirToCam = normalize(CamPos - (mul(float4(input.pos, 1.0f), WorldMat).xyz));
    output.VertexDeph = mul(input.pos, (float3x4) WorldMat).z;
    output.normal = normalize(mul(input.normal, WorldMat));
    output.textCoord = input.textCoord;
    output.color = input.color;
    output.tangent = input.tangent;
    output.binormal = input.binormal;
    
    return output;


}

