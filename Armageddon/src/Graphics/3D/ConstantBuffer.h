#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "../../Log.h"
template<class T>
class ConstantBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext = nullptr;
public:
	T data;
	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAdressOf() const
	{
		return buffer.GetAddressOf();
	}

	HRESULT init(ID3D11Device* device, ID3D11DeviceContext* device_context)
	{
		this->deviceContext = device_context;
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		cbDesc.StructureByteStride = 0;
		HRESULT hr = device->CreateBuffer(&cbDesc, 0, buffer.GetAddressOf());
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("FAILED CREATINF CONSTANT BUFFER");
		}
		return hr;
	}

	bool ApplyChanges()
	{

		D3D11_MAPPED_SUBRESOURCE mRessource;
		HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mRessource);
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("ERROR MAPPING CONSTANT BUFFER  : [{0}]", hr);
			return false;
		}
		CopyMemory(mRessource.pData, &data, sizeof(T)); // copie les data du constantBufffer
		this->deviceContext->Unmap(buffer.Get(), 0);
	//	this->device_context->VSSetConstantBuffers(0, 1, buffer.GetAddressOf());
		return true;
	}

};