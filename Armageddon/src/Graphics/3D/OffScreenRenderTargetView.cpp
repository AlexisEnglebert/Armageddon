#include "OffScreenRenderTargetView.h"

bool OfsRenderTargetView::Init(ID3D11Device* device, IDXGISwapChain* swapchain, float width, float height)
{
   
    CreateScreenTexture(device, width, height);
    CreateShaderResourceView(device);
    CreateRenderTargetView(device,swapchain);
    //output 
  


    return true;
}
void OfsRenderTargetView::CreateShaderResourceView(ID3D11Device* device)
{
    //Input Texture -> ressource view on the Texture
    D3D11_SHADER_RESOURCE_VIEW_DESC sRenderTargetDesc;
    sRenderTargetDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sRenderTargetDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    sRenderTargetDesc.Texture2D.MipLevels = 1;
    sRenderTargetDesc.Texture2D.MostDetailedMip = 0;
    HRESULT hr = device->CreateShaderResourceView(ScreenTex.Get(), &sRenderTargetDesc, ShaderRessourceView.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING SHADER RESSOURCE VIEX  {0}", hr);
    }
}
void OfsRenderTargetView::ResizeRenderTargetView(ID3D11Device* device,IDXGISwapChain* swapchain,float width, float height, ID3D11Debug* debug )
{
    if (width > 1 && height > 1 && device != NULL)
    {

        CleanRenderTargetView();

        if (debug != nullptr)
        {
            debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        }
        swapchain->ResizeTarget(0);
        CreateScreenTexture(device, width, height);
        CreateShaderResourceView(device);
        CreateRenderTargetView(device, swapchain);
    }
}



void OfsRenderTargetView::CleanRenderTargetView()
{
    if (pTargetView)
    {
        pTargetView.Reset();
        pTargetView = (std::nullptr_t) NULL;
    }
    if (ScreenTex)
    {
        ScreenTex.Reset();
        ScreenTex = (std::nullptr_t) NULL;
    }
    if (ShaderRessourceView)
    {
        ShaderRessourceView.Reset();
        ShaderRessourceView = (std::nullptr_t) NULL;
    }

}

void OfsRenderTargetView::CreateRenderTargetView(ID3D11Device* device , IDXGISwapChain* swapchain)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    renderTargetViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;


    HRESULT hr = device->CreateRenderTargetView(ScreenTex.Get(), &renderTargetViewDesc, pTargetView.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING RENDER TARGETVIEW FOR SCENE TEX {0}", hr);
    }
}

void OfsRenderTargetView::CreateScreenTexture(ID3D11Device* device , float width, float height)
{
    //crée la texture
    D3D11_TEXTURE2D_DESC SceneDesc;
    SceneDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SceneDesc.Width = width;
    SceneDesc.Height = height;
    SceneDesc.ArraySize = 1;
    SceneDesc.MipLevels = 1;
    SceneDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    SceneDesc.Usage = D3D11_USAGE_DEFAULT;
    SceneDesc.CPUAccessFlags = 0;
    SceneDesc.MiscFlags = 0;
    SceneDesc.SampleDesc.Count = 1;
    SceneDesc.SampleDesc.Quality = 0;

    HRESULT hr = device->CreateTexture2D(&SceneDesc, NULL, ScreenTex.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING TEXTURE2D FOR SCENE TEX {0}", hr);
    }
    this->ImageX = width;
    this->ImageY = height;
}



void OfsRenderTargetView::Bind(ID3D11DeviceContext* device_context)
{
    device_context->OMSetRenderTargets(1, pTargetView.GetAddressOf(), nullptr);

}
void OfsRenderTargetView::Bind(ID3D11DeviceContext* device_context, ID3D11DepthStencilView* dephStencil)
{
    device_context->OMSetRenderTargets(1, pTargetView.GetAddressOf(),dephStencil);

}

void OfsRenderTargetView::Clear(ID3D11DeviceContext* device_context)
{
    float color[] = { 0.1f,0.1f,0.1f,1.0f };

   device_context->ClearRenderTargetView(pTargetView.Get(), color);

}

void OfsRenderTargetView::BindBackBuffer(ID3D11DeviceContext* device_context, ID3D11RenderTargetView* render)
{
    device_context->OMSetRenderTargets(1, pTargetView.GetAddressOf(), nullptr);

}

void OfsRenderTargetView::BindBackBuffer(ID3D11DeviceContext* device_context, ID3D11RenderTargetView** renderTarget, ID3D11DepthStencilView* dephStencil)
{
    device_context->OMSetRenderTargets(1, renderTarget, dephStencil);

}
