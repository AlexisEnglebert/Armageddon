#pragma once
#include <DirectXMath.h>


struct CB_VS_SHADER
{
	DirectX::XMMATRIX mat;
	float Yoffset = 0; //4 bytes
	float Xoffset = 0; //4 bytes
	float color = 0;  //4 bytes
	float a; //4X3 bytes
	float b;
	float c;
};

/*struct CB_PS_SHADER
{
	float LightDir = 0; //4 bytes
};*/
