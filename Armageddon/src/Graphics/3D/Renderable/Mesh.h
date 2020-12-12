#pragma once
#include "../../../Includes.h"
#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../D3D_vertex.h"
#include <d3d11.h>
#include "Object.h"
#include "../Material.h"
class DLL Mesh
{
public:
	Mesh(Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, std::vector<Vertex> VertexData, std::vector<DWORD> indices);
	Mesh(Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, std::vector<Vertex> VertexData, std::vector<DWORD> indices,
		const char* MaterialName,
		std::string AlbedoMap,
		std::string NormalMap,
		std::string SpecularMap,
		std::string OcclusionMap);

	//Mesh(const Mesh&);
	~Mesh()
	{
	
	};
	void Draw();
	Material mat;
	
private:

	VertexBuffer<Vertex> VertexBuffer;
	IndexBuffer IndexBuffer;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext;

};