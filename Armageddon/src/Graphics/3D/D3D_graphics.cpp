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
#include <SpriteBatch.h>
#include "../../Application.h"
#include <PrimitiveBatch.h>
#include "ScreenGrab.h"
bool Armageddon::D3D_graphics::Init(HWND hwnd, int height, int width)
{
    if (!InitDirectX(hwnd, height, width))
    {
        return false;
    }
    if (!InitShader())
    {
        return false;
    }
    if (!InitScene())
    {
        return false;
    }

   // Armageddon::Application::GetInstance()->Init();
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED COINIT {0}", hr);
    }
    Armageddon::Application::GetInstance()->ImGuiInit();


    //Armageddon::Application::GetInstance()->Init();


    //  io.DisplaySize = ImVec2((float)width, (float)height);


    ImGui_ImplWin32_Init(hwnd);
     ImGui_ImplDX11_Init(this->device.Get(), this->device_context.Get());

    handlewnd = &hwnd;

    return true;
}
/*static void test()
{
    HRESULT a = ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

}*/
static float f = 0;
/*bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);*/


void Armageddon::D3D_graphics::RenderFrame()
{
    float color[] = { 0.1f,0.1f,0.1f,1.0f };
   
   
    r.Bind(this->device_context.Get(), this->depthStencilView.Get());
    r.Clear(this->device_context.Get());
    //this->device_context->ClearRenderTargetView(this->target_view.Get(), color);
    this->device_context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //ICI QU'ON FAIT TOUT LES RENDU APRES AVOIR CLEAN LE PLAN
    
    /*ID3D11Texture2D* pBuffer;
    HRESULT result = this->swapchain->GetBuffer(0, IID_PPV_ARGS(&pBuffer));
    this->device_context->CopyResource(SceneTex.Get(), pBuffer);*/

   
   Armageddon::Application::GetInstance()->ImGuiBegin();

    //ImGuiRender();
    Armageddon::Application::GetInstance()->ImGuiRender();
    this->device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* WORLD MATRIX*/
   /* DirectX::XMMATRIX WorldMat = DirectX::XMMatrixIdentity();
   

    
    f += 0.01;
     
    WorldMat += DirectX::XMMatrixRotationY(90);*/

   // WorldMat += DirectX::XMMatrixRotationY(f);

   // camera.AdjustRotation(0.1f, 0.0f, 0.0f);
   /* ConstantBuffer.data.mat = WorldMat *  camera.GetViewMatrix() * camera.GetProjectionMatrix() ;
   //a ConstantBuffer.data.mat = DirectX::XMMatrixTranspose(ConstantBuffer.data.mat);
    ConstantBuffer.data.Yoffset = 0;
    ConstantBuffer.data.Xoffset = 0;
    // Armageddon::Log::GetLogger()->trace(ConstantBuffer.data.Yoffset);
    if (!ConstantBuffer.ApplyChanges())
    {
        Armageddon::Log::GetLogger()->error("ERRO WHEN APPLYING CHANGES");
    }

    this->device_context->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAdressOf());*/
        

    /***********SHADER*******************************/

 

    /***********Texture Sampler*******************************/

    this->device_context->PSSetSamplers(0, 1, this->ColorSampler.GetAddressOf());

    /***********DEPHT BUFFER*******************************/

   this->device_context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);

    /***********RASTERIZER STATE*******************************/

    this->device_context->RSSetState(this->rasterizerState.Get());


    /***********UPDATE LES CONSTANTS BUFFER*******************************/

    /*this->device_context->VSSetShader(vertexShader.GetShader(), NULL, 0);
    this->device_context->PSSetShader(pixelShader.GetShader(), NULL, 0);
    this->device_context->IASetInputLayout(this->vertexShader.GetInputLayout());*/
    this->device_context->PSSetShaderResources(0, 1, textures.GetAddressOf());

    //DrawQuad();

   /* UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->device_context->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    this->device_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    this->device_context->DrawIndexed(indicesBuffer.GetSize(), 0, 0);*/

    

  Armageddon::Application::GetInstance()->OnRender();

  //this->device_context->DrawIndexed(indicesBuffer.GetSize(), 0, 0);


  for (UINT i = 0; i < o.size(); i++)
  {
      o[i].get()->UpDateConstantBuffer(&camera);
      o[i].get()->Draw(device_context.Get());
  }



















     ImGui::Render();
    r.BindBackBuffer(this->device_context.Get(), this->target_view.GetAddressOf(), this->depthStencilView.Get());
    this->device_context->ClearRenderTargetView(this->target_view.Get(), color);
    this->device_context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


    this->device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* WORLD MATRIX*/
   /* DirectX::XMMATRIX d = DirectX::XMMatrixIdentity();



    f += 0.01;
    // WorldMat += DirectX::XMMatrixRotationX(f);
     //WorldMat += DirectX::XMMatrixRotationY(-f);

    // WorldMat += DirectX::XMMatrixRotationY(f);

    // camera.AdjustRotation(0.1f, 0.0f, 0.0f);
    ConstantBuffer.data.mat = d * f * camera.GetViewMatrix() * camera.GetProjectionMatrix();
    //a ConstantBuffer.data.mat = DirectX::XMMatrixTranspose(ConstantBuffer.data.mat);
    ConstantBuffer.data.Yoffset = 0;
    ConstantBuffer.data.Xoffset = 0;
    // Armageddon::Log::GetLogger()->trace(ConstantBuffer.data.Yoffset);
    if (!ConstantBuffer.ApplyChanges())
    {
        Armageddon::Log::GetLogger()->error("ERRO WHEN APPLYING CHANGES");
    }

    this->device_context->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAdressOf());*/


    /***********SHADER*******************************/



    /***********Texture Sampler*******************************/

   // this->device_context->PSSetSamplers(0, 1, this->ColorSampler.GetAddressOf());

    /***********DEPHT BUFFER*******************************/

   this->device_context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);

    /***********RASTERIZER STATE*******************************/

    //this->device_context->RSSetState(this->rasterizerState.Get());


    /***********UPDATE LES CONSTANTS BUFFER*******************************/

   /* this->device_context->VSSetShader(vertexShader.GetShader(), NULL, 0);
    this->device_context->PSSetShader(pixelShader.GetShader(), NULL, 0);
    this->device_context->IASetInputLayout(this->vertexShader.GetInputLayout());*/
   // this->device_context->PSSetShaderResources(0, 1,r.GetAddressOfShaderRessource());

    //DrawQuad();

   /*  stride = sizeof(Vertex);
     offset = 0;
    this->device_context->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    this->device_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    this->device_context->DrawIndexed(indicesBuffer.GetSize(), 0, 0);*/

   //this->device_context->DrawIndexed(indicesBuffer.GetSize(), 0, 0);
  // this->device_context->DrawIndexed(indicesBuffer.GetSize(), 0, 0);

  // m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->device_context.Get());
   /*m_primitive = std::make_unique<DirectX::PrimitiveBatch<Vertex>>(this->device_context.Get());
   m_primitive->Begin();
   for (SpriteData sData : sprite.GetSprites())
   {
       this->device_context->VSSetShader(vertexShader.GetShader(), NULL, 0);
       this->device_context->PSSetShader(pixelShader.GetShader(), NULL, 0);
       this->device_context->IASetInputLayout(this->vertexShader.GetInputLayout());
       this->device_context->PSSetShaderResources(0, 1, sData.texture.GetAddressOf());

       m_primitive->DrawQuad(sData.v1, sData.v2, sData.v3, sData.v4);
   }
   m_primitive->End();
*/

  /* if (sprite.GetSprites().size() > 0)
   {
  

       m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, NULL, ColorSampler.Get(), NULL, rasterizerState.Get(), [=]
       {
          this->device_context->PSSetShader(pixelShader.GetShader(), NULL, 0);
           this->device_context->VSSetShader(vertexShader.GetShader(), NULL, 0);
           this->device_context->IASetInputLayout(this->vertexShader.GetInputLayout());
       //   this->device_context->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAdressOf());

         // 
       });
       m_spriteBatch->SetViewport(viewport);

       for (SpriteData data : sprite.GetSprites())
       {
           m_spriteBatch->Draw(data.texture.Get(), data.origin, nullptr, DirectX::Colors::White, 0.0f, data.c_origin);
        
       }
     
      
       m_spriteBatch->End();
       
   } 
*/

    for(UINT i = 0 ; i < o.size();i++)
    {
        o[i].get()->UpDateConstantBuffer(&camera);
        o[i].get()->Draw(device_context.Get());
    }
    Armageddon::Application::GetInstance()->OnRender();

  // Armageddon::Application::GetInstance()->ImGuiEnd();
  
   ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
   if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
   {
       ImGui::UpdatePlatformWindows();
       ImGui::RenderPlatformWindowsDefault();
   }
    this->swapchain->Present(1,0);



}
void Armageddon::D3D_graphics::DrawQuad()
{

 

}
void Armageddon::D3D_graphics::ImGuiRender()
{
     
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

std::vector<std::unique_ptr<Object>>& Armageddon::D3D_graphics::GetVectorObjects()
{
   return o;
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
    DephtStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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


bool Armageddon::D3D_graphics::InitShader()
{
    std::wstring shaderFolder = L"";
#pragma region DertmineShaderPath
    if (IsDebuggerPresent() == TRUE)
#ifdef _DEBUG
#ifdef _WIN64
        shaderFolder = L"..\\bin\\Debug-x64\\Armageddon\\";
#endif
#else
        shaderFolder = L"..\\bin\\Release-x64\\Armageddon\\";

#endif*/
    Armageddon::Log::GetLogger()->trace("Fye");

   D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},


    };
   Armageddon::Log::GetLogger()->trace("CREATE INPUT LAYOUT");

    if (!vertexShader.Init(this->device, shaderFolder + L"VertexShader.cso",layout,ARRAYSIZE(layout)))
    {
        Armageddon::Log::GetLogger()->error("FAILED INITIALIZE VERTEX SHADERS ");
        return false; 
    }

    if (!pixelShader.Init(this->device, shaderFolder + L"PixelShader.cso"))
    {
        Armageddon::Log::GetLogger()->error("FAILED INITIALIZE PIXEL SHADERS ");
        return false;
    }


   
    return true;

}

