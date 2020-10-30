#pragma once
#include "entt.hpp"
#include "component.h"
#include "../Macros.h"
#include "../Graphics/3D/Renderable/RenderModels.h"
#include "../Application.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Includes.h"
#include "../Graphics/3D/Renderable/Object.h"
#include <vector>
#include <stdio.h>
class Entity;
class DLL Scene
{
public:
	Scene();
	void UpdateScene();
	Entity CreateEntity();
	Entity GetEntityById(entt::entity ent);
	void DrawGameObjectScene();
	void DrawEntity(Entity& ent);
	entt::registry g_registry;

private:
	void DrawTransFormComponent(Entity& ent);
	void DrawModelComponent(Entity& ent);
	friend class Entity;
	std::vector<Entity> EntityList;
};