#pragma once
#include "Component.h"
#include "entt.hpp"
#include "Scene.h"
#include "../Log.h"
class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handleEntity, Scene* scene) : HandleEntity(handleEntity), scene(scene) {  }
	//Entity(Entity& ent) = default;
	Entity(const Entity& ent) = default;
	template<typename T,typename... Args>
	T& AddComponent(Args&&... args)
	{
		return scene->g_registry.emplace<T>(HandleEntity, std::forward<Args>(args)...);
	};
	template<typename T>
	void DeleteComponent()
	{
		return scene->g_registry.remove<T>(HandleEntity);
	};
	template<typename T>
	bool HasComponent()
	{
		return scene->g_registry.has<T>(HandleEntity);
	};
	template<typename T>
	T& GetComponent() 
	{
		return scene->g_registry.get<T>(HandleEntity);
	};
	std::unique_ptr<Object> SetModel(const char* modelPath)
	{
		auto model = std::make_unique<RenderModels>(modelPath,Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDevice(), Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetComptrDeviceContext());
		Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects().push_back(std::move(model));
		this->Rendermodel = model.get();
		return model;
	}
	entt::entity GetHandle()
	{
		return HandleEntity;
	};
	std::unique_ptr<Object>& GetEntityObject()
	{
		//Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects();
		/*for (auto& mods : Armageddon::Application::GetInstance()->GetWindow()->GetWindowGraphics()->GetVectorObjects())
		{
			if (mods.get() == Rendermodel)
			{
				return mods;
			}
			
		}*/
		
	};
	operator bool() const { return HandleEntity != entt::null; }

private:

	RenderModels* Rendermodel = nullptr;
	entt::entity HandleEntity{ entt::null };	
	Scene* scene = nullptr;
};