bool Armageddon::D3D_graphics::InitScene()
{



    return true;
    
}





void Armageddon::D3D_graphics::DrawTriangle(Vertex v[], int Vertexcount,const wchar_t* FilePath, float x , float y)
{
/*
    Vertex vv[] =
    {
        Vertex(-1.5f,  -0.1f, 1.0f, 0.0f, 1.0f,1,0,0), /// botom Left Point  - [0] // bas gauche
        Vertex(-1.5f,   0.1f,1.0f,  0.0f, 0.0f,0,1,0), //top Left Point     - [1] // haut gauche
        Vertex{0.0f,  -0.1f, 1.0f,  0.0f, 1.0f,0,0,1},     //               -[2]   // haut droit
        Vertex(0.0,   0.1f, 1.0f, 0.0f, 0.0f,1,0,1), //Right Point      - [3]    // bas droit



       /* Vertex(-0.3f,  -0.3f,  0.0f, 1.0f), /// botom Left Point  - [0]
        Vertex(-0.3f,   0.3f,  0.0f, 0.0f), //top Left Point     - [1]
        Vertex{ 0.05f, -0.3f,   0.0f, 1.0f},     //               -[2]
        Vertex(0.05f,   0.3f,  0.0f, 0.0f), //Right Point      - [3]*/


   /* };

    HRESULT hr = vertexBuffer.Initialize(this->device.Get(), this->device_context.Get(), vv, ARRAYSIZE(vv));


    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED INITIALIZE VERTEX BUFFER  ");

    }
    DWORD indices[] =
    {
        0,1,2,
        1,2,3,

    };

    hr = this->indicesBuffer.Init(this->device.Get(), indices, ARRAYSIZE(indices));

    Armageddon::Log::GetLogger()->error("TEST");*/

 
    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->device_context.Get());    //pointeur sur le sprite 
   HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"..\\TestApplication\\assets\\Textures\\checker_board.png", resource.GetAddressOf(), textures.GetAddressOf());
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN CREATING WIC TEXTURE [{0}]", hr);
    }
    Microsoft::WRL::ComPtr<ID3D11Texture2D> cat;
    hr = resource.As(&cat);
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN RESSOURCE AS CAT [{0}]", hr);
    }
    CD3D11_TEXTURE2D_DESC catDesc;
    cat->GetDesc(&catDesc);

    this->m_origin.x = float(catDesc.Width / 2);
    this->m_origin.y = float(catDesc.Height / 2);

  //  sprite.AddSprite(this->device.Get(),FilePath , x ,y );

