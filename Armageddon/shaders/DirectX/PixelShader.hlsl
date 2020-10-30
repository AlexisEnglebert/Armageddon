cbuffer Constant : register(b0)
{
    row_major float4x4 mat;
    float Yoffset; //4 bytes
    float Xoffset; //4 bytes
    float color;
    float3 LightDir;
};

struct PSinput
{
    float4 position : SV_Position;
    float2 textcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;


};
Texture2D colormap : register(t0);  
SamplerState colorSampler : register(s0);

float4 ps_main(PSinput input) : SV_Target
{
    float AmbianceFactor = 0.2f;
    float3 AmbianceColor = (1.0f, 1.0f, 1.0f);
    float3 AmbianceLight = AmbianceColor * AmbianceFactor;
    
    float DifuseFactor = 1.0f;
    float3 DifuseColor = (1.0f, 1.0f, 1.0f);
    float amount_diffuse = max(0.0f, dot(LightDir.xyz, input.normal));
    float3 DifuseLight = amount_diffuse * DifuseFactor * DifuseColor;
        
    
   // float3 colora = colormap.Sample(colorSampler, input.textcoord);
  
    //return float4(1.0f, 1.0f,1.0f,1.0f);
    float3 final = AmbianceLight + DifuseLight;
    return float4(final, 1.0f);


}