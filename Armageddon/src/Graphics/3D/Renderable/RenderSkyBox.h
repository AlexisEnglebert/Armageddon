#pragma once
#include "Object.h"
#include "../../../Includes.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>


class DLL RenderSkyBox : public Object
{
public:
	RenderSkyBox(const wchar_t* SkyBoxTexture, const wchar_t* IrSkyBoxTexture,Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context) : device(device), device_context(device_context)
	{
        CreateSphere(100, 100);
		//HRESULT hr = DirectX::CreateWICTextureFromFileEx(device.Get(), SkyBoxTexture ,Texresource.Get(), ShaderTextRessource.GetAddressOf());
		//HRESULT hr = DirectX::CreateDDSTextureFromFile(device.Get(), SkyBoxTexture, Texresource.GetAddressOf(), ShaderTextRessource.GetAddressOf());
		//LoadTexture(SkyBoxTexture);

		//HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), L"..\\TestApplication\\assets\\Textures\\checker_board.png", Texresource.GetAddressOf(), ShaderTextRessource.GetAddressOf());
		HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
			device.Get(),
			SkyBoxTexture,
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			D3D11_RESOURCE_MISC_TEXTURECUBE,  
			false,
			Texresource.GetAddressOf(),
			ShaderTextRessource.GetAddressOf(),
			nullptr);

