#include "D3D_graphics.h"
#include <vector>
#include "..\AdaptaterData.h"
#include "..\..\Log.h"
#include <wrl/client.h>
#include <WICTextureLoader.h>
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "Camera.h"
#include "../../Application.h"
#include <PrimitiveBatch.h>
#include "ScreenGrab.h"
bool Armageddon::D3D_graphics::Init(HWND hwnd, int height, int width)
{
    if (!InitDirectX(hwnd, height, width))
    {
        return false;
    }
    Armageddon::Application::GetInstance()->ImGuiInit();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(this->device.Get(), this->device_context.Get());


   // Armageddon::Application::GetInstance()->Init();
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED COINIT {0}", hr);
    }
   

    handlewnd = &hwnd;

    return true;
}

static float f = 0;



void Armageddon::D3D_graphics::RenderFrame()
{
    float color[] = { 0.1f,0.1f,0.1f,1.0f };
   
   
    r.Bind(this->device_context.Get(), this->depthStencilView.Get());
    r.Clear(this->device_context.Get());
    this->device_context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1);
    //ICI QU'ON FAIT TOUT LES RENDU APRES AVOIR CLEAN LE PLAN

   Armageddon::Application::GetInstance()->ImGuiBegin();


   /* DRAWING STUFF*/
    Armageddon::Application::GetInstance()->ImGuiRender();
    this->device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

 

    /***********DEPHT BUFFER*******************************/

   this->device_context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);

    /***********RASTERIZER STATE*******************************/

    this->device_context->RSSetState(this->rasterizerState.Get());


    

  Armageddon::Application::GetInstance()->OnRender();

     ImGui::Render();


    r.BindBackBuffer(this->device_context.Get(), this->target_view.GetAddressOf(), this->depthStencilView.Get()); // on remet le buffer normal
    this->device_context->ClearRenderTargetView(this->target_view.Get(), color);
    this->device_context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    this->device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


   ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
   if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
   {
       ImGui::UpdatePlatformWindows();
       ImGui::RenderPlatformWindowsDefault();
   }
    this->swapchain->Present(1,0);

    OldDeltaTime = CurrentTime;
    CurrentTime = GetTickCount();
    DeltaTime = OldDeltaTime ? ((CurrentTime - OldDeltaTime) / 1000.0f) : 0;
    //Armageddon::Log::GetLogger()->trace("Delta time : {0} , FPS {1}", DeltaTime, 1.0f/DeltaTime);


}

void Armageddon::D3D_graphics::CreateRasterizer(D3D11_RASTERIZER_DESC rDesc)
{
  

   HRESULT result = this->device->CreateRasterizerState(&rDesc, this->rasterizerState.GetAddressOf());
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING THE RASTERIZER STATE");
    }
}

void Armageddon::D3D_graphics::ChangeRasterizer(D3D11_RASTERIZER_DESC rDesc)
{
    this->rasterizerState.Reset();
    CreateRasterizer(rDesc);
}

void Armageddon::D3D_graphics::SetWireFrame(bool wireframe)
{
    if (wireframe != WireFrame)
    {
        if (wireframe)
        {
            Armageddon::Log::GetLogger()->trace("SET THE WIRE FRAME");
            D3D11_RASTERIZER_DESC rDesc;
            ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
            rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
            rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
            ChangeRasterizer(rDesc);
        }
        else
        {
            Armageddon::Log::GetLogger()->trace("DISABLE THE WIRE FRAME");

            D3D11_RASTERIZER_DESC rDesc;
            ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
            rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
            rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
            ChangeRasterizer(rDesc);
        }
        
        WireFrame = wireframe;
    }
}


bool Armageddon::D3D_graphics::InitDirectX(HWND hwnd, float height, float width)
{
    //HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,)
    this->windowHeight = height;
    this->windowWidth = width;
    std::vector<AdaptaterData> adapter = AdaptaterReader::GetAdaptaterData();
    if (adapter.size() < 1)
    {
        Armageddon::Log::GetLogger()->error("NO ADAPTER FOUND");
        return false;
    }
    
    DXGI_SWAP_CHAIN_DESC swapCD;
    ZeroMemory(&swapCD, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapCD.BufferDesc.Width = 0;
    swapCD.BufferDesc.Height = 0;
    swapCD.BufferDesc.RefreshRate.Numerator = 60;
    swapCD.BufferDesc.RefreshRate.Denominator = 1;
    swapCD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapCD.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapCD.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swapCD.SampleDesc.Count = 1;
    swapCD.SampleDesc.Quality = 0;

    swapCD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    swapCD.BufferCount = 2;

    swapCD.OutputWindow = hwnd;
    swapCD.Windowed = TRUE;

    swapCD.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    swapCD.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT result = D3D11CreateDeviceAndSwapChain(adapter[0].ptrAdaptater,
        D3D_DRIVER_TYPE_UNKNOWN,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG,
        featureLevelArray,
        2,
        D3D11_SDK_VERSION,
        &swapCD,
        this->swapchain.GetAddressOf(),
        this->device.GetAddressOf(),
        &featureLevel,
        this->device_context.GetAddressOf());
   /* this->d3dDebug->AddRef(device);
    this->d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);*/
   // d3dDebug->QueryInterface();
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING SWAPCHAIN");
        return false;
    }
    CreateDephtStencilBuffer(width, height);

    CreateRenderTargetView();
    r.Init(this->device.Get(),this->swapchain.Get(), width, height);

    CreateViewPort(width, height);

    D3D11_RASTERIZER_DESC rDesc;
    ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
    rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

    CreateRasterizer(rDesc);
  

    D3D11_SAMPLER_DESC sDesc;
    ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));
    sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sDesc.MinLOD = 0;
    sDesc.MaxLOD = D3D11_FLOAT32_MAX;

    result = this->device->CreateSamplerState(&sDesc, this->ColorSampler.GetAddressOf());

    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING THE COLOR SAMPLER STATE");
    }
    
    this->device->QueryInterface(IID_PPV_ARGS(d3dDebug.GetAddressOf()));
    return true;
   

   
}

