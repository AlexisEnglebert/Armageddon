#pragma once
#include "../../Includes.h"
#include <d3d11.h>
#include <wrl/client.h>


class DLL OfsRenderTargetView
{
public:

	bool Init(ID3D11Device* device, IDXGISwapChain* swapchain, float width, float height);
	void ResizeRenderTargetView(ID3D11Device* device,IDXGISwapChain* swapchain, float width, float height , ID3D11Debug* debug );
	void CleanRenderTargetView();
	void CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapchain);
	void CreateScreenTexture(ID3D11Device* device , float width, float height);
	void CreateShaderResourceView(ID3D11Device* device);
	void Bind(ID3D11DeviceContext* device_context);
	void Bind(ID3D11DeviceContext* device_context, ID3D11DepthStencilView* dephStencil);
	void Clear(ID3D11DeviceContext* device_context);
	void BindBackBuffer(ID3D11DeviceContext* device_context, ID3D11RenderTargetView* render);
	void BindBackBuffer(ID3D11DeviceContext* device_context, ID3D11RenderTargetView** render, ID3D11DepthStencilView* dephStencil);
	ID3D11ShaderResourceView* GetShaderRessource() {return ShaderRessourceView.Get();};
	ID3D11ShaderResourceView** GetAddressOfShaderRessource() {return ShaderRessourceView.GetAddressOf();};
	ID3D11Texture2D* GetScreenTexture() { return ScreenTex.Get(); };
	ID3D11RenderTargetView* GetRenderTargetView() { return pTargetView.Get(); };
	float GetImageX(){return ImageX;};
	float GetImageY(){return ImageY;};

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderRessourceView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> ScreenTex;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTargetView;
	float ImageX = 0, ImageY =0 ;
};