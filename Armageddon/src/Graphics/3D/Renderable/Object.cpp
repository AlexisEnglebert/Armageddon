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
	if (!vertexShader.Init(device, ShaderFloder + VertexShader, layout, SizeOfLayout))
	{
		Armageddon::Log::GetLogger()->error("FAILED INITIALIZE VERTEX SHADERS ");
	}

	if (!pixelShader.Init(device, ShaderFloder + PixelShader))
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
	hr = lightBuffer.init(device, device_contex);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING Light Buffer [{0}]", hr);

	}
}

void Object::InitRasterizer(ID3D11Device* device)
{
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
	if (!lightBuffer.ApplyChanges())
	{

	}
	device_contex->VSSetConstantBuffers(1, 1, this->lightBuffer.GetAdressOf());
	device_contex->PSSetConstantBuffers(1, 1, this->lightBuffer.GetAdressOf());
}

void Object::BindInputLayout(ID3D11DeviceContext* device_contex)
{
	device_contex->IASetInputLayout(this->vertexShader.GetInputLayout());

}

void Object::BindRasterize(ID3D11Device* device)
{
}

void Object::Draw(ID3D11DeviceContext* device_contex)
{
	Armageddon::Log::GetLogger()->info("DRAW OBJECT");
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

	
	f += 0.1;
	a += 0.01;
	//WorldMat += DirectX::XMMatrixRotationX(sin(f));
	
	

	//Armageddon::Log::GetLogger()->trace(a);
	if (a >= 1)
	{
		a = 0;
	}

	//this->ConstantBuffer.data.LightDir = { 0.0f,0.0f,DirectX::XMConvertToRadians(f) };
	ConstantBuffer.data.WorldMat = (WorldMat * this->TransformMat);
	ConstantBuffer.data.ProjectionMat = cam->GetProjectionMatrix();
	ConstantBuffer.data.ViewMat = cam->GetViewMatrix();
	ConstantBuffer.data.MVP = cam->GetViewMatrix() * cam->GetProjectionMatrix();
	ConstantBuffer.data.Yoffset = 0;
	ConstantBuffer.data.Xoffset = 0; 
	ConstantBuffer.data.LightDir = LightDirection;
	ConstantBuffer.data.CameraPos = { cam->GetPos().x,cam->GetPos().y,cam->GetPos().z };
	//ConstantBuffer.data.EyePos = cam->getpo
	ConstantBuffer.data.RoughNess = Roughness;
	ConstantBuffer.data.MetalNess = metalNess;
	//this->lightBuffer.data.LightIntensity = 1.0f;



	lightBuffer.data.LightIntensity = 10;
}
