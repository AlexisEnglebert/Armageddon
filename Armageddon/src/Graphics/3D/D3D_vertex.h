#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z)
		: pos(x, y, z)
	{}
	
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), textcoord(u, v)
	{}
	Vertex(float x, float y, float z,  float u, float v, float nx, float ny, float nz) 
		: pos(x, y, z), textcoord(u, v) ,normal(nx,ny,nz)
	{} 
	/*Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz,float r , float g, float b)
		: pos(x, y, z), textcoord(u, v), normal(nx, ny, nz),color(r,g,b)
	{}*/
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz)
		: pos(x, y, z), textcoord(u, v), normal(nx, ny, nz), Tangent(tx,ty,tz)
	{}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz,float btx,float bty,float btz)
		: pos(x, y, z), textcoord(u, v), normal(nx, ny, nz), Tangent(tx, ty, tz) , Bitangent(btx,bty,btz)
	{}
	
	DirectX::XMFLOAT3 pos = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT2 textcoord= { 0.0f,1.0f,};
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT3 Bitangent;

	//DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
};