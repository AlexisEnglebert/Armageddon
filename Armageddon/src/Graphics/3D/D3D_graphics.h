#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "D3D_Shaders.h"
#include "D3D_vertex.h"
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include <vector>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBufferTypes.h"
#include "ConstantBuffer.h"
#include <memory>
#include <SimpleMath.h>

#include "Camera.h"
#include "../../Macros.h"
#include <PrimitiveBatch.h>
#include "OffScreenRenderTargetView.h"
#include "Renderable/Object.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
namespace Armageddon
{
	//extern thread_local ImGuiContext* ImguiContext;

	struct SpritesData
	{
	
	};

	class DLL D3D_graphics {
	public:
		~D3D_graphics();
		bool Init(HWND hwnd, int height, int width);
		void RenderFrame();
		bool CreateDephtStencilBuffer(int width, int height);
		void ResetDephtStencileBuffer(float width, float height);
		int max_Vertex = 500;
		
		
		HWND* GetHandleWindow() { return handlewnd; }
		void ResizeBuffer(float width, float height);
		OfsRenderTargetView* GetOfsRender() { return &r; };
		void Begin();
		void BindBackBuffer();
		void End();

		/* SPRITES */
		//std::unique_ptr<DirectX::PrimitiveBatch<Vertex>> m_primitive = nullptr;

		DirectX::SimpleMath::Vector2 m_screenPos;
		DirectX::SimpleMath::Vector2 m_origin;

		Camera* GetCamera()
		{
			return &camera;
		}
		ID3D11Device* Getdevice()
		{
			return device.Get();
		};
		ID3D11DeviceContext* Getdevicecontext()
		{
			return device_context.Get();
		};

		Microsoft::WRL::ComPtr <ID3D11Device>& GetComptrDevice()
		{
			return device;
		};

		Microsoft::WRL::ComPtr <ID3D11DeviceContext>& GetComptrDeviceContext()
		{
			return device_context;
		};
		std::vector<std::unique_ptr<Object>>& GetVectorObjects();

	IDXGISwapChain* GetSwapChain()
	{
		return swapchain.Get();
	};
		
	ID3D11Debug* GetDebug()
	{
		return d3dDebug.Get();
	};
	float GetDeltaTime()
	{
		return DeltaTime;
	}
	void SetWireFrame(bool wireframe);

	private:
		Microsoft::WRL::ComPtr <IDXGISwapChain> swapchain;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> target_view;
		Microsoft::WRL::ComPtr <ID3D11Device> device;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context;
		Microsoft::WRL::ComPtr <ID3D11Debug> d3dDebug;

		bool InitDirectX(HWND hwnd, float height, float width);

		
		void CreateRenderTargetView();
		void CreateViewPort(float width, float height);
		void CleanRenderTargetView();
		void CreateRasterizer(D3D11_RASTERIZER_DESC rDesc);
		void ChangeRasterizer(D3D11_RASTERIZER_DESC rDesc);
		D3D11_VIEWPORT viewport;
		int windowWidth = 0;
		int windowHeight = 0;
		int Index = 0;
		HWND* handlewnd;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> FrameBuffer;
		
		/* DELTA TIME */
		float DeltaTime = 0.0f;
		float OldDeltaTime = 0.0f;
		float CurrentTime = 0.0f;


		/*BATCH RENDERING*/
		std::unique_ptr<SpritesData[]> SpriteQueue;
		int n_SpriteCount = 0; 

		/* ImGui */

		/*TRIANGLE SETUP*/
	//	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;


		ConstantBuffer<CB_VS_SHADER> ConstantBuffer;
		VertexBuffer<Vertex> vertexBuffer;

		VertexShader vertexShader;
		PixelShader pixelShader;

		OfsRenderTargetView r;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

		//Create  Deph Buffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer;
		
		Microsoft::WRL::ComPtr<ID3D11SamplerState> ColorSampler;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textures;


		// FrameBuffer Tex

		bool WireFrame = false;

		IndexBuffer indicesBuffer;
		 
		int NumOfVert = 0;

		VertexBuffer<Vertex> VertexBuffer;

	


		Camera camera;




	};
}