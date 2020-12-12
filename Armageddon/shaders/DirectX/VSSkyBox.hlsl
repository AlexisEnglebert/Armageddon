cbuffer Constant : register(b0)
{
    row_major float4x4 WorldMat;
    row_major float4x4 ProjectionMat;
    row_major float4x4 ViewMat;
    row_major float4x4 MVP;

    float Yoffset; //4 bytes
    float Xoffset; //4 bytes
    float color;
    float3 LightDir;
    float3 CamPos;
};


struct VSinput
{
    float3 pos : POSITION;
    float2 textCoord : TEXCOORD0;

};
struct PSinput
{
    float4 position : SV_Position;
    float3 textCoord : TEXCOORD0;

};
PSinput vs_main(VSinput input)
{
    PSinput output = (PSinput) 0;
    
   // input.pos.y += Yoffset;
    float4x4 mat = mul(WorldMat, MVP);
    output.position = mul(float4(input.pos, 1.0), mat);
    output.textCoord = input.pos;
    
    return output;


}


/*

*/ 