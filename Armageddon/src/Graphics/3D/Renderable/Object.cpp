#include "Object.h"




void Object::InitVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_contex, Vertex v[],UINT VertexSize)
{
	HRESULT hr = VertexBuffer.Initialize(device, device_contex, v, VertexSize);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("FAILED INITILIZE VERTEXBUFFER : {0}%x",hr);
	}


}

void Object::InitIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_contex, DWORD indices[],UINT IndicesSize)
{

	HRESULT hr = this->IndexBuffer.Init(device, indices, IndicesSize);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("FAILED INITIALIZE INDEX BUFFER  ");

	}
}

void Object::InitShaders(Microsoft::WRL::ComPtr <ID3D11Device>& device,std::wstring ShaderFloder, std::wstring VertexShader, std::wstring PixelShader, D3D11_INPUT_ELEMENT_DESC layout[],UINT SizeOfLayout)
{
	if (!vertexShader.Init(device, ShaderFloder + L"VertexShader.cso", layout, SizeOfLayout))
	{
		Armageddon::Log::GetLogger()->error("FAILED INITIALIZE VERTEX SHADERS ");
	}

	if (!pixelShader.Init(device, ShaderFloder + L"PixelShader.cso"))
	{
		Armageddon::Log::GetLogger()->error("FAILED INITIALIZE PIXEL SHADERS ");
	}
}

void Object::InitConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* device_contex )
{
	HRESULT hr = ConstantBuffer.init(device, device_contex);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING CONSTANT BUFFER [{0}]", hr);
	}
}

void Object::BindVertexBuffer(ID3D11DeviceContext* device_contex)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	device_contex->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
}

void Object::BindIndexBuffer(ID3D11DeviceContext* device_contex)
{
	device_contex->IASetIndexBuffer(this->IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

}

void Object::BindShader(ID3D11DeviceContext* device_contex)
{
	device_contex->VSSetShader(this->vertexShader.GetShader(), NULL, 0);
	device_contex->PSSetShader(this->pixelShader.GetShader(), NULL, 0);
	
}

void Object::BindConstantBuffer(ID3D11DeviceContext* device_contex)
{
	
	if (!this->ConstantBuffer.ApplyChanges())
	{
	}

	device_contex->VSSetConstantBuffers(0, 1, this->ConstantBuffer.GetAdressOf());
	device_contex->PSSetConstantBuffers(0, 1, this->ConstantBuffer.GetAdressOf());
}

void Object::BindInputLayout(ID3D11DeviceContext* device_contex)
{
	device_contex->IASetInputLayout(this->vertexShader.GetInputLayout());

}

void Object::Draw(ID3D11DeviceContext* device_contex)
{
		BindShader(device_contex);
		BindConstantBuffer(device_contex);
		BindInputLayout(device_contex);
		BindVertexBuffer(device_contex);
		BindIndexBuffer(device_contex);
		DrawIndex(device_contex);
	
}

/*void Object::Draw(ID3D11DeviceContext* device_contex) 
{
	
		BindShader(device_contex);
		BindConstantBuffer(device_contex);
		BindInputLayout(device_contex);
		BindVertexBuffer(device_contex);
		BindIndexBuffer(device_contex);
		DrawIndex(device_contex);
	
	
}*/

void Object::DrawIndex(ID3D11DeviceContext* device_contex)
{
	device_contex->DrawIndexed(this->IndexBuffer.GetSize(), 0, 0);

}
float a;
void Object::UpDateConstantBuffer(Camera* cam)
{

	DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();
	f += 0.01;
	a += 0.01;
	//WorldMat += DirectX::XMMatrixRotationX(sin(f));
	
	

	//Armageddon::Log::GetLogger()->trace(a);
	if (a >= 1)
	{
		a = 0;
	}
	//this->ConstantBuffer.data.LightDir = { 0.0f,0.0f,f };
	this->ConstantBuffer.data.color = { 1.0f };
	this->ConstantBuffer.data.mat = (WorldMat * TransformMat) * cam->GetViewMatrix() * cam->GetProjectionMatrix() ;
	this->ConstantBuffer.data.Yoffset = 0;
	this->ConstantBuffer.data.Xoffset = 0;
}
void Object::UpDateConstantBuffer(Camera* cam, DirectX::XMMATRIX Transform)
{
	DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();

	f += 0.01;
	Armageddon::Log::GetLogger()->trace("F = {0}",f);
	WorldMat = DirectX::XMMatrixRotationY(f);


	this->ConstantBuffer.data.color = { 1.0f };
	this->ConstantBuffer.data.mat = WorldMat * cam->GetViewMatrix() * cam->GetProjectionMatrix() * Transform;
	this->ConstantBuffer.data.Yoffset = 0;
	this->ConstantBuffer.data.Xoffset = 0;
}