#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "../Application.h"
#include <vector>
#include "../Graphics/3D/Renderable/RenderModels.h"
#include "../Graphics/3D/Renderable/Object.h"
#include <entt.hpp>
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
	TransFormComponent(const DirectX::XMFLOAT3& transform, const DirectX::XMFLOAT3& Rotation, const DirectX::XMFLOAT3& Scale) : Translation(transform), Rotation(Rotation), Scale(Scale)
	{}	

	DirectX::XMMATRIX GetTransformMatrix() const  // TODO FAIRE EN SORTE QUE CA MARCHE
	{
		return/* DirectX::XMMatrixTransformation(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)
			, DirectX::XMVectorSet(Scale.x, Scale.y, Scale.z, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(Rotation.x, Rotation.y, Rotation.z, 0.0f)
			, DirectX::XMVectorSet(Translation.x, Translation.y, Translation.z, 0.0f));*/
			DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z),DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z)
			 ,DirectX::XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z)));
	}
};

struct ModelComponent
{
	Object* object = nullptr;
	const char* ComponentName = "ModelComponent";
	const char* ModelPath = "";
	const char* BmodelPath = "";
	entt::entity Handle;
	ModelComponent() = default;
	ModelComponent(const ModelComponent&) = default;
	ModelComponent(const char* ModelName, entt::entity handle) : ModelPath(ModelName), BmodelPath(ModelName),Handle(handle)
	{
		auto model = std::make_unique<RenderModels>(ModelName, Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(model));
		object = model.get();
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

	 operator const bool() { return object != nullptr; }
};
struct MeshComponent
{
	MeshComponent() = default;

};

struct MaterialComponent
{
	MaterialComponent() = default;
	MaterialComponent(const MaterialComponent&) = default;
	//MaterialComponent(const char* ModelName) : ModelPath(ModelName)
};

struct TagComponent
{
	std::string TagName;
	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(std::string tag) : TagName(tag) {}
};
