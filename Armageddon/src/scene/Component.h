#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "../Application.h"
#include <vector>
#include "../Graphics/3D/Renderable/RenderModels.h"
#include "../Graphics/3D/Renderable/Object.h"
#include "../Graphics/3D/Renderable/RenderCube.h"
#include <entt.hpp>
#include "../Graphics/3D/Renderable/RenderSkyBox.h"
#include "../Graphics/3D/Material.h"
#include <stdio.h>
#include <codecvt>
#pragma comment(lib,"d3d11.lib")


struct TransFormComponent
{
	DirectX::XMFLOAT3 Translation = {0.0f,0.0f, 0.0f};
	DirectX::XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT3 Scale = { 1.0f, 1.0f, 1.0f};
	
	const char* ComponentName = "TransForm";
	TransFormComponent() = default;
	TransFormComponent(const TransFormComponent&) = default;
	TransFormComponent(DirectX::XMFLOAT3& transform, const char* ComponentName) : Translation(transform), ComponentName(ComponentName)
	{}
	TransFormComponent(const DirectX::XMFLOAT3& transform ) : Translation(transform)
	{}
	TransFormComponent(const DirectX::XMFLOAT3& transform, const DirectX::XMFLOAT3& Rotation) : Translation(transform), Rotation(Rotation)
	{}
	TransFormComponent(const DirectX::XMFLOAT3 transform, const DirectX::XMFLOAT3 Rotation, const DirectX::XMFLOAT3 Scale) : Translation(transform), Rotation(Rotation), Scale(Scale)
	{}	
	TransFormComponent(float transformX, float transformY, float transformZ,
		float RotationX, float RotationY, float RotationZ
		, float ScaleX, float ScaleY, float ScaleZ) : Translation({ transformX, transformY, transformZ }), 
		Rotation(RotationX, RotationY, RotationZ),
		Scale(ScaleX,  ScaleY, ScaleZ)
	{}
	DirectX::XMMATRIX GetTransformMatrix() const  // TODO FAIRE EN SORTE QUE CA MARCHE
	{
		
		return DirectX::XMMatrixTransformation(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
			, DirectX::XMVectorSet(Scale.x, Scale.y, Scale.z, 1.0f),DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z)
			, DirectX::XMVectorSet(Translation.x, Translation.y, Translation.z, 1.0f));
	/*	DirectX::XMVECTOR m0 = DirectX::XMVectorSet(Scale.x, 0.0f, 0.0f,0.0f);
		DirectX::XMVECTOR m1 =  DirectX::XMVectorSet(0.0f, Scale.y,0.0f, 0.0f);
		DirectX::XMVECTOR m2 =  DirectX::XMVectorSet(0.0f,0.0f, Scale.z, 0.0f);
		DirectX::XMVECTOR m3 =  DirectX::XMVectorSet(Translation.x, Translation.y, Translation.z,1.0f);*/

			/*DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z),DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z)
			 ,DirectX::XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z)));*/
	}
};

struct ModelComponent
{
	const char* ComponentName = "ModelComponent";
	const char* ModelPath = "";
	const char* BmodelPath = "";


	ModelComponent() = default;
	ModelComponent(const ModelComponent&) = default;
	ModelComponent(const char* ModelName, std::map<int, std::unique_ptr<Object>>& DrawableObject, entt::entity EntId) : ModelPath(ModelName), BmodelPath(ModelName)
	{
		auto model = std::make_unique<RenderModels>(ModelName, Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
		//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(model));
		DrawableObject.emplace(std::make_pair((int)EntId,std::move(model)));
	

	};

	void UpdateModel()
	{
		if (ModelPath != BmodelPath)
		{
			/*delete Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects()[(int)Handle].get();
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().erase(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().begin()+(int)Handle);
			auto model = std::make_unique<RenderModels>(ModelPath, Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
			Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(model));
			object = model.get();
			BmodelPath = ModelPath;*/
		}
		// si le modèle a changé
		// on éradique le modèle de l'array
		// on recrée le modèle
		// on le remet dans l'array
	}

	// operator const bool() { return object != nullptr; }
};

struct SkyBoxComponent
{
	const wchar_t* SkyPath;
	//Microsoft::WRL::ComPtr<ID3D11Resource> SkyBoxTextureRessource;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SkyBoxShaderTextRessource;
	//Microsoft::WRL::ComPtr<ID3D11Resource> IrTexresource;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> IrShaderTextRessource;

