#pragma once
#include "Object.h"
class RenderCube : public Object
{
public:
	RenderCube(Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context) : device(device), device_context(device_context)
	{
        SetName("Cube");

        Vertex vv[] =
        {
            // X - Y - Z



            /*


            1----------------------------3
            |                            |
            |                            |
            |                            |
            |                            |
            |                            |
            |                            |
            |                            |
            |                            |
            0----------------------------2


            */



            // Front Face
             Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
             Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
             Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
             Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

     // Back Face
             Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
             Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
             Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
             Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

     // Top Face
             Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
             Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
             Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
            Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

     // Bottom Face
            Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
             Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
             Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
             Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

     // Left Face
             Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
             Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
             Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
            Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

     // Right Face
            Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
            Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
            Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
            Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),


        };
		 InitVertexBuffer(device.Get(),device_context.Get(),vv, ARRAYSIZE(vv));



         DWORD indices[] =
         {
             // Front Face
         0,  1,  2,
         0,  2,  3,

         // Back Face
         4,  5,  6,
         4,  6,  7,

         // Top Face
         8,  9, 10,
         8, 10, 11,

         // Bottom Face
         12, 13, 14,
         12, 14, 15,

         // Left Face
         16, 17, 18,
         16, 18, 19,

         // Right Face
         20, 21, 22,
         20, 22, 23

         };
		 InitIndexBuffer(device.Get(), device_context.Get(),indices, ARRAYSIZE(indices));
         D3D11_INPUT_ELEMENT_DESC layout[] =
         {
            {"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},


         };
		 InitShaders(device,L"..\\bin\\Debug-x64\\Armageddon\\", L"VertexShader.cso", L"PixelShader.cso", layout, ARRAYSIZE(layout));

		 InitConstantBuffer(device.Get(), device_context.Get());
         //graphics.GetVectorObjects().push_back(&o);
        auto mat = Material(device, device_context);
        mat.setMaterialName("DEFAULT MATERIAL");
        ObjectMaterials.push_back(mat);
         Armageddon::Log::GetLogger()->trace("CREATED MODEL");
	};
    virtual void Draw(ID3D11DeviceContext* device_contex) override
    {
        ObjectMaterials[0].BindRessources();
        BindShader(device_contex);
        BindConstantBuffer(device_contex);
        BindInputLayout(device_contex);
        BindVertexBuffer(device_contex);
        BindIndexBuffer(device_contex);
        DrawIndex(device_contex);
    };

private:
    Microsoft::WRL::ComPtr <ID3D11Device>& device;
    Microsoft::WRL::ComPtr <ID3D11DeviceContext>& device_context;
};