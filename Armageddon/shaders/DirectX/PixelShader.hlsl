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

cbuffer LightBuffer : register(b1)
{
    float lightIntensity;
}
struct PSinput
{
    float4 position : SV_Position;
    float2 textcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 DirToCam : TEXCOORD2;
    float3 color : COLOR0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    float VertexDeph : TEXCOORD3;




};
Texture2D colormap : register(t3);  
Texture2D Normalmap : register(t4);
Texture2D Specmap : register(t5);
Texture2D Occlusionmap : register(t6);
Texture2D BRDFLOOKUP : register(t7);

SamplerState colorSampler : register(s0);
TextureCube SkyMap : register(t1);
TextureCube IRSkyMap : register(t2);
static const float PI = 3.141592653589793;
static const float Epsilon = 0.00001;
#define CASCADE_COUNT_FLAG 3 // number of cascades



float normal_distribution_function(float3 normal, float3 h, float Roughness)
{
 
    float a = Roughness * Roughness;
    float a2 = a * a; // a2 = a^2
    float NdotH = max(dot(normal, h), 0.0); 
    float NdotH2 = NdotH * NdotH; 
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / max(denom, 0.00000001);
}

float G1(float cosLo, float Roughtness)
{
    return 2 * cosLo / cosLo + pow(sqrt(sqrt(Roughtness)) + (1.0f - sqrt(sqrt(Roughtness))) * sqrt(cosLo), 1 / 2); 
}
float GeometrySchlickGGX(float3 NdotV, float3 roughness)  
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}

float GeometryTerm(float3 normal,float3 Viewir,float3 LightDir,float3 Roughtness)
{
  
    float NdotV = max(dot(normal, Viewir), 0.0f);
    float NdotL = max(dot(normal, LightDir), 0.0f);
    float ggx1 = GeometrySchlickGGX(NdotV, Roughtness);
    float ggx2 = GeometrySchlickGGX(NdotL, Roughtness);

    return ggx1 * ggx2;

}

float fresnelSchlick(float3 F0, float cosTheta) // cosTheta = HdotV  et  F0 est la colormap 
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}
float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)   // cosTheta is n.v and F0 is the base reflectivity
{
    return F0 + (max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0f);
}
float3 Cook_Torance_BRDF(float3 lightDir, float3 ViewDir, float3 normal, float F0, float roughness, float metalness, float3 colormap, float3 irradiance)  // LI = light in  , LO = light out
{   

    
   
    float3 h = normalize(lightDir + ViewDir);  
 
    
    float F = fresnelSchlick(F0, max(0.0f, dot(h, ViewDir))); 
    float D = normal_distribution_function(normal, h, roughness);
    float G = GeometryTerm(normal, ViewDir, lightDir, roughness);

    

    
    float3 specular =  F * D * G / 4 * max(dot(normal, ViewDir), 0.0f) * max(dot(normal, lightDir), 0.0) + 0.001f; // 0.001f just in case product is 0
   
    
    float3 kS = FresnelSchlickRoughness(max(dot(normal, ViewDir), 0.0f), F0, roughness);

    
    float3 kD = 1.0f - kS;
    kD *= 1.0f - metalness;
    //float3 finalspec = (kD * colormap / PI + specular) * max(0, dot(normal, LightDir));

    float3 diffuse = colormap * irradiance;

    float2 BRDF_lookup = BRDFLOOKUP.Sample(colorSampler, float2(max(dot(normal, ViewDir), 0), roughness)).rg;
    float3 diffuseSpec = irradiance* (kS * BRDF_lookup.x + BRDF_lookup.y);
    float3 ambiant = kD * diffuse + specular;
    
    return ambiant;

}



// PBR  : https://blog.selfshadow.com/publications/s2013-shading-course/rad/s2013_pbs_rad_notes.pdf
// PBR : https://dirkiek.wordpress.com/2015/05/31/physically-based-rendering-and-image-based-lighting/
// PBR : https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// PBR : https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
// PBR (DIFFUSE) : https://graphics.stanford.edu/papers/envmap/envmap.pdf

float4 ps_main(PSinput input) : SV_Target
{
   
    
  
    
    float3 colorTex = colormap.Sample(colorSampler, input.textcoord); // albedo 
    float3 NormalTex = Normalmap.Sample(colorSampler, input.textcoord); // normal 
    float specTex = Specmap.Sample(colorSampler, input.textcoord).r;
    float OcclusionTex = Occlusionmap.Sample(colorSampler, input.textcoord).r; //metal
  
    
    NormalTex = normalize((2.0f * NormalTex) - 1.0f);
    input.tangent = normalize(input.tangent - dot(input.tangent, input.normal) * input.normal);
    float3 biTangent = cross(input.normal, input.tangent);
    float3x3 texSpace = float3x3(input.tangent, biTangent, input.normal);
    NormalTex = normalize(mul(NormalTex, texSpace));
    
    // Lambert Diffuse Lightning Equation D =  C * dot(N*Dir) / PI

    float3 DiffuseColor = float3(1.0f, 1.0f, 1.0f);
    float3 NormalDot = dot(NormalTex, LightDir); 
    float3 DiffuseLight = (DiffuseColor * NormalDot) / PI;
    

    //Cook/Torrence Microfacet Equation: f = D * F * G / (4 * (N.L) * (N.V))
    float F0 = lerp(0.04, colorTex, OcclusionTex);
    float3 irradiance = SkyMap.Sample(colorSampler, input.normal);

    float3 specular = Cook_Torance_BRDF(LightDir, input.DirToCam, NormalTex, F0, specTex, OcclusionTex, colorTex, irradiance); // DIR TO CAM PARAM 2*/
    
   
   // float3 prefilteredColor =
    float3 finalColor = colorTex + specular;
   
    /*CASCADE SHADOW*/
    float4 PixelDeph = input.VertexDeph;
    int CurrentCascadeIndex;
    /*float4 fComparison = (PixelDeph > m_fCascadeFrustumsEyeSpaceDepths_data[0]);
    float fIndex = dot(
        float4(CASCADE_COUNT_FLAG > 0,
        CASCADE_COUNT_FLAG > 1,
        CASCADE_COUNT_FLAG > 2,
        CASCADE_COUNT_FLAG > 3)
        , fComparison);*/

   /* fIndex = min(fIndex, CASCADE_COUNT_FLAG);
    CurrentCascadeIndex = (int) fIndex;*/
    return float4(finalColor, 1.0f);


}