void Armageddon::D3D_graphics::CreateRenderTargetView()
{
    this->swapchain->ResizeTarget(0);


    ID3D11Texture2D* pBackBuffer;
    HRESULT result = this->swapchain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED GETTING BUFFER FOR THE SWAP CHAIN");

    }
    result = this->device->CreateRenderTargetView(pBackBuffer, NULL, this->target_view.GetAddressOf());

    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING THE RENDERTARGETVIEW {0}",result);

    }
    pBackBuffer->Release();
}

void Armageddon::D3D_graphics::CreateViewPort(float width, float height)
{

    


    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));


    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    this->m_screenPos.x = width / 2.f;
    this->m_screenPos.y = height / 2.f;

    this->device_context->RSSetViewports(1, &viewport);
}



bool Armageddon::D3D_graphics::CreateDephtStencilBuffer(int width , int height)
{

    D3D11_TEXTURE2D_DESC DephtStencilDesc;
    DephtStencilDesc.Width = width;
    DephtStencilDesc.Height = height;
    DephtStencilDesc.MipLevels = 1;
    DephtStencilDesc.ArraySize = 1;
    DephtStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DephtStencilDesc.SampleDesc.Count = 1;
    DephtStencilDesc.SampleDesc.Quality = 0;
    DephtStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    DephtStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DephtStencilDesc.CPUAccessFlags = 0;
    DephtStencilDesc.MiscFlags = 0;

    this->device->CreateTexture2D(&DephtStencilDesc, NULL, this->depthBuffer.GetAddressOf());

    HRESULT result = this->device->CreateDepthStencilView(this->depthBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING DEPH STENCIL VIEW");
        return false;
    }

   

    D3D11_DEPTH_STENCIL_DESC Ddesc;
    ZeroMemory(&Ddesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    Ddesc.DepthEnable = true;
    Ddesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    Ddesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

    result = this->device->CreateDepthStencilState(&Ddesc, this->depthStencilState.GetAddressOf());
    if (FAILED(result))
    {
        Armageddon::Log::GetLogger()->error("FAILED CREATING DEPH STENCIL STATE");
        return false;
    }
    return true;
}


void Armageddon::D3D_graphics::ResetDephtStencileBuffer(float width, float height)
{
    if (depthBuffer)
    {
        
        depthBuffer.Reset();
        depthBuffer = (std::nullptr_t) NULL;
        
    }
    if (depthStencilView)
    {
        depthStencilView.Reset();
        depthStencilView = (std::nullptr_t) NULL;
    }
    if (depthStencilState)
    { 
        depthStencilState.Reset();
        depthStencilState = (std::nullptr_t) NULL;
    }

}


void Armageddon::D3D_graphics::CleanRenderTargetView()
{
    if (target_view)
    {
        target_view.Reset();
        target_view =(std::nullptr_t) NULL;
    }


   
}
void Armageddon::D3D_graphics::ResizeBuffer(float width , float height)
{

    CleanRenderTargetView();


    HRESULT hr = this->swapchain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);

    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN RESIZING THE BUFFER  WESH :[{0}]", hr);
    }

    CreateRenderTargetView();
    CreateViewPort(width, height);

}

void Armageddon::D3D_graphics::Begin()
{
    float color[] = { 0.1f,0.1f,0.1f,1.0f };


    r.Bind(this->device_context.Get(), this->depthStencilView.Get());
    r.Clear(this->device_context.Get());
    this->device_context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1);
    this->device_context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
   // Armageddon::Application::GetInstance()->ImGuiBegin();
  
}

void Armageddon::D3D_graphics::BindBackBuffer()
{
    float color[] = { 0.1f,0.1f,0.1f,1.0f };

    r.BindBackBuffer(this->device_context.Get(), this->target_view.GetAddressOf(), this->depthStencilView.Get()); // on remet le buffer normal
    this->device_context->ClearRenderTargetView(this->target_view.Get(), color);
    this->device_context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    this->device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Armageddon::D3D_graphics::End()
{
    /*ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }*/
    this->swapchain->Present(1, 0);
}




Armageddon::D3D_graphics::~D3D_graphics()
{
   // o.clear();
    ImGui::DestroyContext();
}