		hr = DirectX::CreateDDSTextureFromFileEx(
			device.Get(),
			IrSkyBoxTexture,
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			D3D11_RESOURCE_MISC_TEXTURECUBE,
			false,
			IrTexresource.GetAddressOf(),
			IrShaderTextRessource.GetAddressOf(),
			nullptr);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
		{"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

#if  _DEBUG
		InitShaders(device, L"..\\bin\\Debug-x64\\Armageddon\\", L"VSSKyBox.cso", L"PSSKyBox.cso", layout, ARRAYSIZE(layout));
#else
		Armageddon::Log::GetLogger()->info("oof");
		InitShaders(device, L"..\\bin\\Debug-x64\\Armageddon\\", L"VSSKyBox.cso", L"PSSKyBox.cso", layout, ARRAYSIZE(layout));

		//InitShaders(device, L"", L"VSSKyBox.cso", L"PSSKyBox.cso", layout, ARRAYSIZE(layout));

#endif //  _DEBUG

		
		InitConstantBuffer(device.Get(), device_context.Get());

		D3D11_SAMPLER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));
		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sDesc.MinLOD = 0;
		sDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = device->CreateSamplerState(&sDesc, SamplerState.GetAddressOf());

		if (FAILED(result))
		{
			Armageddon::Log::GetLogger()->error("FAILED CREATING THE COLOR SAMPLER STATE");
		}
		
		D3D11_RASTERIZER_DESC rDesc;
		ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
		rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		 result = device->CreateRasterizerState(&rDesc, rasterizerState.GetAddressOf());
	}
	

	void CreateSphere(float LatLines, float LongLines)
	{

		float radius = 10.0f;
		Armageddon::Log::GetLogger()->info("CREATE SPHERE");
		float NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
		float NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);
		float sphereYaw = 0.0f;
		float spherePitch = 0.0f;



		
		DirectX::XMVECTOR currVertPos = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		v.push_back({ 0.0f,0.0f,1.0f });
	
		for (DWORD i = 0; i < LatLines - 2; ++i)
		{
			spherePitch = (i + 1) * (3.14 / (LatLines - 1));
			DirectX::XMMATRIX Rotationx = DirectX::XMMatrixRotationX(spherePitch);
			for (DWORD j = 0; j < LongLines; ++j)
			{
				sphereYaw = j * (6.28 / (LongLines));
				DirectX::XMMATRIX Rotationy = DirectX::XMMatrixRotationZ(sphereYaw);
				DirectX::XMVECTOR currVertPos = DirectX::XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
				currVertPos = DirectX::XMVector3Normalize(currVertPos);

				v.push_back({ DirectX::XMVectorGetX(currVertPos),DirectX::XMVectorGetY(currVertPos),DirectX::XMVectorGetZ(currVertPos) });

			
			}
		}
		v.push_back({ 0.0f,0.0f,-1.0f });




		InitVertexBuffer(device.Get(), device_context.Get(), v.data(), v.size());

        int k = 0;
        for (DWORD l = 0; l < LongLines - 1; ++l)
        {
            indices.push_back(0);
			indices.push_back(l + 1);
			indices.push_back(l + 2);
            k += 3;
        }

		indices.push_back(0);
		indices.push_back(LongLines);
		indices.push_back(1);
        k += 3;

        for (DWORD i = 0; i < LatLines - 3; ++i)
        {
            for (DWORD j = 0; j < LongLines - 1; ++j)
            {
				indices.push_back(i * LongLines + j + 1);
				indices.push_back (i * LongLines + j + 2);
				indices.push_back((i + 1) * LongLines + j + 1);

				indices.push_back( (i + 1) * LongLines + j + 1);
				indices.push_back(i * LongLines + j + 2);
				indices.push_back((i + 1) * LongLines + j + 2);

                k += 6; // next quad
            }

			indices.push_back((i * LongLines) + LongLines);
			indices.push_back((i * LongLines) + 1);
			indices.push_back(((i + 1) * LongLines) + LongLines);

			indices.push_back(((i + 1) * LongLines) + LongLines);
			indices.push_back((i * LongLines) + 1);
			indices.push_back(((i + 1) * LongLines) + 1);

            k += 6;
        }

        for (DWORD l = 0; l < LongLines - 1; ++l)
        {
			indices.push_back(NumSphereVertices - 1);
			indices.push_back((NumSphereVertices - 1) - (l + 1));
			indices.push_back((NumSphereVertices - 1) - (l + 2));
            k += 3;
        }

		indices.push_back(NumSphereVertices - 1);
		indices.push_back((NumSphereVertices - 1) - LongLines);
		indices.push_back(NumSphereVertices - 2);

	

        InitIndexBuffer(device.Get(), device_context.Get(), indices.data(), indices.size());

	}

	


	virtual void Draw(ID3D11DeviceContext* device_contex)
	{
		device_context->PSSetSamplers(1, 1, SamplerState.GetAddressOf());
		device_context->PSSetShaderResources(1, 1, ShaderTextRessource.GetAddressOf());
		BindShader(device_contex);
		BindConstantBuffer(device_contex);
		BindInputLayout(device_contex);
		BindVertexBuffer(device_contex);
		BindIndexBuffer(device_contex);
		//Armageddon::Log::GetLogger()->trace("oofdf");
		//device_contex->RSSetState(rasterizerState.Get());
		DrawIndex(device_contex);
	};

	virtual void UpDateConstantBuffer(Camera* cam)
	{
		DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(999.0f, 999.0f, 999.0f);	
		//DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(cam->GetPos().x, cam->GetPos().y, cam->GetPos().z);
	

		//ConstantBuffer.data.LightDir = { 0,0,f };
		ConstantBuffer.data.CameraPos = { cam->GetPos().x,cam->GetPos().y,cam->GetPos().z };

		//ConstantBuffer.data.mat = WorldMat  * scale  * DirectX::XMMatrixTranslation(cam->GetPos().x, cam->GetPos().y, cam->GetPos().z)* cam->GetViewMatrix() * cam->GetProjectionMatrix();
		ConstantBuffer.data.WorldMat = WorldMat * scale * DirectX::XMMatrixTranslation(cam->GetPos().x, cam->GetPos().y, cam->GetPos().z);
		ConstantBuffer.data.ProjectionMat = cam->GetProjectionMatrix();
		ConstantBuffer.data.ViewMat = cam->GetViewMatrix();
		ConstantBuffer.data.MVP = cam->GetViewMatrix() * cam->GetProjectionMatrix();
		ConstantBuffer.data.Yoffset = 0;
		ConstantBuffer.data.Xoffset = 0;
	}
	/*virtual void UpDateConstantBuffer(Camera* cam)
	{
		DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();

		f += 0.1f;
		Armageddon::Log::GetLogger()->trace("of");
		WorldMat = DirectX::XMMatrixRotationY(f);

		ConstantBuffer.data.LightDir = { 0,0,f };
		ConstantBuffer.data.color = { 1.0f };
		ConstantBuffer.data.mat = WorldMat * cam->GetViewMatrix() * cam->GetProjectionMatrix() * this->TransformMat;
		ConstantBuffer.data.Yoffset = 0;
		ConstantBuffer.data.Xoffset = 0;
	}*/
	~RenderSkyBox()
	{
		//rasterizerState.Reset();
	//	ShaderTextRessource.Reset();
	//	device.Reset();
	//	device_context.Reset();
	}
	Microsoft::WRL::ComPtr<ID3D11Resource> Texresource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderTextRessource;

	Microsoft::WRL::ComPtr<ID3D11Resource> IrTexresource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> IrShaderTextRessource;
private :
	//std::vector<Vertex> v;
	std::vector<Vertex> v;

	std::vector<DWORD> indices;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

	

	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;

	Microsoft::WRL::ComPtr <ID3D11Device> device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context;
};