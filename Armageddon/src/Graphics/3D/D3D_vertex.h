#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z,  float u, float v, float nx, float ny, float nz) 
		: pos(x, y, z), textcoord(u, v) ,normal(nx,ny,nz)
	{} 
	
	DirectX::XMFLOAT3 pos = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 textcoord= { 0.0f,1.0f,};
	//DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
};