	SkyBoxComponent() = default;
	SkyBoxComponent(const SkyBoxComponent&) = default;
	SkyBoxComponent(const wchar_t* SkyPath, const wchar_t* IRSkyPath ,std::map<int, std::unique_ptr<Object>>& DrawableEntity, entt::entity EntId) : SkyPath(SkyPath)
	{
		auto SkyBox = std::make_unique<RenderSkyBox>(SkyPath, IRSkyPath,Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
	//	SkyBoxTextureRessource = SkyBox.get()->Texresource;
	//	SkyBoxTextureRessource = SkyBox.get()->ShaderTextRessource;
		//SkyBoxShaderTextRessource = SkyBox.get()->IrTexresource;
		//IrShaderTextRessource = SkyBox.get()->IrShaderTextRessource;
		//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(SkyBox));
		DrawableEntity.emplace(std::make_pair((int)EntId,std::move(SkyBox)));
	}

};
struct MeshComponent
{
	const char* t = "t";
	MeshComponent(const MeshComponent&) = default;
	MeshComponent() = default;
	MeshComponent(std::map<int, std::unique_ptr<Object>>& DrawableEntity, entt::entity EntId)
	{
		auto r = std::make_unique<RenderCube>(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
		//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(r));
		DrawableEntity.emplace(std::make_pair((int)EntId,std::move(r)));

	};
	void render()
	{
		Armageddon::Log::GetLogger()->info("NULE NULE NULE");
		/*if (obj != nullptr)
		{
			Armageddon::Log::GetLogger()->info("DRAW MESH");
			obj->UpDateConstantBuffer(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetCamera());
			obj->Draw(Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext().Get());
		}*/
	}
};


struct CameraComponent
{
	CameraComponent() = default;

};

struct ScriptComponent
{
	ScriptComponent() = default;

};


struct MaterialComponent
{
	Material Mat;
	const wchar_t* NormalPath;
	const wchar_t* ColorPath ;
	const wchar_t* SpecPath;
	const wchar_t* OcclusionPath;
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;

/*	std::wstring name = convert.from_bytes("Steve Nash");
	const wchar_t* szName = name.c_str();*/
	void SetColorMap(const wchar_t* AlbedoMap)
	{
		//ColorPath = AlbedoMap;
		Mat.SetColorMap(AlbedoMap);
	}
	void SetNormalMap(const wchar_t* NormalMap)
	{
		Mat.SetNormalMap(NormalMap);
	}
	void SetSpecularMap(const wchar_t* SpecularMap)
	{
		Mat.SetSpecMap(SpecularMap);
	}
	void SetOcclusionMap(const wchar_t* OcclusionMap)
	{
		Mat.SetOcclusionMap(OcclusionMap);
	}
	MaterialComponent() = default;
	MaterialComponent(const MaterialComponent&) = default;
	MaterialComponent(Microsoft::WRL::ComPtr <ID3D11Device> device, Microsoft::WRL::ComPtr <ID3D11DeviceContext> device_context) {
		//NormalPath = L"..\\TestApplication\\assets\\Textures\\default_normal.png";
		//ColorPath = L"..\\TestApplication\\assets\\Textures\\checker_texture.png";
		Armageddon::Log::GetLogger()->info("MAT COMPONENT");
		Mat = Material(device, device_context);
		//Mat.SetColorMap(ColorPath);
	//	Mat.SetNormalMap(NormalPath);
	//	Mat.SetOcclusionMap(OcclusionPath);
	//	Mat.SetSpecMap(SpecPath);
	};
};

struct TagComponent
{
	std::string TagName;
	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(std::string tag) : TagName(tag) {}
};
