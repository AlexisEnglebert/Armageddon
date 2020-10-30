#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <vector>
#include "D3D_vertex.h"
struct SpriteData
{
	const wchar_t* Path;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	Vertex v1;
	Vertex v2;
	Vertex v3;
	Vertex v4;

};

class Sprites
{
public:
	void AddSprite(ID3D11Device* device, const wchar_t* p_Path, float x,  float y , Vertex v1, Vertex v2 , Vertex v3 ,Vertex v4)
	{

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture;
		DirectX::SimpleMath::Vector2 org;
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, p_Path, resource.GetAddressOf(), Texture.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
		hr = resource.As(&tex);
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("ERROR WHEN RESSOURCE AS CAT [{0}]", hr);
		}

		CD3D11_TEXTURE2D_DESC texDesc;
		tex->GetDesc(&texDesc);

		

		org.x = x; //float(texDesc.Width / 2);
		org.y = y;// float(texDesc.Height / 2);



		SpriteData data;
		data.Path = p_Path;
		data.texture = Texture;

		data.v1 = v1;
		data.v2 = v2;
		data.v3 = v3;
		data.v4 = v4;
		//data.origin = org;
		//data.c_origin.x = float(texDesc.Width / 2);
		//data.c_origin.y = float(texDesc.Height / 2);

		Sprites.push_back(data);




	};

	std::vector<SpriteData> GetSprites()
	{
		return Sprites;
	}
private:
	std::vector<SpriteData> Sprites;
};