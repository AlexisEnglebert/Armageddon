#include "Mesh.h"
#include "../../../Includes.h"
#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../D3D_vertex.h"
#include "Object.h"


Mesh::Mesh(Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, std::vector<Vertex>VertexData, std::vector<DWORD> indices)
{
	this->deviceContext = deviceContext;
	VertexBuffer.Initialize(device.Get(), deviceContext.Get(), VertexData.data(), VertexData.size());
	IndexBuffer.Init(device.Get(), indices.data(), indices.size());
}

void Mesh::Draw()
{
		 UINT stride = sizeof(Vertex);
		 UINT offset = 0;
		 deviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
		 deviceContext->IASetIndexBuffer(this->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		 deviceContext->DrawIndexed(this->IndexBuffer.GetSize(), 0, 0);


}







