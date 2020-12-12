#pragma once
#include "../../Includes.h"
#include <assimp/material.h>
#include <wrl\client.h>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#pragma comment(lib,"d3d11.lib")


class DLL Material
{
public:
	Material(){}
	Material(Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context) : device(device)
		, device_context(device_context){
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
		Armageddon::Log::GetLogger()->info("[information] Le sampler state a bien été créé");


		HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), L"..\\TestApplication\\assets\\Textures\\brdf_lut.jpg", this->BRDFLookupTex.ReleaseAndGetAddressOf(), this->BRDFLookupShaderRessource.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING BRDF TEXTURE [{0}]", hr);
		}
		 hr = DirectX::CreateDDSTextureFromFileEx(
			device.Get(),
			 L"..\\TestApplication\\assets\\Textures\\skybox\\skybox1_espace.dds",
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			D3D11_RESOURCE_MISC_TEXTURECUBE,
			false,
			 skyTexresource.GetAddressOf(),
			 SkyBoxTextRessource.GetAddressOf(),
			nullptr);
		 if (FAILED(hr))
		 {
			 Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING SKYBOX TEXTURE [{0}]", hr);

		 }

		//SetColorMap(L"..\\TestApplication\\assets\\Textures\\checker_texture.png");
		SetColorMap(L"..\\TestApplication\\assets\\Textures\\streaked-metal1-albedo.png");
		SetNormalMap(L"..\\TestApplication\\assets\\Textures\\streaked-metal1-normal-ogl.png");
		SetSpecMap(L"..\\TestApplication\\assets\\Textures\\streaked-metal1-rough.png");
		SetOcclusionMap(L"..\\TestApplication\\assets\\Textures\\streaked-metal1-metalness.png");
	}
	
	//Material()
	void SetNormalMap(const wchar_t* NormalPath);
	void SetColorMap(const wchar_t* ColorPath);
	void SetSpecMap(const wchar_t* SpecPath);
	void SetOcclusionMap(const wchar_t* OcclusionPath);
	void setMaterialName(const char* MaterialName)
	{
		this->MaterialName = MaterialName;
	}
	ID3D11ShaderResourceView* GetColorMap()
	{
		return ColorShaderRessource.Get();
	}
	ID3D11ShaderResourceView* GetNormalMap()
	{
		return NormalShaderRessource.Get();
	}
	ID3D11ShaderResourceView* GetSpecularMap()
	{
		return SpecularShaderRessource.Get();
	}
	ID3D11ShaderResourceView* GetAOMap()
	{
		return OcclusionShaderRessource.Get();
	}
	
	void BindRessources();
	/*SkyBox*/
	Microsoft::WRL::ComPtr<ID3D11Resource> skyTexresource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SkyBoxTextRessource;
	/*Irradiance skybox*/
	Microsoft::WRL::ComPtr<ID3D11Resource> IrTexresource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> IrSkyBoxTextRessource;
	const char* MaterialName;
	char test[255];
	std::string Albedo = "";
	std::string Normal = "";
	std::string Spec = "";
	std::string Occlusion = "";
private:

	Microsoft::WRL::ComPtr <ID3D11Device> device;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;

	

	/*Albedo Map*/
	Microsoft::WRL::ComPtr<ID3D11Resource> ColorResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ColorShaderRessource;
	/*Normal Map*/
	Microsoft::WRL::ComPtr<ID3D11Resource> NormalResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> NormalShaderRessource;
	/*Specular Map*/
	Microsoft::WRL::ComPtr<ID3D11Resource> SpecularResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SpecularShaderRessource;
	/*Occlusion Map*/
	Microsoft::WRL::ComPtr<ID3D11Resource> OcclusionResource;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> OcclusionShaderRessource;

	Microsoft::WRL::ComPtr<ID3D11Resource> BRDFLookupTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> BRDFLookupShaderRessource;
	

	//Specular
	//ColorMap -> Albedo
	//Metal Map 
	//GlossMap
	//NormalMap
};