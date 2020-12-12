#pragma once
#include <DirectXMath.h>


struct CB_VS_SHADER
{
	DirectX::XMMATRIX WorldMat;
	DirectX::XMMATRIX ProjectionMat;
	DirectX::XMMATRIX ViewMat;
	DirectX::XMMATRIX MVP;
	
	//DirectX::XMMATRIX TransformMat;
	float Yoffset = 0; //4 bytes
	float Xoffset = 0; //4 bytes
	DirectX::XMFLOAT3A LightDir = { 0,0,0 };
	DirectX::XMFLOAT3A CameraPos = {0,0,0};
	float RoughNess = 0.0;
	float MetalNess = 0.0;


	//float EyePos = 0;
};
struct LightBuffer
{
	float LightIntensity;
	//DirectX::XMFLOAT3 LightDir = { 0,0,0 };


};
/*struct CB_PS_SHADER
{
	float LightDir = 0; //4 bytes
};*/
