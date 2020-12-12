#include "Material.h"

void Material::SetNormalMap(const wchar_t* NormalPath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(device .Get(), NormalPath, NormalResource.ReleaseAndGetAddressOf(), NormalShaderRessource.ReleaseAndGetAddressOf());
	//std::string Albedo(strlen(NormalPath), L'#');
	//mbstowcs(&Albedo[0], AlbedoMap.c_str(), strlen(AlbedoMap.c_str()));

	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING Normal TEXTURE [{0}]",hr);
	}
}

void Material::SetColorMap(const wchar_t* ColorPath)
{
	Armageddon::Log::GetLogger()->info("COKKKKIR");
	Armageddon::Log::GetLogger()->trace("Material Set color map path {0}", (void*)ColorPath);
	//this->ColorShaderRessource.Reset();
	//this->ColorResource.Reset();
	HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), ColorPath, this->ColorResource.ReleaseAndGetAddressOf(), this->ColorShaderRessource.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING Color TEXTURE [{0}]", hr);
	}
}
void Material::BindRessources()
{
	device_context->PSSetSamplers(0, 1, SamplerState.GetAddressOf());
	device_context->PSSetShaderResources(1, 1, SkyBoxTextRessource.GetAddressOf());
	//device_context->PSSetShaderResources(2, 1, IrSkyBoxTextRessource.GetAddressOf());
	device_context->PSSetShaderResources(3, 1, ColorShaderRessource.GetAddressOf());
	device_context->PSSetShaderResources(4, 1, NormalShaderRessource.GetAddressOf());
	device_context->PSSetShaderResources(5, 1, SpecularShaderRessource.GetAddressOf());
	device_context->PSSetShaderResources(6, 1, OcclusionShaderRessource.GetAddressOf());
	device_context->PSSetShaderResources(7, 1, BRDFLookupShaderRessource.GetAddressOf());
};
void Material::SetSpecMap(const wchar_t* SpecPath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), SpecPath, SpecularResource.ReleaseAndGetAddressOf(), SpecularShaderRessource.ReleaseAndGetAddressOf());
	
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING NORMAL TEXTURE [{0}]", hr);
	}
}

void Material::SetOcclusionMap(const wchar_t* OcclusionPath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), OcclusionPath, OcclusionResource.ReleaseAndGetAddressOf(), OcclusionShaderRessource.ReleaseAndGetAddressOf());

	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING NORMAL TEXTURE [{0}]", hr);
	}
}
