#pragma once
#include "../../../Includes.h"
#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../D3D_Shaders.h"
#include "../ConstantBufferTypes.h"
#include "../ConstantBuffer.h"
#include "../D3D_vertex.h"
#include <wrl/client.h>
#include "../Camera.h"
#include <d3d11.h>
#include <stdlib.h>
#include "Mesh.h"

#pragma comment(lib,"d3d11.lib")

class DLL Object
{
public:



	Object()
	{
		Armageddon::Log::GetLogger()->trace("INSTANCIATE A NEW OBJECT");

	};

	virtual void InitVertexBuffer(ID3D11Device* device , ID3D11DeviceContext* device_contex ,Vertex v[], UINT VertexSize);
	virtual void InitIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_contex, DWORD indices[], UINT IndicesSize);
	virtual void InitShaders(Microsoft::WRL::ComPtr <ID3D11Device>& device,std::wstring ShaderFloder, std::wstring VertexShader, std::wstring PixelShader, D3D11_INPUT_ELEMENT_DESC layout[], UINT SizeOfLayout);
	virtual void InitConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* device_contex);


	virtual  void BindVertexBuffer(ID3D11DeviceContext* device_contex);
	 virtual  void BindIndexBuffer(ID3D11DeviceContext* device_contex);
	 virtual void BindShader(ID3D11DeviceContext* device_contex);
	 virtual  void BindConstantBuffer(ID3D11DeviceContext* device_contex);
	 virtual  void BindInputLayout(ID3D11DeviceContext* device_contex);
	// virtual void Draw(ID3D11DeviceContext* device_contex) = 0;
	 virtual void Draw(ID3D11DeviceContext* device_contex);
	 
	 virtual  void DrawIndex(ID3D11DeviceContext* device_contex);
	 virtual void UpDateConstantBuffer(Camera* cam);
	 void UpDateConstantBuffer(Camera* cam, DirectX::XMMATRIX Transform);
	 virtual void SetName(const char* n) { this->name = n; };
	  const char* GetName() { return name; };
	 virtual Object* GetInstance()
	 {
		 return this;
	 };
	 ConstantBuffer<CB_VS_SHADER> ConstantBuffer;
	 XMMATRIX TransformMat = XMMatrixIdentity();
	 void SetTransformMatrix(XMMATRIX mat)
	 {
		 TransformMat = mat;
	 }
	bool CustomDraw = false;
	int c = float(rand()) / float((RAND_MAX)) * 5.0f;
	int b = float(rand()) / float((RAND_MAX)) * 5.0f;
	float f = 0;
	~Object()
	{
		
	}

private: 

	VertexBuffer<Vertex> VertexBuffer;
	IndexBuffer IndexBuffer;

	VertexShader vertexShader;
	PixelShader pixelShader;

	// Object ID ? 
	const char*  name = "default name";
};

