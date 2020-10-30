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



            // FACE FACE
            Vertex(-0.1f,  -0.1f, -0.1f,  0.0f, 1.0f,0,0,-1.0), /// botom Left Point  - [0] // bas gauche
            Vertex(-0.1f,   0.1f, -0.1f,  0.0f, 0.0f,0,0,-1.0), //top Left Point     - [1] // haut gauche
            Vertex{0.1f,   0.1f, -0.1f,  1.0f, 0.0f,0,0,-1.0},     //               -[2]   // haut droit
            Vertex(0.1,     -0.1f,-0.1f,  1.0f, 1.0f,0,0,-1.0), //Right Point      - [3]    // bas droit



            Vertex(0.1f,  -0.1f,  0.1f,  0.0f, 1.0f,0,0,-1.0), /// botom Left Point  - [4] // bas gauche
            Vertex(0.1f,   0.1f, 0.1f,  0.0f, 0.0f,0,0,-1.0), //top Left Point     - [5] // haut gauche
            Vertex{-0.1f,   0.1f, 0.1f,  1.0f, 0.0f,0,0,-1.0},     //               -[6]   // haut droit
            Vertex(-0.1,     -0.1f, 0.1f,  1.0f, 1.0f,0,0,-1.0), //Right Point      - [7]    // bas droit



            Vertex(0.2f,  0.2f, -0.1f,  0.0f, 1.0f,0,0,-1.0), /// botom Left Point  - [0] // bas gauche
            Vertex(0.2f,   0.4f, -0.1f,  0.0f, 0.0f,0,0,-1.0), //top Left Point     - [1] // haut gauche
            Vertex{0.4f,   0.4f, -0.1f,  1.0f, 0.0f,0,0,-1.0},     //               -[2]   // haut droit
            Vertex(0.4,    0.2f,-0.1f,  1.0f, 1.0f,0,0,-1.0), //Right Point      - [3]    // bas droit


        };
		 InitVertexBuffer(device.Get(),device_context.Get(),vv, ARRAYSIZE(vv));



         DWORD indices[] =
         {
             // avant -- face
             0,1,2,
             2,3,0,
             // arrière -- face

             4,5,6,
             6,7,4,

             //haut -- face
           1,6,5,
           5,2,1,



           //bas -- face
           7,0,3,
           3,4,7,

           // droit  -- face
           3,2,5,
           5,4,3,
           //gauche -- face
           7,6,1,
           1,0,7,

           8,9,10,
           10,11,8



         };
		 InitIndexBuffer(device.Get(), device_context.Get(),indices, ARRAYSIZE(indices));
         D3D11_INPUT_ELEMENT_DESC layout[] =
         {
             {"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

         };
		 InitShaders(device,L"..\\bin\\Debug-x64\\Armageddon\\", L"VertexShader.cso", L"PixelShader.cso", layout, ARRAYSIZE(layout));

		 InitConstantBuffer(device.Get(), device_context.Get());
         //graphics.GetVectorObjects().push_back(&o);
         Armageddon::Log::GetLogger()->trace("CREATED MODEL");
	};
    virtual void Draw(ID3D11DeviceContext* device_contex) override
    {

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