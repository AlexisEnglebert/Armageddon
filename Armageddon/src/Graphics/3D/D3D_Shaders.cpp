#include "D3D_Shaders.h"
#include "../../Log.h"
#include <dxgitype.h>
bool VertexShader::Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath , D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements)
{
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(),this->shader_buffer.GetAddressOf());
    if (FAILED(hr))
    {
      
        Armageddon::Log::GetLogger()->error("FAILED INIT VERTEX SHADER  [{0}]",hr);
        return false;

    }

    hr = device->CreateVertexShader(this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED  CREATING VERTEX SHADER  [{0}]", hr);
        return false;

    }

    hr = device->CreateInputLayout(desc, numElements, this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), this->inputlayout.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED  CREATING INPUT LAYOUT : {0}",hr);

    }
    Armageddon::Log::GetLogger()->trace("FINISHED");

    return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
    //Armageddon::Log::GetLogger()->info("GETTING VERTEX SHADER");
    return  this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
   // Armageddon::Log::GetLogger()->info("GETTING VERTEX BUFFER");

    return this->shader_buffer.Get();
}

inline ID3D11InputLayout* VertexShader::GetInputLayout()
{
   // Armageddon::Log::GetLogger()->info("GETTING InputLayout");
    return this->inputlayout.Get();
}

bool PixelShader::Init(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath)
{
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->shader_buffer.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED PIXEL SHADER");
        return false;
    }
    hr = device->CreatePixelShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING PIXEL SHADER");
        return false;
    }
    return true;
}

inline ID3D11PixelShader* PixelShader::GetShader()
{
   // Armageddon::Log::GetLogger()->info("GETTING PIXEL SHADER");
    return this->shader.Get();
}

inline ID3D10Blob* PixelShader::GetBuffer()
{
   // Armageddon::Log::GetLogger()->info("GETTING PIXEL BUFFER");
    return this->shader_buffer.Get();
}
