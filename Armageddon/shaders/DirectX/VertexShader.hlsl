cbuffer Constant : register(b0)
{
    row_major float4x4 mat;
    float Yoffset ; //4 bytes
    float Xoffset ; //4 bytes
    float color;
    float3 LightDir;
};


struct VSinput
{
    float3 pos : POSITION;
    float2 textCoord : TEXCOORD0;
    float3 normal : NORMAL0;
};
struct PSinput
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD0;
    float3 normal : TEXCOORD1;


};
PSinput vs_main(VSinput input)
{
    PSinput output = (PSinput) 0;
    
   // input.pos.y += Yoffset;
    output.position = /*float4(input.pos, 0.0f, 1.0f); */ mul(float4(input.pos, 1.0),mat);
    output.normal = mul(input.normal, (float3x3) mat);
    output.textCoord = input.textCoord;


    
    return output;


}


/*

*/ 