/*    Armageddon::Log::GetLogger()->trace("CURRENT SPRITES : [{0}]",sprite.GetSprites().size());
  
  
    DWORD indices[] =
    {
        0,1,2,
        1,2,3,
    };

   // HRESULT hr = this->indicesBuffer.Init(this->device.Get(), indices, ARRAYSIZE(indices));
     Microsoft::WRL::ComPtr<ID3D11Resource> resource;
     HRESULT  hr = DirectX::CreateWICTextureFromFile(this->device.Get(), FilePath, resource.GetAddressOf(),textures.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3D11Texture2D> cat;
    hr = resource.As(&cat);
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN RESSOURCE AS CAT [{0}]", hr);
    }
    CD3D11_TEXTURE2D_DESC catDesc;
    cat->GetDesc(&catDesc);

    this->m_origin.x = float(catDesc.Width / 2);
    this->m_origin.y = float(catDesc.Height / 2); 

    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED INITIALIZE WIC TEXTURE  ");

    }

    /* SPRITES */
    //Microsoft::WRL::ComPtr<ID3D11Texture2D> cat;
 

}

void Armageddon::D3D_graphics::DrawSprite(const wchar_t* FilePath, float x, float y)
{


    //sprite.AddSprite(this->device.Get(), FilePath, x, y);
    Armageddon::Log::GetLogger()->trace("CURRENT SPRITES : [{0}]", sprite.GetSprites().size());


    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
   HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), FilePath, resource.GetAddressOf(), textures.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3D11Texture2D> cat;
    hr = resource.As(&cat);
    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("ERROR WHEN RESSOURCE AS CAT [{0}]", hr);
    }
    CD3D11_TEXTURE2D_DESC catDesc;
    cat->GetDesc(&catDesc);

    this->m_origin.x = float(catDesc.Width / 2);
    this->m_origin.y = float(catDesc.Height / 2);

    if (FAILED(hr))
    {
        Armageddon::Log::GetLogger()->error("FAILED INITIALIZE WIC TEXTURE  ");

    }


  


}

void Armageddon::D3D_graphics::DrawTest(const wchar_t* FilePath)
{

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex;
    HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), FilePath, nullptr, tex.GetAddressOf());

    SpritesData* sprite = &SpriteQueue[n_SpriteCount];
    sprite->texture = tex.Get();
    // sprite->vertex = &vert;

    // Armageddon::Log::GetLogger()->info("ARRAY SIZE : " +));

    n_SpriteCount++;

    D3D11_MAPPED_SUBRESOURCE mappedBuffer;

    hr = this->device_context->Map(this->VertexBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &mappedBuffer);
        if (FAILED(hr))
        {
            Armageddon::Log::GetLogger()->error("ERROR WHEN MAPPING {0}",hr);
        }

   /* Vertex v[] =
    {
        Vertex(-0.5f,  -0.5f,  0.0f, 1.0f), /// botom Left Point  - [0]
        Vertex(-0.5f,   0.5f,  0.0f, 0.0f), //top Left Point     - [1]
        Vertex{ 0.0f, -0.5f,   1.0f, 1.0f},     //               -[2]
        Vertex(0.0f,   0.5f,  1.0f, 0.0f), //Right Point      - [3]

    };

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    //  Disable GPU access to the vertex buffer data.
  
    this->device_context->Map(VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    //  Update the vertex buffer here.
    memcpy(mappedResource.pData, v, sizeof(v));
    //  Reenable GPU access to the vertex buffer data.
    this->device_context->Unmap(VertexBuffer.Get(), 0);*/
}


Armageddon::D3D_graphics::~D3D_graphics()
{
    
    ImGui::DestroyContext();
}

